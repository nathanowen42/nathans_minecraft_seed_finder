#include "custom_finders.h"

#include <gtest/gtest.h>

#include <iostream>
#include <vector>

namespace {

TEST(CustomFinders, IsIslandSpawn_MC_1_19) {
    std::vector<int64_t> truePositives{
        -3874624793136709106, -6299538536873765642, -4599710822397431702};

    for (auto seed : truePositives) {
        cubiomes::generator g{cubiomes::mc_version::MC_1_19, seed};
        auto spawnPos = cubiomes::get_spawn(g);
        EXPECT_TRUE(custom_finders::is_island(g, 4, spawnPos, 10, 200000))
            << "seed " << seed;
    }
}

TEST(CustomFinders, IsIslandSpawn_MC_1_19_DetectsFalsePositive) {
    std::vector<int64_t> falsePositives{
        2814193133105272675, 5368089814708928674, -3138533370790577155,
        1328233709022733016, -6559902457696112996};
    for (auto seed : falsePositives) {
        cubiomes::generator g{cubiomes::mc_version::MC_1_19, seed};
        auto spawnPos = cubiomes::get_spawn(g);
        EXPECT_FALSE(custom_finders::is_island(g, 4, spawnPos, 10, 200000))
            << "seed " << seed;
    }
}

TEST(CustomFinders, IsIslandSpawn_MC_1_19_GreyArea) {
    std::vector<int64_t> falsePositives{8338430434554161236,
                                        3522430528685162186};

    for (auto seed : falsePositives) {
        cubiomes::generator g{cubiomes::mc_version::MC_1_19, seed};
        auto spawnPos = cubiomes::get_spawn(g);
        EXPECT_FALSE(custom_finders::is_island(g, 4, spawnPos, 10, 200000))
            << "seed " << seed;
    }
}

}  // namespace
