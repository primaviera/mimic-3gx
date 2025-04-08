#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "config.hpp"
#include "func_ptrs.hpp"
#include "hacks/hacks.hpp"
#include "logger.hpp"
#include "patterns.hpp"

#include "mimic_types.hpp"

namespace CTRPluginFramework {

namespace hacks {

    extern "C" const char* bgm_array[];
    extern "C" const char* bg_array[];
    extern "C" const char* enemy_array[];
    extern "C" const char* enemy_summon_array[];
    extern "C" uint32_t stage_bgm;
    extern "C" int get_rand_int(int min, int max)
    {
        // TODO: Use sead::Random::getU32
        return Utils::Random(min, max);
    }

    /* Randomize battle BGM */
    const char* randomize_battle_bgm()
    {
        return bgm_array[get_rand_int(0, 78)];
    }

    /* Randomize Darker Lord BGM (Second Phase) */
    void NAKED randomize_trans_bgm()
    {
        asm("push {r0, r2, lr} \n"
            "mov r0, #0 \n"
            "mov r1, #78 \n"
            "bl get_rand_int \n"
            "ldr r2, =bgm_array \n"
            "ldr r1, [r2, r0, lsl #2] \n"
            "pop {r0, r2, pc}");
    }

    /* Randomize battle background */
    void NAKED randomize_battle_bg()
    {
        asm("push {r0-r2, lr} \n"
            "mov r0, #0 \n"
            "mov r1, #41 \n"
            "bl get_rand_int \n"
            "ldr r1, =bg_array \n"
            "ldr r0, [r1, r0, lsl #2] \n"
            "str r0, [r9, #4] \n"
            "pop {r0-r2, pc}");
    }

    /* Randomize battle intro jingle */
    void randomize_battle_intro(uintptr_t r0, uintptr_t r1, int* index)
    {
        *index = Utils::Random(0, 2);
        HookContext::GetCurrent().OriginalFunction<void>(r0, r1, index);
    }

    /*
     * Randomize stage BGM
     * The BGM is only randomized at the start
     * of a level (when r7 is 0)
     */
    uint32_t stage_bgm = 0;
    void NAKED randomize_stage_bgm()
    {
        asm("push {r4, lr} \n"
            "ldr r4, =stage_bgm \n"
            "ldr r0, [r4] \n"
            "cmp r7, #0 \n"
            "moveq r0, #0 \n"
            "moveq r1, #78 \n"
            "bleq get_rand_int \n"
            "str r0, [r4] \n"
            "ldr r1, =bgm_array \n"
            "ldr r0, [r1, r0, lsl #2] \n"
            "pop {r4, pc}");
    }

    /* Randomize stage BG */
    uintptr_t randomize_stage_bg(uintptr_t r0)
    {
        // This currently crashes
        if (!r0)
            return r0;
        *(char**)(r0 + 0xC) = (char*)"BG/MapS/MapSCave00"; // For testing purposes
        return r0;
    }

    /* Randomize title BGM */
    void NAKED randomize_title_bgm()
    {
        // Not sure what all of the IDs mean
        // New Lumos is 27, probably the max
        asm("push {r0, r2, lr} \n"
            "mov r0, #0 \n"
            "mov r1, #27 \n"
            "bl get_rand_int \n"
            "mov r1, r0 \n"
            "pop {r0, r2, pc}");
    }

    /* Randomize title BG */
    void NAKED randomize_title_bg()
    {
        asm("push {r0, lr} \n"
            "mov r0, #0 \n"
            "mov r1, #27 \n"
            "bl get_rand_int \n"
            "mov r1, r0 \n"
            "pop {r0} \n"
            "strb r1, [r0, #0x1d] \n"
            "pop {pc}");
    }

    /* Randomize map BGM */
    void NAKED randomize_map_bgm()
    {
        asm("push {r1-r2, lr} \n"
            "mov r0, #0 \n"
            "mov r1, #78 \n"
            "bl get_rand_int \n"
            "ldr r1, =bgm_array \n"
            "ldr r0, [r1, r0, lsl #2] \n"
            "pop {r1-r2, pc}");
    }

    /* Randomize town BGM */
    void NAKED randomize_town_bgm()
    {
        asm("push {r1-r2, lr} \n"
            "mov r0, #0 \n"
            "mov r1, #78 \n"
            "bl get_rand_int \n"
            "ldr r1, =bgm_array \n"
            "ldr r0, [r1, r0, lsl #2] \n"
            "str r0, [r5, #4] \n"
            "pop {r1-r2, pc}");
    }

    /*
     * Randomize enemies
     * Sometimes crashes, not sure why
     */
    uintptr_t randomize_enemy(uintptr_t r0)
    {
        logger::clear();
        logger::write("[ RANDOMIZED ENEMIES ]\n");

        int max_num = Utils::Random(1, 4);
        for (int i = 0; i < 8; i++) {
            uintptr_t enemy_data = (uintptr_t)(r0 + i * 4);
            if (i >= max_num) {
                *(uint32_t*)(enemy_data + 0x74) = 0;
                continue;
            }
            uint32_t rand = Utils::Random(0, 298);
            const char* enemy_name = enemy_array[rand];
            *(uint32_t*)(enemy_data + 0x74) = sead_HashCRC32_calcHash(enemy_name, strlen(enemy_name));

            logger::write(Utils::Format("%s (%d)\n", enemy_name, rand));
        }
        return r0;
    }

    /* Handle enemy stats */
    enemy_param* handle_enemy_stats(enemy_param* enemy)
    {
        if (!(uintptr_t)enemy)
            return enemy;

        // For testing purposes
        enemy->status->hp = 1;
        enemy->status->atk = 0;
        enemy->status->def = 0;
        enemy->status->mag = 0;
        enemy->status->spd = 0;
        return enemy;
    }

    /* Randomize enemy slot 1 moves */
    void NAKED randomize_enemy_skills_1()
    {
        asm("push {r0, lr} \n"
            "mov r0, #0 \n"
            "mov r1, #29 \n"
            "bl get_rand_int \n"
            "mov r1, r0 \n"
            "pop {r0, pc}");
    }

    /* Randomize enemy slot 2 moves */
    void NAKED randomize_enemy_skills_2()
    {
        asm("push {r1, lr} \n"
            "mov r0, #0 \n"
            "mov r1, #25 \n"
            "bl get_rand_int \n"
            "pop {r1, pc}");
    }

    /* Randomize enemy slot 3 moves */
    void NAKED randomize_enemy_skills_3()
    {
        asm("push {r0, lr} \n"
            "mov r0, #0 \n"
            "mov r1, #17 \n"
            "bl get_rand_int \n"
            "mov r1, r0 \n"
            "pop {r0, pc}");
    }

    /* Randomize enemy slot 4 moves */
    void NAKED randomize_enemy_skills_4()
    {
        asm("push {r1-r2, lr} \n"
            "mov r0, #0 \n"
            "mov r1, #13 \n"
            "bl get_rand_int \n"
            "pop {r1-r2, pc}");
    }

    void install_randomizer()
    {
        if (!config::randomizer.active) return;

        /* Finished */
        install_hook(battle_bgm_pattern, -0x4, (WRAP_SUB), 0, (uint32_t)randomize_battle_bgm);
        install_hook(trans_bgm_pattern, 0x10, (WRAP_SUB), (uint32_t)randomize_trans_bgm, 0);
        install_hook(battle_bg_pattern, 0xC, (USE_LR_TO_RETURN | EXECUTE_OI_BEFORE_CB), (uint32_t)randomize_battle_bg, 0);
        install_hook(battle_intro_pattern, 0x0, (MITM_MODE), (uint32_t)randomize_battle_intro, 0);

        install_hook(stage_bgm_pattern, -0x4, (WRAP_SUB), 0, (uint32_t)randomize_stage_bgm);
        install_hook(title_bg_pattern, 0, (USE_LR_TO_RETURN | EXECUTE_OI_AFTER_CB), (uint32_t)randomize_title_bg, 0);
        install_hook(title_bgm_pattern, -0x4, (WRAP_SUB), (uint32_t)randomize_title_bgm, 0);
        install_hook(title_bgm_pattern, 0x18, (WRAP_SUB), (uint32_t)randomize_title_bgm, 0);
        install_hook(map_bgm_pattern, -0xC, (WRAP_SUB), (uint32_t)randomize_map_bgm, 0);
        install_hook(town_bgm_pattern, 0x10, (WRAP_SUB), (uint32_t)randomize_town_bgm, 0);

        install_hook(enemy_skills_1_pattern, 0xC, (USE_LR_TO_RETURN | EXECUTE_OI_AFTER_CB), (uint32_t)randomize_enemy_skills_1, 0);
        install_hook(enemy_skills_2_pattern, 0x8, (USE_LR_TO_RETURN | EXECUTE_OI_AFTER_CB), (uint32_t)randomize_enemy_skills_2, 0);
        install_hook(enemy_skills_3_pattern, 0x10, (USE_LR_TO_RETURN | EXECUTE_OI_AFTER_CB), (uint32_t)randomize_enemy_skills_3, 0);
        install_hook(enemy_skills_4_pattern, 0xC, (USE_LR_TO_RETURN | EXECUTE_OI_AFTER_CB), (uint32_t)randomize_enemy_skills_4, 0);

        /* WiP */
        install_hook(enemy_pattern, 0x10, (WRAP_SUB), 0, (uint32_t)randomize_enemy);
        install_hook(enemy_stats_pattern, 0x38, (USE_LR_TO_RETURN | EXECUTE_OI_AFTER_CB), (uint32_t)handle_enemy_stats, 0);

        /* For later */
        // install_hook(map_bg_pattern, 0x1C, (USE_LR_TO_RETURN | EXECUTE_OI_AFTER_CB), (uint32_t)randomize_map_bg, 0);
        // install_hook(grub_stats_pattern, 0xC, (USE_LR_TO_RETURN | EXECUTE_OI_AFTER_CB), (uint32_t)randomize_grub_stats, 0);
    }

// clang-format off

    const char* bgm_array[] = {
        "BGM_BATTLE_BOSS_BIGBOSS",
        "BGM_BATTLE_BOSS_BLACK_SAGE",
        "BGM_BATTLE_BOSS_SATAN",
        "BGM_BATTLE_BOSS_SUBBOSS",
        "BGM_BATTLE_BOSS_SUPER_SATAN_BODY",
        "BGM_BATTLE_BOSS_SUPER_SATAN_BODY_TRANSFORMED",
        "BGM_BATTLE_BOSS_SUPER_SATAN_LEFT",
        "BGM_BATTLE_BOSS_SUPER_SATAN_RIGHT",
        "BGM_BATTLE_EVENT",
        "BGM_BATTLE_EXTRA_CLOUD",
        "BGM_BATTLE_EXTRA_CYBERPUNK",
        "BGM_BATTLE_EXTRA_ICE",
        "BGM_BATTLE_EXTRA_SEA",
        "BGM_BATTLE_EXTRA_WONDER",
        "BGM_BATTLE_RARE",
        "BGM_BATTLE_WORLD1",
        "BGM_BATTLE_WORLD2",
        "BGM_BATTLE_WORLD3",
        "BGM_BATTLE_WORLD4",
        "BGM_BATTLE_WORLD6",
        "BGM_MAP_STAGE_1_1_PLAIN",
        "BGM_MAP_STAGE_1_2_FOREST",
        "BGM_MAP_STAGE_1_3_MOUNTAIN",
        "BGM_MAP_STAGE_1_4_TOWER",
        "BGM_MAP_STAGE_2_1_DESERT",
        "BGM_MAP_STAGE_2_2_SAND_CAVE",
        "BGM_MAP_STAGE_2_3_RUINS",
        "BGM_MAP_STAGE_3_1_ELF_FOREST",
        "BGM_MAP_STAGE_3_2_GIANT",
        "BGM_MAP_STAGE_3_3_BIG_TREE",
        "BGM_MAP_STAGE_4_1_HEAT_LAND",
        "BGM_MAP_STAGE_4_2_PANORAMA",
        "BGM_MAP_STAGE_4_2_VOLCANO",
        "BGM_MAP_STAGE_4_3_DEVIL_CASTLE",
        "BGM_MAP_STAGE_5_1_SNOW",
        "BGM_MAP_STAGE_5_2_ICE_CAVE",
        "BGM_MAP_STAGE_5_3_BLIZZARD",
        "BGM_MAP_STAGE_5_4_SKY_LAND",
        "BGM_MAP_STAGE_5_5_HIGH_TECH",
        "BGM_MAP_STAGE_5_6_COLLAPSE",
        "BGM_MAP_STAGE_6_1_GLASSWAY",
        "BGM_MAP_STAGE_6_2_SPACE",
        "BGM_MAP_STAGE_EXTRA_CYBERPANK",
        "BGM_MAP_STAGE_GENERAL_CAVE_00",
        "BGM_MAP_STAGE_GENERAL_CAVE_01",
        "BGM_MAP_STAGE_GENERAL_CREEK_00",
        "BGM_MAP_STAGE_GENERAL_CREEK_01",
        "BGM_MAP_STAGE_GENERAL_MANSION_00",
        "BGM_MAP_STAGE_GENERAL_PANORAMA_01",
        "BGM_MAP_STAGE_GENERAL_PANORAMA_02",
        "BGM_MAP_STAGE_GENERAL_PSYCHE_00",
        "BGM_MAP_STAGE_GENERAL_SEA_00",
        "BGM_MAP_STAGE_GENERAL_SUBSPACE_00",
        "BGM_MAP_STAGE_GENERAL_SUBSPACE_02"
        "BGM_MAP_STAGE_AMIIBO_GIRL",
        "BGM_MAP_STAGE_EVENTMAN",
        "BGM_MAP_STAGE_EXPLORER",
        "BGM_MAP_STAGE_GOURMET",
        "BGM_MAP_STAGE_POSTMAN",
        "BGM_FIRST_TOWN_RESIDENT",
        "BGM_TOWN_1_NORMAL",
        "BGM_TOWN_2_DESERT",
        "BGM_TOWN_3_ELF",
        "BGM_TOWN_4_MOUNTAIN",
        "BGM_TOWN_5_FOREIGNER",
        "BGM_TOWN_PINCH",
        "BGM_MAP_WORLD_1",
        "BGM_MAP_WORLD_2",
        "BGM_MAP_WORLD_3",
        "BGM_MAP_WORLD_4_DEVIL_CASTLE",
        "BGM_MAP_WORLD_4_VOLCANO",
        "BGM_MAP_WORLD_5_SKY_LAND",
        "BGM_MAP_WORLD_5_SNOW",
        "BGM_MAP_WORLD_5_WONDER",
        "BGM_MAP_WORLD_6_GRASSWAY",
        "BGM_MAP_WORLD_6_SPACE",
        "BGM_MAP_WORLD_7_CYBERPUNK",
        "BGM_MAP_WORLD_7_SEA",
        "BGM_MAP_WORLD_DANGEON_NEGATIVE",
        "BGM_MAP_WORLD_DANGEON_RANDOM"
    };

    const char* bg_array[] = {
        "BG/Battle/BtBigTree00",
        "BG/Battle/BtBlizzard00",
        "BG/Battle/BtCave00",
        "BG/Battle/BtCave01",
        "BG/Battle/BtCave02",
        "BG/Battle/BtCollapse00",
        "BG/Battle/BtCreek00",
        "BG/Battle/BtCreek01",
        "BG/Battle/BtCyberPank00",
        "BG/Battle/BtCyberPank01",
        "BG/Battle/BtCyberPank02",
        "BG/Battle/BtDesert00",
        "BG/Battle/BtDevilCastle00",
        "BG/Battle/BtDummy00",
        "BG/Battle/BtElfForest00",
        "BG/Battle/BtForest00",
        "BG/Battle/BtGiant00",
        "BG/Battle/BtGlassWay00",
        "BG/Battle/BtHeatLand00",
        "BG/Battle/BtHighTech00",
        "BG/Battle/BtIceCave00",
        "BG/Battle/BtMansion00",
        "BG/Battle/BtMountain00",
        "BG/Battle/BtOasis00",
        "BG/Battle/BtOtherCastle00",
        "BG/Battle/BtPanorama00",
        "BG/Battle/BtPanorama01",
        "BG/Battle/BtPanorama02",
        "BG/Battle/BtPlain00",
        "BG/Battle/BtPsychedelic00",
        "BG/Battle/BtRuins00",
        "BG/Battle/BtSandCave00",
        "BG/Battle/BtSea00",
        "BG/Battle/BtSkyLand00",
        "BG/Battle/BtSnow00",
        "BG/Battle/BtSpace00",
        "BG/Battle/BtSpace01",
        "BG/Battle/BtSpace02",
        "BG/Battle/BtSubspace00",
        "BG/Battle/BtSubspace02",
        "BG/Battle/BtTower00",
        "BG/Battle/BtVolcano00"
    };

    const char* enemy_array[] = { 
        "Alien0",
        "Alien1",
        "Alien2",
        "Armor0",
        "Armor1",
        "Armor2",
        "Armor3",
        "Banshee0",
        "Banshee1",
        "Banshee2",
        "Banshee3",
        "Bomb0",
        "Bomb1",
        "Bomb2",
        "Bread0",
        "Bread1",
        "Bread2",
        "Bread3",
        "Bread4",
        "Burger0",
        "Burger1_Boss",
        "Burger1",
        "Burger2_Boss",
        "Burger2",
        "Butterfly0",
        "Butterfly1",
        "Butterfly2",
        "Butterfly3",
        "Butterfly4",
        "Butterfly5",
        "Butterfly6",
        "Butterfly7",
        "Butterfly8",
        "Butterfly9",
        "Cactus0",
        "Cactus1",
        "Cactus2",
        "Cerberus0",
        "Cerberus1",
        "Cerberus2",
        "Cerberus3",
        "Cloud0",
        "Cloud1",
        "Cloud2",
        "Cloud3",
        "Cloud4",
        "Cobra1",
        "Cobra2",
        "Cobra3",
        "Crystal0",
        "Crystal1",
        "Crystal2",
        "Demon0",
        "Demon1",
        "Demon2",
        "Demon3",
        "Devil0",
        "Devil1_Boss",
        "Devil1",
        "Devil2",
        "Devil3",
        "Devil4",
        "Devil6",
        "Devil7",
        "Devil8",
        "Devil9",
        "Dog0",
        "Dog1",
        "Dog2",
        "Dragon0",
        "Dragon1",
        "Dragon2_Boss",
        "Dragon2",
        "Dragon3",
        "EatPlant0",
        "EatPlant1",
        "EatPlant2",
        "Excite0",
        "Excite1",
        "Excite2",
        "Fluff0",
        "Fluff1",
        "Fluff2",
        "Fluff3",
        "Fluff4",
        "Fluff5",
        "Fluff6",
        "Fluff7",
        "Fluff8",
        "Fossil0",
        "Fossil1",
        "Fossil2",
        "Fossil3",
        "Fossil4",
        "Fossil5",
        "Frog0",
        "Frog1",
        "Frog2",
        "Frog3",
        "Frog4",
        "Goblin0",
        "Goblin10",
        "Goblin11",
        "GoblinFairy",
        "GoblinMagic",
        "GoblinRed",
        "GoblinRobot",
        "GoblinSpace",
        "Golem0",
        "Golem1",
        "Golem2",
        "Golem3",
        "Golem4",
        "Golem5",
        "Golem6",
        "Gorilla0",
        "Gorilla1",
        "Gorilla2_Boss",
        "Gorilla2",
        "Gremlin0",
        "Gremlin1",
        "Gremlin2",
        "Griffon0_Boss",
        "Griffon0",
        "Griffon1",
        "Griffon2",
        "Hammer0",
        "Hammer1",
        "Hammer2",
        "Harpy0",
        "Harpy1",
        "Harpy2",
        "Hieroglyph0",
        "Hieroglyph1",
        "Hieroglyph2",
        "IceQueen0",
        "IceQueen1_Boss",
        "IceQueen1",
        "IceQueen2",
        "IceQueen3",
        "IronBall0",
        "IronBall1",
        "IronBall2",
        "Lizardman0",
        "Lizardman1",
        "Lizardman2",
        "Lizardman3",
        "Lizardman4",
        "Medusa0",
        "Medusa1",
        "Medusa2",
        "Mimit0",
        "Mimit1",
        "Mimit2",
        "Mimit3",
        "Mimit4",
        "Mimit5",
        "Mimit6",
        "Mimit7",
        "Mimit8",
        "Minotaur0_Boss",
        "Minotaur0",
        "Minotaur1",
        "Minotaur2",
        "Minotaur3",
        "Mole0",
        "Mole1",
        "Mole2",
        "Monitor0",
        "Monitor1",
        "Monitor2",
        "Monitor4",
        "Monitor5",
        "Mouse0",
        "Mouse1",
        "Mouse2",
        "Mouse3",
        "Mouse4",
        "Mouse5",
        "Mouse6",
        "Mummy0",
        "Mummy1",
        "Mummy2",
        "Mural0",
        "Mural1",
        "Mushroom0",
        "Mushroom1",
        "Mushroom2",
        "Nose0",
        "Nose1",
        "Nose2",
        "Owl0",
        "Owl1",
        "Owl2",
        "Owl3",
        "Owl4",
        "Owl5",
        "Owl6",
        "Painting0",
        "Painting1",
        "Painting2",
        "Painting3",
        "Painting4",
        "Painting5",
        "Painting6",
        "Painting7",
        "Penguin0",
        "Penguin1",
        "Penguin2",
        "Pharaoh0",
        "Pharaoh1",
        "Pharaoh2_Boss",
        "Pharaoh2",
        "Puppet0",
        "Puppet1",
        "Puppet2",
        "Robot0",
        "Robot1",
        "Robot2",
        "Robot3_Boss",
        "Robot3",
        "Robot4",
        "Robot5",
        "Rock0",
        "Rock1",
        "Rock2",
        "Rock3",
        "Rock4",
        "Rock5",
        "Rock6",
        "Satan1",
        "Satan2",
        "Satan3_Boss",
        "Satan3",
        "Scorpion0",
        "Scorpion1",
        "Scorpion2",
        "Serpent0",
        "Serpent1",
        "Serpent2_Boss",
        "Serpent2",
        "Shield0",
        "Shield1",
        "Shield2",
        "Shield3",
        "Shield4",
        "Skeleton0",
        "Skeleton1",
        "Skeleton2",
        "Slime0",
        "Slime1",
        "Slime2",
        "Slime3",
        "Slime4",
        "Slime5",
        "Slime6",
        "Slime7",
        "Slime8",
        "Snail0",
        "Snail1",
        "Snail2",
        "Snowman0",
        "Snowman1",
        "Snowman2",
        "Spider0",
        "Spider1",
        "Spider2",
        "Spider3",
        "Spider4",
        "SuperSatan0",
        "SuperSatanBit0",
        "SuperSatanBit1",
        "SuperSatanLeft0",
        "SuperSatanRight0",
        "Sword0",
        "Sword1",
        "Sword2",
        "Sword3",
        "Tadpole0",
        "Tadpole1",
        "Tadpole2",
        "Tadpole3",
        "Tomato1",
        "Tomato2",
        "Turkey0",
        "Turkey1",
        "Turkey2",
        "Ufo0",
        "Ufo1",
        "Ufo2",
        "Ufo3",
        "Ufo4",
        "Ufo5",
        "Wind0",
        "Wind1",
        "Wind2",
        "Wind3",
        "Wind4",
        "Yeti0",
        "Yeti1"
    };

    // These enemies can summon minions
    // As for the Dark(er) lords, it seems like two enemies have to be preloaded otherwise it will most likely crash
    const char* enemy_summon_array[] = { 
        "Cobra0",
        "Cobra4",
        "Cobra5",
        "Monitor3",
        "Satan0",
        "SuperSatanCenter0",
        "SuperSatanCenter1"
    };

// clang-format on

} // namespace hacks

} // namespace CTRPluginFramework
