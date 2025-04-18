#pragma once

#define PATTERN(name, ...) inline const std::vector<uint32_t> name = { __VA_ARGS__ };

namespace CTRPluginFramework {

/* hacks/randomizer.cpp */
PATTERN(battle_bgm_pattern, 0xE596317C, 0xE1A01000, 0xE3A02001, 0xE1A00003); // 0x006B85A8 (-0x4) - US
PATTERN(trans_bgm_pattern, 0xE8BD87F0, 0xE5940064, 0xE3A02001, 0xE28F1060); // 0x004B95A0 (+0x10) - US
PATTERN(battle_bg_pattern, 0xE3A08000, 0xE320F000, 0xEF000028, 0xE28D9FD6); // 0x0042AA9C (+0xC) - US
PATTERN(battle_intro_pattern, 0xE92D4070, 0xE3510000, 0xE1A05002, 0xED2D8B02); // 0x005208AC (0x0) - US

PATTERN(stage_bgm_pattern, 0xE1B05000, 0x0A000063, 0xE59F8198, 0xE5D80000); // 0x00342864 (-0x4) - US
PATTERN(stage_bg_pattern, 0xDA000004, 0xE5900050, 0xE5900000, 0xE3500000,
    0x12800018, 0x1A000000, 0xE3A00000, 0xE12FFF1E); // 0x0072AEB4 (+0x1C) - US

PATTERN(title_bgm_pattern, 0xE59DA04C, 0xE1A0B000, 0xE1A0800A, 0xE59D1048); // 0x00699A9C (-0x4 & +0x18) - US
PATTERN(title_bg_pattern, 0xE5D0001D, 0xE1A0A000, 0xE5980000, 0xE2800020); // 0x00699A04 (0x0) - US

PATTERN(map_bgm_pattern, 0xE58401E0, 0xE3A01000, 0xE28D0008, 0xE58D1008); // 0x002990CC (-0xC) - US

PATTERN(town_bgm_pattern, 0xE3A02000, 0xE5950004, 0xEEB00A48, 0xE1A01002); // 0x0065CE60 (+0x10) - US

PATTERN(enemy_pattern, 0xE58DB020, 0xE3A00001, 0xE5C400E8, 0xE59400C8); // 0x0042A0C4 (+0x10) - US
PATTERN(enemy_stats_pattern, 0xE12FFF1E, 0xE5900018, 0xE3500000, 0x15912000, 0x0A000007); // 0x007FCAA8 (+0x38) - US
PATTERN(enemy_skills_1_pattern, 0xE92D4FF0, 0xE1A04000, 0xE3A07001, 0xED2D8B02); // 0x0040D980 (+0x1C) - US
PATTERN(enemy_skills_2_pattern, 0xE5D10015, 0xE5D17017, 0xE3500019, 0x379FF100); // 0x0040CFC4 (+0x8) - US
PATTERN(enemy_skills_3_pattern, 0xE3A0201C, 0xE0822108, 0xE0800002, 0xED908A00); // 0x0040AA0C (+0x10) - US
PATTERN(enemy_skills_4_pattern, 0xED9F8ABF, 0xE5902000, 0xE5D2000F, 0xE350000D); // 0x0040C8D8 (+0xC) - US

PATTERN(grub_stats_pattern, 0xE3540006, 0xE2800014, 0x30800104, 0xE5900000); // 0x002DC1E8 (+0xC) - US

/* hacks/skills.cpp */
PATTERN(enemy_targetting_skills_pattern, 0xE92D41F0, 0xE24DD0F8, 0xE1A08000, 0xE5910000); // 0x003A6F7C (0x0) - US
PATTERN(ally_targetting_skills_pattern, 0xE92D41F0, 0xE24DD098, 0xE1A07000, 0xE5910000); // 0x003A0598 (0x0) - US
PATTERN(does_skill_target_enemy_pattern, 0xE5910000, 0xE350004F, 0x0A00003C, 0xCA00001F); // 0x006FDC98 (0x0) - US
PATTERN(does_skill_target_ally_pattern, 0xE5910000, 0xE350004C, 0x0A000035, 0xCA00001B); // 0x006FD61C (0x0) - US
PATTERN(can_skill_select_enemy_pattern, 0xE92D4010, 0xE5911000, 0xE351004F, 0x0A00003B); // 0x006FDDA0 (0x0) - US
PATTERN(can_skill_select_ally_pattern, 0xE92D4070, 0xE1A05000, 0xE5910000, 0xE24DD008); // 0x006FD708 (0x0) - US
PATTERN(get_skill_status_pattern, 0xE92D43F0, 0xE1A05000, 0xE24DD00C, 0xE1A06002); // 0x006FBAD4 (0x0) - US
PATTERN(is_not_autoskill_pattern, 0xE5910000, 0xE3500072, 0x0A000019, 0xCA00000D); // 0x006FCC7C (0x0) - US

/* hacks/controllable.cpp */
PATTERN(is_main_hero_pattern, 0xE9540004, 0xE1D819DC, 0xE1D006F0, 0xE1500001); // 0x006B66A4 (+0xC & +0x10) - US
PATTERN(load_skills_page_pattern, 0xE92D43F0, 0xE1A04000, 0xE3E00000, 0xE1A08001); // 0x005DC64C (0x0) - US
PATTERN(autobattle_availability_pattern, 0xE5D60060, 0xE58D6044, 0xE5C5019C, 0xE2877001); // 0x006B8BE0 (-0x4) - US

/* Function pointers */
PATTERN(sead_HashCRC32_calcHash_pattern, 0xE92D00F0, 0xE59F60C8, 0xE5D62000, 0xE3520000); // 0x00104FE0 - US
PATTERN(sead_Random_getU32_pattern, 0xE52D4004, 0xE8901002, 0xE590200C, 0xE5903008); // 0x002478E4 - US
PATTERN(CalcRandPercentage_pattern, 0xE5900008, 0xE1A00000, 0xE92D4070, 0xE2800B01); // 0x0043C2EC - US

PATTERN(GetNumberOfPartyMembers_pattern, 0xE2800C05, 0xE1D00CB8, 0xE3500004, 0x83A00004, 0xE12FFF1E); // 0x0070B0CC - US
PATTERN(GetPartyMemberAtIndex_pattern, 0xE59F202C, 0xE19220B0, 0xE3520004, 0x83A02004); // 0x0043C868 - US
PATTERN(GetNumberOfEnemies_pattern, 0xE2800C05, 0xE1D00CBA, 0xE3500008, 0x83A00008, 0xE12FFF1E); // 0x0070B184 - US
PATTERN(GetEnemyAtIndex_pattern, 0xE59F202C, 0xE19220B0, 0xE3520008, 0x83A02008); // 0x0043A0C0 - US

PATTERN(CanEnemyBeHit_pattern, 0xE92D4010, 0xE1A04000, 0xE5900000, 0xE5901030, 0xE1A00004, 0xE12FFF31, 0xE3500000); // 0x007044FC - US
PATTERN(IsPartyMemberAvailable_pattern, 0xE92D4010, 0xE1A04000, 0xE5900000, 0xE5901034, 0xE1A00004, 0xE12FFF31, 0xE3500000); // 0x006F93C4 - US

PATTERN(GetRandomEnemy_pattern, 0xE92D40F0, 0xE24DD014, 0xE1A04000, 0xE5D00068); // 0x0040C7A8 - US

PATTERN(PlayBattleState_pattern, 0xE1A05001, 0xE1A07002, 0xE5901064, 0xE1A04000); // 0x0043A0F8 - US
PATTERN(_PlayBattleState_pattern, 0xE52DE004, 0xE1D230B0, 0xE24DD014, 0xE1D220B2); // 0x00679CCC - US
PATTERN(LoadSkillEffect_pattern, 0xE52DE004, 0xE1D110B0, 0xE24DD00C, 0xE3A0C03A); // 0x0049A4A4 - US
PATTERN(PlaySkillEffect_pattern, 0xE52DE004, 0xE24DD00C, 0xE3A02051, 0xE58D2004); // 0x0049A504 - US
PATTERN(SetMiiFeeling_pattern, 0xE92D43F0, 0xE1A04000, 0xE24DD02C, 0xE1A07001); // 0x00368F88 - US

PATTERN(HasEnoughMPForSkill_pattern, 0xE92D40F8, 0xE1A04000, 0xE1A06002, 0xE1D006FC); // 0x006F9B74 - US
PATTERN(GetSkillMPCost_pattern, 0xE59F0048, 0xE52DE004, 0xE3520000, 0xE5912000); // 0x00712EB8 - US
PATTERN(SpendSkillMP_pattern, 0xE92D4010, 0xE1D027B8, 0xE5913000, 0xE24DD020); // 0x0036BB44 - US
PATTERN(ShowCutIn_pattern, 0xE92D4030, 0xE1A04000, 0xE5910000, 0xE24DD01C); // 0x00369D80 - US
PATTERN(SetupSkillHelp_pattern, 0xE92D40F0, 0xE1A06000, 0xE24DD024, 0xE1D007B8); // 0x00362BAC - US

PATTERN(CalcDamage_pattern, 0xE92D47F0, 0xE1A05000, 0xE1A09003, 0xE1A0A001); // 0x006FB6B0 - US
PATTERN(SetupDamageParams_pattern, 0xE92D40F0, 0xE24DD014, 0xE1A07000, 0xE5910000); // 0x00679800 - US
PATTERN(DamageEnemy_pattern, 0xE92D40F0, 0xE24DD014, 0xE1A04000, 0xE5D00068); // 0x0040C7A8 - US

PATTERN(CalcHealing_pattern, 0xE92D47F0, 0xE1A08000, 0xE1A09003, 0xE1A0A001); // 0x006FAD1C - US
PATTERN(SetupHealingParams_pattern, 0xE92D40F0, 0xE24DD014, 0xE1A07000, 0xE5910000, 0xE1A05001, 0xE3A03001, 0xE590C040); // 0x0067AF30 - US
PATTERN(HealMiiHP_pattern, 0xE92D41F0, 0xE24DD010, 0xE1A05003, 0xE1A04002, 0xE1A08001); // 0x006798B8 - US
PATTERN(HealMiiMP_pattern, 0xE92D41F0, 0xE24DD010, 0xE1A05003, 0xE1A04002, 0xE1A07001); // 0x0067995C - US

PATTERN(PlayHeartLikeEffect_pattern, 0xE92D4008, 0xE3A03000, 0xE1A02001); // 0x003928B4 - US
PATTERN(UpdateLoveExp_pattern, 0xE92D4FF0, 0xE1A05001, 0xE3A07000, 0xE24DD02C); // 0x003672B0 - US

PATTERN(SummonEnemy_pattern, 0xE92D43F0, 0xE1B04002, 0xE24DD034, 0xE1A09003); // 0x00409ED0 - US
PATTERN(MakeEnemyFlee_pattern, 0xE92D40F0, 0xE24DD014, 0xE1A04000, 0xEB0BEA71); // 0x00409A90 - US

} // namespace CTRPluginFramework
