#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "config.hpp"
#include "func_ptrs.hpp"
#include "hacks/hacks.hpp"
#include "hacks/skills.hpp"
#include "patterns.hpp"

#include "hacks/custom_skills/fighter.hpp"
#include "hacks/custom_skills/scientist.hpp"

#include "mimic_types.hpp"

namespace CTRPluginFramework {

namespace hacks {

    int16_t battle_state_no_target = -1;

    uint32_t enemy_targetting_skills(uintptr_t mii_info, uint32_t* skill_index, uintptr_t enemy_info, uintptr_t target_mii)
    {
        switch (*skill_index) {
            /* PoC: Custom skill for Warrior that targets an enemy */
            case SKILL_FIGHTER_09:
                if (!HasEnoughMPForSkill(mii_info, skill_index, 0))
                    return 0;
                if (!enemy_info)
                    return 0;
                return warrior_flee(mii_info, skill_index, enemy_info);

            /* PoC: Custom skill for Warrior that targets all enemies */
            case SKILL_FIGHTER_10:
                if (!HasEnoughMPForSkill(mii_info, skill_index, 0))
                    return 0;
                return warrior_hit_all(mii_info, skill_index);
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(mii_info, skill_index, enemy_info, target_mii);
    }

    uint32_t ally_targetting_skills(uintptr_t mii_info, uint32_t* skill_index, uint32_t r2, uintptr_t target_mii)
    {
        switch (*skill_index) {
            /* PoC: Custom skill for Warrior that targets an ally */
            case SKILL_FIGHTER_11:
                if (!HasEnoughMPForSkill(mii_info, skill_index, 0))
                    return 0;
                if (!target_mii)
                    return 0;
                return warrior_single_heal(mii_info, skill_index, target_mii);

            /* PoC: Custom skill for Warrior that targets all allies */
            case SKILL_FIGHTER_12:
                if (!HasEnoughMPForSkill(mii_info, skill_index, 0))
                    return 0;
                return warrior_status_all(mii_info, skill_index);
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(mii_info, skill_index, r2, target_mii);
    }

    uint32_t does_skill_target_enemy(uintptr_t r0, uint32_t* skill_index)
    {
        switch (*skill_index) {
            case SKILL_FIGHTER_09:
                return 1;
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(r0, skill_index);
    }

    uint32_t does_skill_target_ally(uintptr_t r0, uint32_t* skill_index)
    {
        switch (*skill_index) {
            case SKILL_FIGHTER_11:
                return 1;
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(r0, skill_index);
    }

    uint32_t can_skill_select_enemy(uintptr_t r0, uint32_t* skill_index, uintptr_t r2)
    {
        switch (*skill_index) {
            case SKILL_FIGHTER_09:
                *skill_index = SKILL_FIGHTER_DOUBLE;
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(r0, skill_index, r2);
    }

    uint32_t can_skill_select_ally(uintptr_t r0, uint32_t* skill_index, uintptr_t r2)
    {
        switch (*skill_index) {
            case SKILL_FIGHTER_11:
                *skill_index = SKILL_PRIEST_CURE;
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(r0, skill_index, r2);
    }

    void get_skill_status(uint32_t* out_status, uintptr_t mii_info, uint32_t* skill_index) {
        switch (*skill_index) {
            case SKILL_FIGHTER_09:
            case SKILL_FIGHTER_10:
            case SKILL_FIGHTER_11:
            case SKILL_FIGHTER_12:
                *out_status = SKILL_STATUS_ENABLE;
                if (-1 < *(int16_t*)(mii_info + 0x6C)) {
                    *out_status = SKILL_STATUS_NO_WEAPON;
                    return;
                }
                if (!HasEnoughMPForSkill(mii_info, skill_index, 0)) {
                    *out_status = SKILL_STATUS_NO_MP;
                    return;
                }
                return;
        }
        HookContext::GetCurrent().OriginalFunction<uint32_t>(out_status, mii_info, skill_index);
    }

    uint32_t is_not_autoskill(uint32_t r0, uint32_t* skill_index) {
        switch (*skill_index) {
            case SKILL_SCIENTIST_09:
                return 0;
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(r0, skill_index);
    }

    uint32_t enemy_slot_1_skills(uintptr_t enemy_info) {
        // I'm too lazy to make structs for this
        // So enjoy this mess for now
        uint8_t* skill_index = (uint8_t*)(*(uintptr_t*)(enemy_info + 0x38)+0xF);

        // I'm using this field to track turns
        // Not sure if this breaks anything
        if (*(uint8_t*)(enemy_info + 0x60))
            *(uint8_t*)(enemy_info + 0x60) -= 1;

        switch (*skill_index) {
            // WiP: Custom enemy skill (recreation of Boss Snurp's moves)
            case ENEMY_SKILL_1_LAST_MIMIT:
                uint32_t status = FEELING_NORMAL;
                uint32_t (*get_enemy_hp)(uintptr_t) = (uint32_t (*)(uintptr_t))(*(uintptr_t*)(*(uintptr_t*)(enemy_info)+0x48));
                uint32_t (*get_enemy_max_hp)(uintptr_t) = (uint32_t (*)(uintptr_t))(*(uintptr_t*)(*(uintptr_t*)(enemy_info)+0x4C));
                float phase_2_hp_threshold = 0.5f;
                float phase_3_hp_threshold = 0.3f;

                if (!*(uint8_t*)(enemy_info + 0x60) && (float)(get_enemy_hp(enemy_info) <= (float)(get_enemy_max_hp(enemy_info) * phase_3_hp_threshold))) {
                    *(uint8_t*)(enemy_info + 0x60) = 5; // Block enemy from using this move for 5 more attacks

                    PlayBattleState(enemy_info, "MagicLock", &battle_state_no_target);
                    for (uint32_t i = 0; i < GetNumberOfPartyMembers(*(uintptr_t*)(enemy_info + 0x8)); i++) {
                        status = Utils::Random(0, 23);
                        if (status == FEELING_FACELESS) status =+ 1;

                        uintptr_t select_mii = GetPartyMemberAtIndex(*(uintptr_t*)(enemy_info + 0x8), i);
                        if (select_mii && IsPartyMemberAvailable(select_mii)) {
                            SetMiiFeeling(select_mii, &status, (int16_t*)(*(uintptr_t*)(select_mii + 0x4) + 0x60), 0);
                            PlayBattleState(select_mii, "ErasedBananaEnd", (int16_t*)(*(uintptr_t*)(select_mii + 0x4) + 0x60));
                            PlayBattleState(select_mii, "SkillResurrectHeal", (int16_t*)(*(uintptr_t*)(select_mii + 0x4) + 0x60));
                        }
                    }
                    return 1;
                }

                if ((float)(get_enemy_hp(enemy_info) <= (float)(get_enemy_max_hp(enemy_info) * phase_2_hp_threshold))) {
                    for (int32_t i = GetNumberOfEnemies(*(uintptr_t*)(enemy_info + 0x8)); i > -1; i--) {
                        uintptr_t select_enemy = GetEnemyAtIndex(*(uintptr_t*)(enemy_info + 0x8), i);
                        uint32_t (*is_enemy_dead)(uintptr_t) = (uint32_t (*)(uintptr_t))(*(uintptr_t*)(*(uintptr_t*)(enemy_info)+0x30));
                        if (select_enemy == enemy_info)
                            continue;
                        if (!select_enemy)
                            continue;
                        // Only revive when all other enemies are dead, otherwise use WideAttack
                        if (!is_enemy_dead(select_enemy)) {
                            goto start_wide_attack;
                        }
                        uint32_t state = 1; // Unsure what this is for, but it's used as an argument for PlayState
                        SummonEnemy(enemy_info, &state, select_enemy, 0);
                    }
                    return 1;
                }

start_wide_attack:
                *skill_index = ENEMY_SKILL_1_WIDE_ATTACK;
                return HookContext::GetCurrent().OriginalFunction<uint32_t>(enemy_info);
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(enemy_info);
    }

    void install_skills()
    {
        if (!config::skills.active) return;

        install_hook(enemy_targetting_skills_pattern, 0x0, (MITM_MODE), (uint32_t)enemy_targetting_skills, 0);
        install_hook(ally_targetting_skills_pattern, 0x0, (MITM_MODE), (uint32_t)ally_targetting_skills, 0);

        install_hook(does_skill_target_enemy_pattern, 0x0, (MITM_MODE), (uint32_t)does_skill_target_enemy, 0);
        install_hook(does_skill_target_ally_pattern, 0x0, (MITM_MODE), (uint32_t)does_skill_target_ally, 0);
        install_hook(can_skill_select_enemy_pattern, 0x0, (MITM_MODE), (uint32_t)can_skill_select_enemy, 0);
        install_hook(can_skill_select_ally_pattern, 0x0, (MITM_MODE), (uint32_t)can_skill_select_ally, 0);
        install_hook(get_skill_status_pattern, 0x0, (MITM_MODE), (uint32_t)get_skill_status, 0);
        install_hook(is_not_autoskill_pattern, 0x0, (MITM_MODE), (uint32_t)is_not_autoskill, 0);

        install_hook(CalcDamage_pattern, 0x0, (MITM_MODE), (uint32_t)scientist_pre_optimize, 0);
        install_hook(CalcHealing_pattern, 0x0, (MITM_MODE), (uint32_t)scientist_pre_optimize, 0);
        install_hook(SpendSkillMP_pattern, 0x24, (USE_LR_TO_RETURN), (uint32_t)scientist_optimize, 0);

        install_hook(enemy_skills_1_pattern, 0x0, (MITM_MODE), (uint32_t)enemy_slot_1_skills, 0);
    }

} // namespace hacks

} // namespace CTRPluginFramework
