#include <atomic>
#include <array>
#include <chrono>
#include <deque>
#include <iostream>
#include <map>
#include <random>
#include <stdio.h>
#include <thread>
#include <utility>

#include "finders.h"
#include "custom_finders.h"

/*
 * This program outputs awesome island seeds
 * An awesome island seed is a seed in which
 * spawn is located on an island, that island is near a mushroom biome
 * and there is at least one village island nearby
 */

std::atomic<uint64_t> numFinds{0};
const auto startTime = std::chrono::steady_clock::now();

void search()
{
    const auto version = MC_NEWEST;
    const int64_t minIslandSizeBlocks = 500;
    const int64_t maxIslandSizeBlocks = 2000000;
    Generator g;

    // Allocate and initialize a stack of biome layers that reflects the biome
    // generation of Minecraft
    setupGenerator(&g, version, 0);

    std::random_device rd;
    std::mt19937_64 eng(rd()); 
    std::uniform_int_distribution<unsigned long long> distr;

    while(true)
    {
        int64_t seed = distr(eng);
        //int64_t seed = -3874624793136709106;

        //std::cout << "checking " << seed << std::endl;

        // Go through the layers in the layer stack and initialize the seed
        // dependent aspects of the generator.
        applySeed(&g, DIM_OVERWORLD, seed);

#if 0
        //quick checks for ocean nearby, creates false negatives but speeds things up a lot

        bool foundOcean = false;

        for(int i = 0; i < 160; i += 16)
        {
            auto pos = Pos{0, i};

            if (isOceanic(getBiomeAt(&g, 1, pos.x, 0, pos.z)))
            {
                foundOcean = true;
                break;
            }
        }

        if(!foundOcean)
        {
            continue;
        }
#endif


        StrongholdIter sh;
        
        bool foundStronghold = true;
        bool foundMushroomStronghold = false;
        int shIt = 0;
        Pos strongholdPos;

        initFirstStronghold(&sh, version, seed);

        while(foundStronghold && shIt < 3)
        {
            auto pos = sh.pos;
            auto surface = getBiomeAt(&g, 1, pos.x, 63, pos.z);
            auto deep = getBiomeAt(&g, 1, pos.x, -56, pos.z);

            if (deep == lush_caves && (surface == mushroom_fields || surface == mushroom_field_shore))
            {
                foundMushroomStronghold = true;
                strongholdPos = pos;
                break;
            }

            foundStronghold = nextStronghold(&sh, &g);
            shIt++;
        }

        if (!foundMushroomStronghold)
        {
            continue;
        }

        std::cout << "found stronghold-only seed: " << seed << std::endl;

    	Pos spawnPos = getSpawn(&g);

        if(!isIsland(&g, 4, spawnPos, minIslandSizeBlocks, maxIslandSizeBlocks))
        {
            continue;
        }
    
        auto foundVillages = getVillagesInRange(&g, strongholdPos, seed, 500);

        if(foundVillages.empty())
        {
            continue;
        }

        bool foundIslandVillage = false;

        for(auto& islandPos : foundVillages)
        {
            if(isIsland(&g, 4, islandPos, minIslandSizeBlocks, maxIslandSizeBlocks))
            {
                std::cout << seed << " village island at " << islandPos.x << "x" << islandPos.z << std::endl;
                foundIslandVillage = true;
                break;
            }
        }

        if (!foundIslandVillage)
        {
            continue;
        }

        std::cout << seed << " mushroom stronghold at " << strongholdPos.x << "x" << strongholdPos.z << std::endl;

#if 0

        const int rangeToSearch = 80;    
        int biomesToFind[] = {lush_caves};
        BiomeFilter biomeFilter;
        setupBiomeFilter(&biomeFilter, version, 0, biomesToFind, sizeof(biomesToFind)/sizeof(int), nullptr, 0, nullptr, 0);
        int edgeLen = rangeToSearch * 2;

        Range r;

        // 1:16, a.k.a. horizontal chunk scaling
        r.scale = 1;
        // Define the position and size for a horizontal area:
        r.x = strongholdPos.x - rangeToSearch, r.z = strongholdPos.z - rangeToSearch;   // position (x,z)
        r.sx = edgeLen, r.sz = edgeLen; // size (width,height)
        // Set the vertical range as a plane near sea level at scale 1:4.
        r.y = 15, r.sy = 1;

        //TODO change to SIGTERM handler volital
        volatile char stop = 0;

        int *cache = allocCache(&g, r);
        bool hasBiomes = static_cast<bool>(checkForBiomes(&g, cache, r, DIM_OVERWORLD, seed, &biomeFilter, &stop));
        free(cache);

        if(!hasBiomes)
        {
            continue;
        }
#endif

        std::cout << "found ideal seed: " << seed << std::endl;
        numFinds++;
        std::cout << "mean time between finds = " << std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - startTime).count() / numFinds << std::endl;
    }
}

int main()
{
    static constexpr int num_threads = 8;
    std::array<std::thread, num_threads> threads;

    // First initialize the global biome table 'int biomes[256]'. This sets up
    // properties such as the category and temperature of each biome.
    initBiomes();

    for(auto& t : threads)
    {
        t = std::thread{search};
    }

    for(auto& t : threads)
    {
        t.join();
    }
    
    return 0;
}

