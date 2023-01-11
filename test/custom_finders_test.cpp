#include "gtest/gtest.h"

#include "custom_finders.h"

namespace {

TEST(CustomFinders, IsIslandSpawn_MC_1_19)
{
    const auto version = MC_1_19;
    int64_t seed = -3874624793136709106;
    Generator g;
    setupGenerator(&g, version, 0);
    applySeed(&g, DIM_OVERWORLD, seed);
    Pos spawnPos = getSpawn(&g);
    EXPECT_TRUE(isIsland(&g, 4, spawnPos, 10, 200000));
}

} // namespace
