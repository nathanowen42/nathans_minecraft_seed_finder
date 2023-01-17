#include "custom_finders.h"

#include <gtest/gtest.h>

#include <iostream>
#include <vector>

namespace {

static constexpr int island_find_resolution = 16;
static constexpr int min_island_distance_to_land = 100;
static constexpr int min_island_size = 10;
static constexpr int max_island_size = 300000;

TEST(CustomFinders, IsIslandSpawn_MC_1_19) {
    std::vector<int64_t> true_positives{
        -3874624793136709106, -6299538536873765642, -4599710822397431702};

    for (auto seed : true_positives) {
        cubiomes::generator g{cubiomes::mc_version::MC_1_19, seed};
        auto spawn_pos = cubiomes::get_spawn(g);
        EXPECT_TRUE(custom_finders::is_island(
            g, island_find_resolution, spawn_pos, min_island_distance_to_land,
            min_island_size, max_island_size))
            << "seed " << seed;
    }
}

TEST(CustomFinders, IsIslandSpawn_MC_1_19_DetectsFalsePositive) {
    std::vector<int64_t> false_positives{
        2814193133105272675, 5368089814708928674, -3138533370790577155,
        1328233709022733016, -6559902457696112996};
    for (auto seed : false_positives) {
        cubiomes::generator g{cubiomes::mc_version::MC_1_19, seed};
        auto spawn_pos = cubiomes::get_spawn(g);
        EXPECT_FALSE(custom_finders::is_island(
            g, island_find_resolution, spawn_pos, min_island_distance_to_land,
            min_island_size, max_island_size))
            << "seed " << seed;
    }
}

TEST(CustomFinders, IsIslandSpawn_MC_1_19_GreyArea) {
    std::vector<int64_t> false_positives{8338430434554161236};

    for (auto seed : false_positives) {
        cubiomes::generator g{cubiomes::mc_version::MC_1_19, seed};
        auto spawn_pos = cubiomes::get_spawn(g);
        EXPECT_FALSE(custom_finders::is_island(
            g, island_find_resolution, spawn_pos, min_island_distance_to_land,
            min_island_size, max_island_size))
            << "seed " << seed;
    }
}

}  // namespace
