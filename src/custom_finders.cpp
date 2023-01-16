#include "custom_finders.h"

#include <algorithm>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <cstdint>
#include <iostream>
#include <list>
#include <map>
#include <optional>
#include <vector>

namespace custom_finders {

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

        if (!cubiomes::is_oceanic(cubiomes::get_biome(g, 1, p.x, 0, p.z))) {
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
               uint64_t minBlocks, uint64_t maxBlocks,
               std::optional<std::vector<cubiomes::biome>> oceanFilter) {
    pos.x /= resolution;
    pos.z /= resolution;
    minBlocks /= resolution * resolution;
    maxBlocks /= resolution * resolution;
    auto requiredOceanBlocks = maxBlocks * maxBlocks;

    if (cubiomes::is_oceanic(
            cubiomes::get_biome(g, resolution, pos.x, 0, pos.z))) {
        return false;
    }

    std::map<std::pair<int, int>, bool> visited{};
    std::list<cubiomes::pos> landQueue;
    std::list<cubiomes::pos> queue;
    uint64_t blocksChecked = 0;

    landQueue.emplace_back(pos);

    while (blocksChecked < maxBlocks && !landQueue.empty()) {
        queue.splice(queue.end(), landQueue);
        uint64_t oceanBlocksChecked = 0;
        std::list<cubiomes::pos> oceanEdge;

        while (!queue.empty() && blocksChecked < maxBlocks) {
            auto p = queue.front();
            queue.pop_front();

            auto& vis = visited[{p.x, p.z}];

            if (vis) {
                continue;
            }

            vis = true;

            auto biome = cubiomes::get_biome(g, resolution, p.x, 0, p.z);

            if (cubiomes::is_oceanic(biome)) {
                if (oceanFilter.has_value()) {
                    auto& list = oceanFilter.value();
                    if (std::find(list.begin(), list.end(), biome) ==
                        list.end()) {
                        return false;
                    }
                }
                oceanEdge.emplace_back(p.x, p.z);
                continue;
            }

            blocksChecked++;

            queue.emplace_back(cubiomes::pos{p.x + 1, p.z});
            queue.emplace_back(cubiomes::pos{p.x - 1, p.z});
            queue.emplace_back(cubiomes::pos{p.x, p.z + 1});
            queue.emplace_back(cubiomes::pos{p.x, p.z - 1});
        }

        if (blocksChecked <= minBlocks || blocksChecked >= maxBlocks) {
            std::cout << "too many non-ocean blocks" << std::endl;
            return false;
        }

        queue.splice(queue.end(), oceanEdge);

        while (!queue.empty() && oceanBlocksChecked < requiredOceanBlocks) {
            auto p = queue.front();
            queue.pop_front();

            auto& vis = visited[{p.x, p.z}];

            if (vis) {
                continue;
            }

            vis = true;
            auto biome = cubiomes::get_biome(g, resolution, p.x, 0, p.z);

            if (cubiomes::is_oceanic(biome)) {
                if (oceanFilter.has_value()) {
                    auto& list = oceanFilter.value();
                    if (std::find(list.begin(), list.end(), biome) ==
                        list.end()) {
                        return false;
                    }
                }

                queue.emplace_back(cubiomes::pos{p.x + 1, p.z});
                queue.emplace_back(cubiomes::pos{p.x - 1, p.z});
                queue.emplace_back(cubiomes::pos{p.x, p.z + 1});
                queue.emplace_back(cubiomes::pos{p.x, p.z - 1});
                oceanBlocksChecked++;

                continue;
            } else {
                landQueue.emplace_back(cubiomes::pos{p.x, p.z});
            }

            blocksChecked++;
        }
    }

    return true;
}

}  // namespace custom_finders
