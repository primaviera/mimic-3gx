#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "func_ptrs.hpp"
#include "hacks/skills.hpp"
#include "hacks/custom_skills/fighter.hpp"

#include "mimic_types.hpp"

namespace CTRPluginFramework {

namespace hacks {

    uint32_t warrior_flee(uintptr_t mii_info, uint32_t* skill_index, uintptr_t enemy_info) {
        PlayBattleState(mii_info, "SkillDanceStart", (int16_t*)(*(uintptr_t*)(enemy_info + 0x4) + 0x60));
        ShowCutIn(mii_info, skill_index);
        SpendSkillMP(mii_info, skill_index);
        
        PlayBattleState(mii_info, "SkillDance", (int16_t*)(*(uintptr_t*)(enemy_info + 0x4) + 0x60));
        PlayBattleState(enemy_info, "ToFeelFever", (int16_t*)(*(uintptr_t*)(mii_info + 0x4) + 0x60));
        MakeEnemyFlee(enemy_info);
        return 1;
    }

    uint32_t warrior_hit_all(uintptr_t mii_info, uint32_t* skill_index) {
        uint32_t* damage_calc = (uint32_t*)malloc(0x24);
        uint32_t* damage_params = (uint32_t*)malloc(0x10);

        *skill_index = SKILL_FIGHTER_DOUBLE; // I'm too lazy to make stats for these moves in skill.sarc
        CalcDamage(1.0f, damage_calc, mii_info, skill_index, 0, 0);
        *skill_index = SKILL_FIGHTER_10;

        PlayBattleState(mii_info, "SkillDanceStart", &battle_state_no_target);
        ShowCutIn(mii_info, skill_index);
        SpendSkillMP(mii_info, skill_index);

        PlayBattleState(mii_info, "SkillDance", &battle_state_no_target);
        PlayBattleState(mii_info, "SkillArrowRainHit", &battle_state_no_target);
        for (uint32_t i = 0; i < GetNumberOfEnemies(*(uintptr_t*)(mii_info + 0x8)); i++) {
            uintptr_t select_enemy = GetEnemyAtIndex(*(uintptr_t*)(mii_info + 0x8), i);
            if (select_enemy && CanEnemyBeHit(select_enemy)) {
                SetupDamageParams(1.0f, damage_params, select_enemy, damage_calc);
                DamageEnemy(select_enemy, mii_info, damage_params, 1);
            }
        }

        free(damage_calc);
        free(damage_params);
        return 1;
    }

    uint32_t warrior_single_heal(uintptr_t mii_info, uint32_t* skill_index, uintptr_t target_mii) {
        uint32_t* heal_calc = (uint32_t*)malloc(0x24);
        uint32_t* healing_params = (uint32_t*)malloc(0x10);

        *skill_index = SKILL_PRIEST_CURE; // I'm too lazy to make stats for these moves in skill.sarc
        CalcHealing(1.0f, heal_calc, mii_info, skill_index, target_mii, 0);
        *skill_index = SKILL_FIGHTER_11;

        PlayBattleState(mii_info, "DefeatEnemyHelp", (int16_t*)(*(uintptr_t*)(mii_info + 0x4) + 0x60));
        ShowCutIn(mii_info, skill_index);
        SpendSkillMP(mii_info, skill_index);

        SetupHealingParams(1.0f, healing_params, target_mii, heal_calc);
        HealMiiHP(target_mii, healing_params, (int16_t*)(*(uintptr_t*)(mii_info + 0x4) + 0x60), 1);

        if (mii_info == target_mii) {
            PlayBattleState(target_mii, "CureSelf", (int16_t*)(*(uintptr_t*)(mii_info + 0x4) + 0x60));
        } else {
            PlayBattleState(target_mii, "CureNormal", (int16_t*)(*(uintptr_t*)(mii_info + 0x4) + 0x60));
        }

        free(heal_calc);
        free(healing_params);
        return 1;
    }

    uint32_t warrior_status_all(uintptr_t mii_info, uint32_t* skill_index) {
        uint32_t* heal_calc = (uint32_t*)malloc(0x24);
        uint32_t* healing_params = (uint32_t*)malloc(0x10);
        uint32_t status = FEELING_NORMAL;

        *skill_index = SKILL_PRIEST_CURE3; // I'm too lazy to make stats for these moves in skill.sarc
        CalcHealing(1.0f, heal_calc, mii_info, skill_index, 0, 0);
        *skill_index = SKILL_FIGHTER_12;

        PlayBattleState(mii_info, "DefeatEnemyHelp", (int16_t*)(*(uintptr_t*)(mii_info + 0x4) + 0x60));
        ShowCutIn(mii_info, skill_index);
        SpendSkillMP(mii_info, skill_index);

        PlayBattleState(mii_info, "SkillWhistleCureStart", &battle_state_no_target);
        for (uint32_t i = 0; i < GetNumberOfPartyMembers(*(uintptr_t*)(mii_info + 0x8)); i++) {
            status = Utils::Random(0, 23);
            if (status == FEELING_FACELESS) status =+ 1;
            uintptr_t select_mii = GetPartyMemberAtIndex(*(uintptr_t*)(mii_info + 0x8), i);
            if (select_mii && IsPartyMemberAvailable(select_mii)) {
                SetupHealingParams(1.0f, healing_params, select_mii, heal_calc);
                if (Utils::Random(0, 1)) {
                    HealMiiHP(select_mii, healing_params, (int16_t*)(*(uintptr_t*)(mii_info + 0x4) + 0x60), 1);
                } else {
                    HealMiiMP(select_mii, healing_params, (int16_t*)(*(uintptr_t*)(mii_info + 0x4) + 0x60), 1);
                }
                SetMiiFeeling(select_mii, &status, (int16_t*)(*(uintptr_t*)(select_mii + 0x4) + 0x60), 0);
                PlayBattleState(select_mii, "ErasedBananaEnd", (int16_t*)(*(uintptr_t*)(select_mii + 0x4) + 0x60));
                PlayBattleState(select_mii, "DogfightEndAttackHitL", (int16_t*)(*(uintptr_t*)(select_mii + 0x4) + 0x60));
            }
        }
        PlayBattleState(mii_info, "SkillWhistleCureEnd", &battle_state_no_target);

        free(heal_calc);
        free(healing_params);
        return 1;
    }

} // namespace hacks

} // namespace CTRPluginFramework
