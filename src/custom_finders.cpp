#include "custom_finders.h"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <list>
#include <map>
#include <optional>
#include <vector>

namespace custom_finders {

static constexpr int sea_level = 63;

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
cubiomes::pos find_nearest_land(const cubiomes::generator& g,
                                cubiomes::pos pos) {
    std::map<std::pair<int, int>, bool> visited{};
    std::list<cubiomes::pos> queue;

    queue.emplace_back(pos);

    while (true) {
        auto p = queue.front();
        queue.pop_front();

        auto& vis = visited[{p.x, p.z}];

        if (vis) {
            continue;
        }

        vis = true;

        if (!cubiomes::is_oceanic(
                cubiomes::get_biome(g, 1, p.x, sea_level, p.z))) {
            return p;
        }

        queue.emplace_back(cubiomes::pos{p.x + 1, p.z});
        queue.emplace_back(cubiomes::pos{p.x - 1, p.z});
        queue.emplace_back(cubiomes::pos{p.x, p.z + 1});
        queue.emplace_back(cubiomes::pos{p.x, p.z - 1});
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

bool is_island(const cubiomes::generator& g, int resolution, cubiomes::pos pos,
               uint32_t minDistFromLand, uint32_t minBlocks, uint32_t maxBlocks,
               std::optional<std::vector<cubiomes::biome>> oceanFilter) {
    pos.x /= resolution;
    pos.z /= resolution;
    minBlocks /= resolution * resolution;
    maxBlocks /= resolution * resolution;
    minDistFromLand /= resolution;
    auto requiredOceanBlocks = maxBlocks * maxBlocks;

    if (cubiomes::is_oceanic(
            cubiomes::get_biome(g, resolution, pos.x, sea_level, pos.z))) {
        return false;
    }

    std::map<std::pair<int, int>, bool> visited{};
    std::map<std::pair<int, int>, cubiomes::biome> biomeCache{};
    std::list<cubiomes::pos> queue;
    uint32_t oceanDist = 0;

    auto getBiome = [&](int x, int z) {
        auto [it, added] = biomeCache.try_emplace({x, z});

        if (added) {
            auto biome = cubiomes::get_biome(g, resolution, x, sea_level, z);
            it->second = biome;
            return biome;
        } else {
            return it->second;
        }
    };

    queue.emplace_back(pos);
    visited[{pos.x, pos.z}] = true;

    while (oceanDist < minDistFromLand) {
        uint64_t oceanBlocksChecked = 0;
        uint32_t blocksChecked = 0;
        std::list<cubiomes::pos> oceanEdge;
        bool foundLand = false;

        auto enqueuePos = [&](int x, int z) {
            auto& vis = visited[{x, z}];

            if (vis) {
                return;
            }

            vis = true;

            auto biome = getBiome(x, z);

            if (cubiomes::is_oceanic(biome)) {
                oceanEdge.emplace_back(cubiomes::pos{x, z});
            } else {
                queue.emplace_back(cubiomes::pos{x, z});
                foundLand = true;
            }
        };

        while (!queue.empty() && blocksChecked < maxBlocks) {
            auto p = queue.front();
            queue.pop_front();
            auto biome = getBiome(p.x, p.z);

            if (cubiomes::is_oceanic(biome)) {
                if (oceanFilter.has_value()) {
                    auto& list = oceanFilter.value();
                    if (std::find(list.begin(), list.end(), biome) ==
                        list.end()) {
                        return false;
                    }
                }
            } else {
                blocksChecked++;
            }

            enqueuePos(p.x + 1, p.z);
            enqueuePos(p.x - 1, p.z);
            enqueuePos(p.x, p.z + 1);
            enqueuePos(p.x, p.z - 1);
        }

        if (blocksChecked >= maxBlocks) {
            return false;
        }

        queue.splice(queue.end(), oceanEdge);

        if (foundLand) {
            oceanDist = 1;
        } else {
            oceanDist++;
        }
    }

    return oceanDist == minDistFromLand;
}

}  // namespace custom_finders
