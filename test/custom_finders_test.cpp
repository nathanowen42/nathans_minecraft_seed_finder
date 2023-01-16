#include <gtest/gtest.h>

#include <iostream>
#include <vector>

import custom_finders;

namespace {

TEST(CustomFinders, IsIslandSpawn_MC_1_19) {
    std::vector<int64_t> truePositives{
        -3874624793136709106, -6299538536873765642, -4599710822397431702};

    for (auto seed : truePositives) {
        const auto version = MC_1_19;
        Generator g;
        setupGenerator(&g, version, 0);
        applySeed(&g, DIM_OVERWORLD, seed);
        Pos spawnPos = getSpawn(&g);
        EXPECT_TRUE(isIsland(&g, 4, spawnPos, 10, 200000)) << "seed " << seed;
    }
}

TEST(CustomFinders, IsIslandSpawn_MC_1_19_DetectsFalsePositive) {
    std::vector<int64_t> falsePositives{
        2814193133105272675, 5368089814708928674, -3138533370790577155,
        1328233709022733016, -6559902457696112996};
    for (auto seed : falsePositives) {
        const auto version = MC_1_19;
        Generator g;
        setupGenerator(&g, version, 0);
        applySeed(&g, DIM_OVERWORLD, seed);
        Pos spawnPos = getSpawn(&g);
        EXPECT_FALSE(isIsland(&g, 4, spawnPos, 10, 200000)) << "seed " << seed;
    }
}

TEST(CustomFinders, IsIslandSpawn_MC_1_19_GreyArea) {
    const auto version = MC_1_19;
    std::vector<int64_t> falsePositives{8338430434554161236,
                                        3522430528685162186};

    for (auto seed : falsePositives) {
        Generator g;
        setupGenerator(&g, version, 0);
        applySeed(&g, DIM_OVERWORLD, seed);
        Pos spawnPos = getSpawn(&g);
        EXPECT_FALSE(isIsland(&g, 4, spawnPos, 10, 200000)) << "seed " << seed;
    }
}

}  // namespace
