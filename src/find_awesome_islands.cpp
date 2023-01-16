#include <array>
#include <atomic>
#include <chrono>
#include <cstdint>
#include <deque>
#include <iostream>
#include <map>
#include <random>
#include <thread>
#include <utility>

import Cubiomes;
import CustomFinders;

/*
 * This program outputs awesome island seeds
 * An awesome island seed is a seed in which
 * spawn is located on an island, that island is near a mushroom biome
 * and there is at least one village island nearby
 */

std::atomic<uint64_t> num_finds{0};
const auto startTime = std::chrono::steady_clock::now();

void search() {
    const int64_t min_island_size_blocks = 50000;
    const int64_t max_island_size_blocks = 500000;

    std::random_device rd;
    std::mt19937_64 eng(rd());
    std::uniform_int_distribution<unsigned long long> distr;

    while (true) {
        // int64_t seed = -3874624793136709106;
        int64_t seed = distr(eng);

        cubiomes::generator g{cubiomes::mc_version::MC_NEWEST, seed};

        // std::cout << "checking " << seed << std::endl;

        cubiomes::pos spawn_pos = cubiomes::get_spawn(g);

        auto spawn_biome =
            cubiomes::get_biome(g, 1, spawn_pos.x, 63, spawn_pos.z);

        if (!(spawn_biome == cubiomes::biome::snowy_slopes ||
              spawn_biome == cubiomes::biome::jagged_peaks ||
              spawn_biome == cubiomes::biome::frozen_peaks ||
              spawn_biome == cubiomes::biome::stony_peaks ||
              spawn_biome == cubiomes::biome::mountain_edge ||
              spawn_biome == cubiomes::biome::mountains ||
              spawn_biome == cubiomes::biome::mushroom_fields ||
              spawn_biome == cubiomes::biome::mushroom_field_shore)) {
            continue;
        }

        if (!custom_finders::is_island(
                g, 4, spawn_pos, min_island_size_blocks, max_island_size_blocks,
                std::vector<cubiomes::biome>{
                    cubiomes::biome::ocean, cubiomes::biome::deep_ocean,
                    cubiomes::biome::warm_ocean,
                    cubiomes::biome::lukewarm_ocean,
                    cubiomes::biome::deep_warm_ocean,
                    cubiomes::biome::deep_lukewarm_ocean})) {
            continue;
        }

        auto found_villages = cubiomes::get_nearby_villages(g, spawn_pos, 500);

        if (found_villages.empty()) {
            continue;
        }

        bool found_island_village = false;

        for (auto& island_pos : found_villages) {
            if (custom_finders::is_island(g, 4, island_pos,
                                          min_island_size_blocks,
                                          max_island_size_blocks)) {
                // std::cout << seed << " village island at " << island_pos.x <<
                // "x" << island_pos.z << std::endl;
                found_island_village = true;
                break;
            }
        }

        if (!found_island_village) {
            continue;
        }

        std::cout << "found island seed " << seed << std::endl;

        bool found_mushroom_stronghold = false;
        cubiomes::pos stronghold_pos;
        auto strongholds = cubiomes::get_first_n_strongholds(g, 3);

        for (auto pos : strongholds) {
            auto surface = cubiomes::get_biome(g, 1, pos.x, 63, pos.z);
            auto deep = cubiomes::get_biome(g, 1, pos.x, -56, pos.z);

            if (deep == cubiomes::biome::lush_caves &&
                (surface == cubiomes::biome::mushroom_fields ||
                 surface == cubiomes::biome::mushroom_field_shore)) {
                found_mushroom_stronghold = true;
                stronghold_pos = pos;
                break;
            }
        }

        if (!found_mushroom_stronghold) {
            continue;
        }

        std::cout << "found stronghold seed: " << seed << std::endl;

        std::cout << seed << " mushroom stronghold at " << stronghold_pos.x
                  << "x" << stronghold_pos.z << std::endl;

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
        r.x = stronghold_pos.x - rangeToSearch, r.z = stronghold_pos.z - rangeToSearch;   // position (x,z)
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
        num_finds++;
        std::cout << "mean time between finds = "
                  << std::chrono::duration_cast<std::chrono::seconds>(
                         std::chrono::steady_clock::now() - startTime)
                             .count() /
                         num_finds
                  << std::endl;
    }
}

int main() {
    static constexpr int num_threads = 8;
    std::array<std::thread, num_threads> threads;

    for (auto& t : threads) {
        t = std::thread{search};
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}

