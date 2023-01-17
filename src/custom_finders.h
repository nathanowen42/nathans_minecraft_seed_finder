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

import Cubiomes;

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
                                cubiomes::pos pos);

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

bool is_island(
    const cubiomes::generator& g, int resolution, cubiomes::pos pos,
    uint32_t minDistFromLand, uint32_t minBlocks, uint32_t maxBlocks,
    std::optional<std::vector<cubiomes::biome>> oceanFilter = std::nullopt);

}  // namespace custom_finders
