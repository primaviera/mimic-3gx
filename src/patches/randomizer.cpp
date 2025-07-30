#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "config.hpp"
#include "func_ptrs.hpp"
#include "logger.hpp"
#include "standalone/mimic_types.hpp"
#include "standalone/patterns.hpp"

#include "patches.hpp"
#include "patches/randomizer.hpp"

namespace CTRPluginFramework {

namespace patches {

    static sead::Random sRandomizerSeed;
    static void** sBGDataManager;

    extern "C" {
        extern const char* bgmArray[];
        extern const char* bgArray[];
        extern const char* enemyArray[];

        int GetRandU32(int min, int max)
        {
            if (!sRandomizerSeed.mX && !sRandomizerSeed.mY && !sRandomizerSeed.mZ && !sRandomizerSeed.mW)
                sead_Random_init(&sRandomizerSeed);
            return min + sead_Random_getU32(&sRandomizerSeed) % (max + 1 - min);
        }
    }

    /* Randomize battle BGM (for now). */
    uint32_t RandomizeBattleData(uintptr_t arg1, uintptr_t arg2, uintptr_t arg3)
    {
        auto res = ORIG(uint32_t, arg1, arg2, arg3);
        *(uint32_t*)(arg2 + 0x60) = GetBGMId(bgmArray[GetRandU32(0, 74)]);
        return res;
    }

    /* Randomize Darker Lord Second Phase BGM. */
    void NAKED RandomizeTransBgm()
    {
        asm("push {r0, r2, lr} \n"
            "mov r0, #0 \n"
            "mov r1, #74 \n"
            "bl GetRandU32 \n"
            "ldr r2, =bgmArray \n"
            "ldr r1, [r2, r0, lsl #2] \n"
            "pop {r0, r2, pc}");
    }

    /* Randomize battle intro jingle. */
    void RandomizeBattleIntro(uintptr_t arg1, uintptr_t arg2, uint32_t* index)
    {
        *index = GetRandU32(0, 2);
        ORIG(void, arg1, arg2, index);
    }

    /* Randomize enemies in battles. */
    void RandomizeEnemy(uintptr_t arg1, uintptr_t arg2)
    {
#ifdef DEBUG
        logger::Write("[Randomizer] ");
#endif // DEBUG
        int maxNum = GetRandU32(1, 4);
        for (int i = 0; i < 8; i++) {
            uint32_t* enemyHash = (uint32_t*)(arg1 + (i * 0x10) + 0x8);
            if (i >= maxNum) {
                *enemyHash = 0;
                continue;
            }
            uint32_t randomId = GetRandU32(0, 299);
            const char* enemyName = enemyArray[randomId];
            *enemyHash = sead_HashCRC32_calcHash(enemyName, strlen(enemyName));
#ifdef DEBUG
            logger::Write(Utils::Format("%s:(%d) ", enemyName, randomId));
#endif // DEBUG
        }
#ifdef DEBUG
        logger::Write("\n");
#endif // DEBUG
        ORIG(void, arg1, arg2);
    }

    /* Edit enemy stats in battle. */
    EnemyParam* HandleEnemyStats(EnemyParam* enemyParam)
    {
        if (!enemyParam)
            return enemyParam;
#ifdef TESTING
        /* For testing purposes. */
        enemyParam->mEnemyStatus->mHp = 1;
        enemyParam->mEnemyStatus->mMp = 0;
        enemyParam->mEnemyStatus->mAtk = 0;
        enemyParam->mEnemyStatus->mDef = 0;
        enemyParam->mEnemyStatus->mMag = 0;
        enemyParam->mEnemyStatus->mSpd = 0;
        enemyParam->mEnemyStatus->mGold = 0x7FFF;
        enemyParam->mEnemyStatus->mExp = 0x7FFF;
#endif // TESTING
        return enemyParam;
    }

    /* Randomize stage BGM. */
    void NAKED RandomizeStageBgm()
    {
        /* Only randomize the BGM when r7 is 0 (at the start of a level). */
        asm("push {r4, lr} \n"
            "add r4, pc, #0x28 \n"
            "ldr r0, [r4] \n"
            "cmp r7, #0 \n"
            "moveq r0, #0 \n"
            "moveq r1, #74 \n"
            "bleq GetRandU32 \n"
            "str r0, [r4] \n"
            "ldr r1, =bgmArray \n"
            "ldr r0, [r1, r0, lsl #2] \n"
            "pop {r4, pc} \n"
            ".word 0x00000000");
    }

    /* Randomize stage background. */
    BGData* RandomizeStageBg(void* bgDataManager, uint32_t* hash)
    {
        const char* randomBg = bgArray[GetRandU32(0, 36)];
        *hash = sead_HashCRC32_calcHash(randomBg, strlen(randomBg));
        return GetBGDataWithHash(bgDataManager, hash);
    }

    /* Randomize title BGM. */
    void NAKED RandomizeTitleBgm()
    {
        asm("push {r0, r2, lr} \n"
            "mov r0, #0 \n"
            "mov r1, #27 \n"
            "bl GetRandU32 \n"
            "mov r1, r0 \n"
            "pop {r0, r2, pc}");
    }

    /* Randomize title background. */
    void NAKED RandomizeTitleBg()
    {
        asm("push {r0, lr} \n"
            "mov r0, #0 \n"
            "mov r1, #27 \n"
            "bl GetRandU32 \n"
            "mov r1, r0 \n"
            "pop {r0} \n"
            "strb r1, [r0, #0x1d] \n"
            "pop {pc}");
    }

    /* Randomize map BGM. */
    void NAKED RandomizeMapBgm()
    {
        asm("push {r1-r2, lr} \n"
            "mov r0, #0 \n"
            "mov r1, #74 \n"
            "bl GetRandU32 \n"
            "ldr r1, =bgmArray \n"
            "ldr r0, [r1, r0, lsl #2] \n"
            "pop {r1-r2, pc}");
    }

    /* Randomize town BGM. */
    void NAKED RandomizeTownBgm()
    {
        asm("push {r1-r2, lr} \n"
            "mov r0, #0 \n"
            "mov r1, #74 \n"
            "bl GetRandU32 \n"
            "ldr r1, =bgmArray \n"
            "ldr r0, [r1, r0, lsl #2] \n"
            "str r0, [r5, #4] \n"
            "pop {r1-r2, pc}");
    }

    void InstallRandomizer()
    {
        if (!config::gUserConfig.mRandomizer.active)
            return;

        logger::Write("[Randomizer] Installing...\n");

        /* Get pointer required to get BGData via GetBGDataWithHash. */
        if (auto res = SEARCH_PATTERN(uint32_t, bgDataManagerAddr_Pattern)) {
            uint32_t addr;
            Process::Read32(res + 0x10, addr);
            sBGDataManager = (void**)(addr);
#ifdef DEBUG
            logger::Write(Utils::Format("[Randomizer] sBGDataManager -> %08X\n", addr));
#endif // DEBUG
        }

        InstallHookAtPattern(battleData_Pattern, 0x0, (MITM_MODE), (uint32_t)RandomizeBattleData, 0);
        InstallHookAtPattern(transBgm_Pattern, 0x10, (WRAP_SUB), (uint32_t)RandomizeTransBgm, 0);
        InstallHookAtPattern(battleIntro_Pattern, 0x0, (MITM_MODE), (uint32_t)RandomizeBattleIntro, 0);

        InstallHookAtPattern(enemyData_Pattern, 0x0, (MITM_MODE), (uint32_t)RandomizeEnemy, 0);
        InstallHookAtPattern(enemyParam_Pattern, 0x38, (USE_LR_TO_RETURN | EXECUTE_OI_AFTER_CB),
            (uint32_t)HandleEnemyStats, 0);

        InstallHookAtPattern(stageBg_Pattern, -0x4, (USE_LR_TO_RETURN), (uint32_t)RandomizeStageBg, 0);
        InstallHookAtPattern(stageBgm_Pattern, -0x4, (WRAP_SUB), 0, (uint32_t)RandomizeStageBgm);
        InstallHookAtPattern(titleBg_Pattern, 0, (USE_LR_TO_RETURN | EXECUTE_OI_AFTER_CB), (uint32_t)RandomizeTitleBg,
            0);
        InstallHookAtPattern(titleBgm_Pattern, -0x4, (WRAP_SUB), (uint32_t)RandomizeTitleBgm, 0);
        InstallHookAtPattern(titleBgm_Pattern, 0x18, (WRAP_SUB), (uint32_t)RandomizeTitleBgm, 0);
        InstallHookAtPattern(mapBgm_Pattern, -0xC, (WRAP_SUB), (uint32_t)RandomizeMapBgm, 0);
        InstallHookAtPattern(townBgm_Pattern, 0x10, (WRAP_SUB), (uint32_t)RandomizeTownBgm, 0);
        InstallHookAtPattern(innBgm_Pattern, 0x44, (WRAP_SUB), (uint32_t)RandomizeMapBgm, 0);
    }

    // clang-format off

    const char* bgmArray[] = {
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
        "BGM_MAP_STAGE_GENERAL_SUBSPACE_02",
        "BGM_MAP_STAGE_AMIIBO_GIRL",
        "BGM_MAP_STAGE_EVENTMAN",
        "BGM_MAP_STAGE_EXPLORER",
        "BGM_MAP_STAGE_GOURMET",
        "BGM_MAP_STAGE_POSTMAN",
        "BGM_FIRST_TOWN_RESIDENT",
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

    const char* bgArray[] = {
        "BigTree00",
        "Blizzard00",
        "Cave00",
        "Cave01",
        "Cave02",
        "Collapse00",
        "Creek00",
        "Creek01",
        "CyberPank00",
        "CyberPank02",
        "Desert00",
        "DevilCastle00",
        "ElfForest00",
        "Forest00",
        "Giant00",
        "GlassWay00",
        "HeatLand00",
        "HighTech00",
        "IceCave00",
        "Mansion00",
        "Mountain00",
        "Panorama00",
        "Panorama01",
        "Panorama02",
        "Plain00",
        "Psychedelic00",
        "Ruins00",
        "SandCave00",
        "Sea00",
        "SkyLand00",
        "Snow00",
        "Space00",
        "Space01",
        "Subspace00",
        "Subspace02",
        "Tower00",
        "Volcano00",
        /* These backgrounds only apply to battles. */
        "CyberPank01",
        "Dummy00",
        "Oasis00",
        "OtherCastle00",
        "Space02",
    };

    const char* enemyArray[] = { 
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

    // clang-format on

} // namespace patches

} // namespace CTRPluginFramework
