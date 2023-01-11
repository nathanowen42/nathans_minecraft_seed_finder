#include <algorithm>
#include <deque>
#include <vector>
#include <map>

#include "finders.h"


/*
 * Finds the closest land to the specified location
 * using a BFS.
 *
 * TODO optimize this algorithm by doing this at the
 * correct layer instead of a block-by-block search
 * I am not sure yet what layer this needs to be done at.
 *
 * Note this is extremely expensive and should be done
 * later during seed finding 
 */
Pos findClosestLand(const Generator *g, Pos pos)
{
    std::map<std::pair<int,int>, bool> visited{};
    std::deque<Pos> queue;

    queue.emplace_back(pos);

    while (true)
    {
        auto p = queue.front();
        queue.pop_front();

        auto& vis = visited[{p.x,p.z}];

        if(vis)
        {
            continue;
        }

        vis = true;

        if(!isOceanic(getBiomeAt(g, 1, p.x, 0, p.z)))
        {
            return p;
        }

        queue.emplace_back(Pos{p.x + 1, p.z});
        queue.emplace_back(Pos{p.x - 1, p.z});
        queue.emplace_back(Pos{p.x, p.z + 1});
        queue.emplace_back(Pos{p.x, p.z - 1});
    }
}

/*
 * When given a position on land, this function will use a BFS
 * to determine if the land is an island (e.g. a landmass surrounded by ocean)
 * with a surface area within the provided range of blocks.
 *
 * TODO optimize this algorithm by doing this at the
 * correct layer instead of a block-by-block search
 * I am not sure yet what layer this needs to be done at.
 *
 * Note this is extremely expensive and should be done
 * later during seed finding 
 */

bool isIsland(const Generator *g, int resolution, Pos pos, uint64_t minBlocks, uint64_t maxBlocks)
{
    pos.x /= resolution;
    pos.z /= resolution;
    minBlocks /= resolution;
    minBlocks /= resolution;
    maxBlocks /= resolution;
    maxBlocks /= resolution;

    if(isOceanic(getBiomeAt(g, resolution, pos.x, 0, pos.z)))
    {
        return false;
    }

    std::map<std::pair<int,int>, bool> visited{};
    std::deque<Pos> queue;
    uint64_t blocksChecked = 0;

    queue.emplace_back(pos);

    while (!queue.empty() && blocksChecked < maxBlocks)
    {
        auto p = queue.front();
        queue.pop_front();

        auto& vis = visited[{p.x,p.z}];

        if(vis)
        {
            continue;
        }

        vis = true;

        if(isOceanic(getBiomeAt(g, resolution, p.x, 0, p.z)))
        {
            continue;
        }

        blocksChecked++;

        queue.emplace_back(Pos{p.x + 1, p.z});
        queue.emplace_back(Pos{p.x - 1, p.z});
        queue.emplace_back(Pos{p.x, p.z + 1});
        queue.emplace_back(Pos{p.x, p.z - 1});
    }

    return blocksChecked > minBlocks && blocksChecked < maxBlocks;
}

std::vector<Pos> getVillagesInRange(Generator *g, Pos pos, int64_t seed, uint64_t range)
{
    std::vector<Pos> foundList;
    StructureConfig sc;
    getStructureConfig(Village, g->mc, &sc);
    auto regionSizeBlocks = sc.regionSize * 16;
    int64_t regionRange = std::max(range / regionSizeBlocks, uint64_t(1));
    auto regionPosX = pos.x / regionSizeBlocks;
    auto regionPosZ = pos.z / regionSizeBlocks;

    for(int regionX = regionPosX - regionRange; regionX <= regionRange; regionX++)
    {
        for(int regionZ = regionPosZ - regionRange; regionZ <= regionRange; regionZ++)
        {
            Pos structPos;
            
            if(!getStructurePos(Village, g->mc, seed, regionX, regionZ, &structPos))
            {
                continue;
            }

            if(isViableStructurePos(Village, g, structPos.x, structPos.z, 0))
            {
                foundList.emplace_back(structPos);
            }
        }
    }

    //free(cache);
    return foundList;
}

