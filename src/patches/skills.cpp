#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "config.hpp"
#include "func_ptrs.hpp"
#include "standalone/mimic_types.hpp"
#include "standalone/patterns.hpp"

#include "patches.hpp"
#include "patches/skills.hpp"
#include "patches/skills/fighter.hpp"
#include "patches/skills/scientist.hpp"

namespace CTRPluginFramework {

namespace patches {

    int16_t gInvalidTarget = -1;

    uint32_t EnemyTargettingSkills(uintptr_t mii_info, uint32_t* skill_index, uintptr_t enemy_info, uintptr_t target_mii)
    {
        switch (*skill_index) {
            case SKILL_FIGHTER_09:
                if (!HasEnoughMPForSkill(mii_info, skill_index, 0))
                    return 0;
                if (!enemy_info)
                    return 0;
                return FighterFlee(mii_info, skill_index, enemy_info);

            case SKILL_FIGHTER_10:
                if (!HasEnoughMPForSkill(mii_info, skill_index, 0))
                    return 0;
                return FighterHitAll(mii_info, skill_index);
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(mii_info, skill_index, enemy_info, target_mii);
    }

    uint32_t AllyTargettingSkills(uintptr_t mii_info, uint32_t* skill_index, uint32_t r2, uintptr_t target_mii)
    {
        switch (*skill_index) {
            case SKILL_FIGHTER_11:
                if (!HasEnoughMPForSkill(mii_info, skill_index, 0))
                    return 0;
                if (!target_mii)
                    return 0;
                return FighterSingleHeal(mii_info, skill_index, target_mii);

            case SKILL_FIGHTER_12:
                if (!HasEnoughMPForSkill(mii_info, skill_index, 0))
                    return 0;
                return FighterStatusAll(mii_info, skill_index);
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(mii_info, skill_index, r2, target_mii);
    }

    uint32_t DoesSkillTargetEnemy(uintptr_t r0, uint32_t* skill_index)
    {
        switch (*skill_index) {
            case SKILL_FIGHTER_09:
                return 1;
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(r0, skill_index);
    }

    uint32_t DoesSkillTargetAlly(uintptr_t r0, uint32_t* skill_index)
    {
        switch (*skill_index) {
            case SKILL_FIGHTER_11:
                return 1;
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(r0, skill_index);
    }

    uint32_t CanSkillSelectEnemy(uintptr_t r0, uint32_t* skill_index, uintptr_t r2)
    {
        switch (*skill_index) {
            case SKILL_FIGHTER_09:
                /* Select any enemy(?) */
                *skill_index = SKILL_FIGHTER_DOUBLE;
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(r0, skill_index, r2);
    }

    uint32_t CanSkillSelectAlly(uintptr_t r0, uint32_t* skill_index, uintptr_t r2)
    {
        switch (*skill_index) {
            case SKILL_FIGHTER_11:
                /* Select anyone, including yourself. */
                *skill_index = SKILL_PRIEST_CURE;
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(r0, skill_index, r2);
    }

    void GetSkillStatus(uint32_t* out_status, uintptr_t mii_info, uint32_t* skill_index) {
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

    uint32_t IsNotAutoSkill(uint32_t r0, uint32_t* skill_index) {
        switch (*skill_index) {
            case SKILL_SCIENTIST_09:
                return 0;
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(r0, skill_index);
    }

    uint32_t EnemySlot1Skills(uintptr_t enemy_info) {
        uint8_t* skill_index = (uint8_t*)(*(uintptr_t*)(enemy_info + 0x38)+0xF);

        if (*(uint8_t*)(enemy_info + 0x60))
            *(uint8_t*)(enemy_info + 0x60) -= 1;

        switch (*skill_index) {
            /* Recreation of Boss Snurp's moves. */
            case ENEMY_SKILL_1_LAST_MIMIT:
                uint32_t status = FEELING_NORMAL;
                uint32_t (*get_enemy_hp)(uintptr_t) = (uint32_t (*)(uintptr_t))(*(uintptr_t*)(*(uintptr_t*)(enemy_info)+0x48));
                uint32_t (*get_enemy_max_hp)(uintptr_t) = (uint32_t (*)(uintptr_t))(*(uintptr_t*)(*(uintptr_t*)(enemy_info)+0x4C));
                float phase_2_hp_threshold = 0.5f;
                float phase_3_hp_threshold = 0.3f;

                if (!*(uint8_t*)(enemy_info + 0x60) && (float)(get_enemy_hp(enemy_info) <= (float)(get_enemy_max_hp(enemy_info) * phase_3_hp_threshold))) {
                    *(uint8_t*)(enemy_info + 0x60) = 5; // Block enemy from using this move for 5 more attacks

                    _PlayBattleState(enemy_info, "MagicLock", &gInvalidTarget);
                    for (uint32_t i = 0; i < GetNumberOfPartyMembers(*(uintptr_t*)(enemy_info + 0x8)); i++) {
                        status = Utils::Random(0, 23);
                        if (status == FEELING_FACELESS) status =+ 1;

                        uintptr_t select_mii = GetPartyMemberAtIndex(*(uintptr_t*)(enemy_info + 0x8), i);
                        if (select_mii && IsPartyMemberAvailable(select_mii)) {
                            SetMiiFeeling(select_mii, &status, (int16_t*)(*(uintptr_t*)(select_mii + 0x4) + 0x60), 0);
                            _PlayBattleState(select_mii, "ErasedBananaEnd", (int16_t*)(*(uintptr_t*)(select_mii + 0x4) + 0x60));
                            _PlayBattleState(select_mii, "SkillResurrectHeal", (int16_t*)(*(uintptr_t*)(select_mii + 0x4) + 0x60));
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
                        /* Only summon enemies if all other enemies are dead. */
                        if (!is_enemy_dead(select_enemy)) {
                            goto start_wide_attack;
                        }
                        uint32_t state = 1;
                        SummonEnemy(enemy_info, &state, select_enemy, 0);
                    }
                    return 1;
                }

start_wide_attack:
                /* Fallback attack. */
                *skill_index = ENEMY_SKILL_1_WIDE_ATTACK;
                return HookContext::GetCurrent().OriginalFunction<uint32_t>(enemy_info);
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(enemy_info);
    }

    void InstallSkills()
    {
        if (!config::gConf.mSkills.active) return;

        InstallHookAtPattern(enemyTargettingSkills_pattern, 0x0, (MITM_MODE), (uint32_t)EnemyTargettingSkills, 0);
        InstallHookAtPattern(allyTargettingSkills_pattern, 0x0, (MITM_MODE), (uint32_t)AllyTargettingSkills, 0);

        InstallHookAtPattern(doesSkillTargetEnemy_pattern, 0x0, (MITM_MODE), (uint32_t)DoesSkillTargetEnemy, 0);
        InstallHookAtPattern(doesSkillTargetAlly_pattern, 0x0, (MITM_MODE), (uint32_t)DoesSkillTargetAlly, 0);
        InstallHookAtPattern(canSkillSelectEnemy_pattern, 0x0, (MITM_MODE), (uint32_t)CanSkillSelectEnemy, 0);
        InstallHookAtPattern(canSkillSelectAlly_pattern, 0x0, (MITM_MODE), (uint32_t)CanSkillSelectAlly, 0);
        InstallHookAtPattern(getSkillStatus_pattern, 0x0, (MITM_MODE), (uint32_t)GetSkillStatus, 0);
        InstallHookAtPattern(isNotAutoSkill_pattern, 0x0, (MITM_MODE), (uint32_t)IsNotAutoSkill, 0);

        InstallHookAtPattern(CalcDamage_pattern, 0x0, (MITM_MODE), (uint32_t)ScientistPreOptimize, 0);
        InstallHookAtPattern(CalcHealing_pattern, 0x0, (MITM_MODE), (uint32_t)ScientistPreOptimize, 0);
        InstallHookAtPattern(SpendSkillMP_pattern, 0x24, (USE_LR_TO_RETURN), (uint32_t)ScientistOptimize, 0);

        InstallHookAtPattern(enemySkills1_pattern, 0x0, (MITM_MODE), (uint32_t)EnemySlot1Skills, 0);
    }

} // namespace hacks

} // namespace CTRPluginFramework
