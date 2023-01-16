module;

#include <list>

#include "finders.h"

export module Cubiomes;

namespace cubiomes {

export enum class biome : int {
    none = -1,
    // 0
    ocean = 0,
    plains,
    desert,
    mountains,
    extremeHills = mountains,
    forest,
    taiga,
    swamp,
    swampland = swamp,
    river,
    nether_wastes,
    hell = nether_wastes,
    the_end,
    sky = the_end,
    // 10
    frozen_ocean,
    frozenOcean = frozen_ocean,
    frozen_river,
    frozenRiver = frozen_river,
    snowy_tundra,
    icePlains = snowy_tundra,
    snowy_mountains,
    iceMountains = snowy_mountains,
    mushroom_fields,
    mushroomIsland = mushroom_fields,
    mushroom_field_shore,
    mushroomIslandShore = mushroom_field_shore,
    beach,
    desert_hills,
    desertHills = desert_hills,
    wooded_hills,
    forestHills = wooded_hills,
    taiga_hills,
    taigaHills = taiga_hills,
    // 20
    mountain_edge,
    extremeHillsEdge = mountain_edge,
    jungle,
    jungle_hills,
    jungleHills = jungle_hills,
    jungle_edge,
    jungleEdge = jungle_edge,
    deep_ocean,
    deepOcean = deep_ocean,
    stone_shore,
    stoneBeach = stone_shore,
    snowy_beach,
    coldBeach = snowy_beach,
    birch_forest,
    birchForest = birch_forest,
    birch_forest_hills,
    birchForestHills = birch_forest_hills,
    dark_forest,
    roofedForest = dark_forest,
    // 30
    snowy_taiga,
    coldTaiga = snowy_taiga,
    snowy_taiga_hills,
    coldTaigaHills = snowy_taiga_hills,
    giant_tree_taiga,
    megaTaiga = giant_tree_taiga,
    giant_tree_taiga_hills,
    megaTaigaHills = giant_tree_taiga_hills,
    wooded_mountains,
    extremeHillsPlus = wooded_mountains,
    savanna,
    savanna_plateau,
    savannaPlateau = savanna_plateau,
    badlands,
    mesa = badlands,
    wooded_badlands_plateau,
    mesaPlateau_F = wooded_badlands_plateau,
    badlands_plateau,
    mesaPlateau = badlands_plateau,
    // 40  --  1.13
    small_end_islands,
    end_midlands,
    end_highlands,
    end_barrens,
    warm_ocean,
    warmOcean = warm_ocean,
    lukewarm_ocean,
    lukewarmOcean = lukewarm_ocean,
    cold_ocean,
    coldOcean = cold_ocean,
    deep_warm_ocean,
    warmDeepOcean = deep_warm_ocean,
    deep_lukewarm_ocean,
    lukewarmDeepOcean = deep_lukewarm_ocean,
    deep_cold_ocean,
    coldDeepOcean = deep_cold_ocean,
    // 50
    deep_frozen_ocean,
    frozenDeepOcean = deep_frozen_ocean,
    BIOME_NUM,

    the_void = 127,

    // mutated variants
    sunflower_plains = plains + 128,
    desert_lakes = desert + 128,
    gravelly_mountains = mountains + 128,
    flower_forest = forest + 128,
    taiga_mountains = taiga + 128,
    swamp_hills = swamp + 128,
    ice_spikes = snowy_tundra + 128,
    modified_jungle = jungle + 128,
    modified_jungle_edge = jungle_edge + 128,
    tall_birch_forest = birch_forest + 128,
    tall_birch_hills = birch_forest_hills + 128,
    dark_forest_hills = dark_forest + 128,
    snowy_taiga_mountains = snowy_taiga + 128,
    giant_spruce_taiga = giant_tree_taiga + 128,
    giant_spruce_taiga_hills = giant_tree_taiga_hills + 128,
    modified_gravelly_mountains = wooded_mountains + 128,
    shattered_savanna = savanna + 128,
    shattered_savanna_plateau = savanna_plateau + 128,
    eroded_badlands = badlands + 128,
    modified_wooded_badlands_plateau = wooded_badlands_plateau + 128,
    modified_badlands_plateau = badlands_plateau + 128,
    // 1.14
    bamboo_jungle = 168,
    bamboo_jungle_hills = 169,
    // 1.16
    soul_sand_valley = 170,
    crimson_forest = 171,
    warped_forest = 172,
    basalt_deltas = 173,
    // 1.17
    dripstone_caves = 174,
    lush_caves = 175,
    // 1.18
    meadow = 177,
    grove = 178,
    snowy_slopes = 179,
    jagged_peaks = 180,
    frozen_peaks = 181,
    stony_peaks = 182,
    old_growth_birch_forest = tall_birch_forest,
    old_growth_pine_taiga = giant_tree_taiga,
    old_growth_spruce_taiga = giant_spruce_taiga,
    snowy_plains = snowy_tundra,
    sparse_jungle = jungle_edge,
    stony_shore = stone_shore,
    windswept_hills = mountains,
    windswept_forest = wooded_mountains,
    windswept_gravelly_hills = gravelly_mountains,
    windswept_savanna = shattered_savanna,
    wooded_badlands = wooded_badlands_plateau,
    // 1.19
    deep_dark = 183,
    mangrove_swamp = 184,
};

export enum class mc_version : int {
    MC_1_0,  // <=1.0 Experimental!
    MC_1_1,
    MC_1_2,
    MC_1_3,
    MC_1_4,
    MC_1_5,
    MC_1_6,
    MC_1_7,
    MC_1_8,
    MC_1_9,
    MC_1_10,
    MC_1_11,
    MC_1_12,
    MC_1_13,
    MC_1_14,
    MC_1_15,
    MC_1_16,
    MC_1_17,
    MC_1_18,
    MC_1_19,
    MC_NEWEST = MC_1_19,
};

export bool is_oceanic(biome b) { return isOceanic(static_cast<int>(b)); }

export using pos = Pos;

bool InitCubiomes() {
    initBiomes();
    return true;
}

export class generator {
   public:
    generator(const generator&) = default;
    generator(generator&&) = default;
    generator& operator=(const generator&) = default;
    generator& operator=(generator&&) = default;
    virtual ~generator() = default;
    generator(mc_version version_, int64_t seed_)
        : version{version_}, seed{seed_} {
        static bool init = InitCubiomes();
        setupGenerator(&g, static_cast<int>(version), 0);
        applySeed(&g, DIM_OVERWORLD, seed);
    }

   private:
    friend pos get_spawn(const generator&);
    friend biome get_biome(const generator&, int, int, int, int);
    friend std::list<cubiomes::pos> get_nearby_villages(generator&, pos,
                                                        uint64_t);
    friend std::list<pos> get_first_n_strongholds(generator& g, size_t n);
    const int64_t seed;
    const mc_version version;
    Generator g;
};

export pos get_spawn(const generator& g) { return getSpawn(&g.g); }

export biome get_biome(const generator& g, int scale, int x, int y, int z) {
    return static_cast<biome>(getBiomeAt(&g.g, scale, x, y, z));
}

export std::list<cubiomes::pos> get_nearby_villages(generator& g,
                                                    cubiomes::pos pos,
                                                    uint64_t range) {
    std::list<cubiomes::pos> foundList;
    StructureConfig sc;
    getStructureConfig(Village, g.g.mc, &sc);
    auto regionSizeBlocks = sc.regionSize * 16;
    int64_t regionRange = std::max(range / regionSizeBlocks, uint64_t(1));
    auto regionPosX = pos.x / regionSizeBlocks;
    auto regionPosZ = pos.z / regionSizeBlocks;

    for (int regionX = regionPosX - regionRange; regionX <= regionRange;
         regionX++) {
        for (int regionZ = regionPosZ - regionRange; regionZ <= regionRange;
             regionZ++) {
            cubiomes::pos structPos;

            if (!getStructurePos(Village, g.g.mc, g.seed, regionX, regionZ,
                                 &structPos)) {
                continue;
            }

            if (isViableStructurePos(Village, &g.g, structPos.x, structPos.z,
                                     0)) {
                foundList.emplace_back(structPos);
            }
        }
    }

    // free(cache);
    return foundList;
}

export std::list<pos> get_first_n_strongholds(generator& g, size_t n) {
    StrongholdIter sh;
    bool found_stronghold;
    initFirstStronghold(&sh, static_cast<int>(g.version), g.seed);
    std::list<pos> ret;
    size_t i = 0;

    while (found_stronghold && i < n) {
        ret.emplace_back(sh.pos);
        found_stronghold = nextStronghold(&sh, &g.g);
        i++;
    }

    return ret;
}

}  // namespace cubiomes
