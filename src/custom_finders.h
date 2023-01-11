#include "finders.h"

/*
 * Finds the closest land to the specified location
 * using a BFS.
 *
 * Note this is extremely expensive and should be done
 * later during seed finding 
 */
Pos findClosestLand(const Generator *g, Pos pos);

/*
 * When given a position on land, this function will use a BFS
 * to determine if the land is an island (e.g. a landmass surrounded by ocean)
 * with a surface area within the provided range of blocks.
 */

bool isIsland(const Generator *g, int resolution, Pos pos, uint64_t minBlocks, uint64_t maxBlocks);

/*
 * Find all villages within a certin range (in blocks) from a position
 */
std::vector<Pos> getVillagesInRange(Generator *g, Pos pos, int64_t seed, uint64_t range);

