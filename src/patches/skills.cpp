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

    uint32_t EnemyTargettingSkills(uintptr_t miiInfo, uint32_t* skillIndex, uintptr_t enemyInfo, uintptr_t targetMii)
    {
        switch (*skillIndex) {
            case SKILL_FIGHTER_09:
                if (!HasEnoughMPForSkill(miiInfo, skillIndex, 0))
                    return 0;
                if (!enemyInfo)
                    return 0;
                return FighterFlee(miiInfo, skillIndex, enemyInfo);

            case SKILL_FIGHTER_10:
                if (!HasEnoughMPForSkill(miiInfo, skillIndex, 0))
                    return 0;
                return FighterHitAll(miiInfo, skillIndex);
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(miiInfo, skillIndex, enemyInfo, targetMii);
    }

    uint32_t AllyTargettingSkills(uintptr_t miiInfo, uint32_t* skillIndex, uint32_t r2, uintptr_t targetMii)
    {
        switch (*skillIndex) {
            case SKILL_FIGHTER_11:
                if (!HasEnoughMPForSkill(miiInfo, skillIndex, 0))
                    return 0;
                if (!targetMii)
                    return 0;
                return FighterSingleHeal(miiInfo, skillIndex, targetMii);

            case SKILL_FIGHTER_12:
                if (!HasEnoughMPForSkill(miiInfo, skillIndex, 0))
                    return 0;
                return FighterStatusAll(miiInfo, skillIndex);
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(miiInfo, skillIndex, r2, targetMii);
    }

    uint32_t DoesSkillTargetEnemy(uintptr_t r0, uint32_t* skillIndex)
    {
        switch (*skillIndex) {
            case SKILL_FIGHTER_09:
                return 1;
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(r0, skillIndex);
    }

    uint32_t DoesSkillTargetAlly(uintptr_t r0, uint32_t* skillIndex)
    {
        switch (*skillIndex) {
            case SKILL_FIGHTER_11:
                return 1;
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(r0, skillIndex);
    }

    uint32_t CanSkillSelectEnemy(uintptr_t r0, uint32_t* skillIndex, uintptr_t r2)
    {
        switch (*skillIndex) {
            case SKILL_FIGHTER_09:
                /* Select any enemy(?) */
                *skillIndex = SKILL_FIGHTER_DOUBLE;
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(r0, skillIndex, r2);
    }

    uint32_t CanSkillSelectAlly(uintptr_t r0, uint32_t* skillIndex, uintptr_t r2)
    {
        switch (*skillIndex) {
            case SKILL_FIGHTER_11:
                /* Select anyone, including yourself. */
                *skillIndex = SKILL_PRIEST_CURE;
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(r0, skillIndex, r2);
    }

    void GetSkillStatus(uint32_t* outStatus, uintptr_t miiInfo, uint32_t* skillIndex) {
        switch (*skillIndex) {
            case SKILL_FIGHTER_09:
            case SKILL_FIGHTER_10:
            case SKILL_FIGHTER_11:
            case SKILL_FIGHTER_12:
                *outStatus = SKILL_STATUS_ENABLE;
                if (-1 < *(int16_t*)(miiInfo + 0x6C)) {
                    *outStatus = SKILL_STATUS_NO_WEAPON;
                    return;
                }
                if (!HasEnoughMPForSkill(miiInfo, skillIndex, 0)) {
                    *outStatus = SKILL_STATUS_NO_MP;
                    return;
                }
                return;
        }
        HookContext::GetCurrent().OriginalFunction<uint32_t>(outStatus, miiInfo, skillIndex);
    }

    uint32_t IsNotAutoSkill(uint32_t r0, uint32_t* skillIndex) {
        switch (*skillIndex) {
            case SKILL_SCIENTIST_09:
                return 0;
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(r0, skillIndex);
    }

    uint32_t EnemySlot1Skills(uintptr_t enemyInfo) {
        uint8_t* skillIndex = (uint8_t*)(*(uintptr_t*)(enemyInfo + 0x38)+0xF);

        if (*(uint8_t*)(enemyInfo + 0x60))
            *(uint8_t*)(enemyInfo + 0x60) -= 1;

        switch (*skillIndex) {
            /* Recreation of Boss Snurp's moves. */
            case ENEMY_SKILL_1_LAST_MIMIT:
                uint32_t status = FEELING_NORMAL;
                uint32_t (*GetEnemyHp)(uintptr_t) = (uint32_t (*)(uintptr_t))(*(uintptr_t*)(*(uintptr_t*)(enemyInfo)+0x48));
                uint32_t (*GetEnemyMaxHp)(uintptr_t) = (uint32_t (*)(uintptr_t))(*(uintptr_t*)(*(uintptr_t*)(enemyInfo)+0x4C));
                float phase2HpThreshold = 0.5f;
                float phase3HpThreshold = 0.3f;

                if (!*(uint8_t*)(enemyInfo + 0x60) && (float)(GetEnemyHp(enemyInfo) <= (float)(GetEnemyMaxHp(enemyInfo) * phase3HpThreshold))) {
                    *(uint8_t*)(enemyInfo + 0x60) = 5; // Block enemy from using this move for 5 more attacks

                    _PlayBattleState(enemyInfo, "MagicLock", &gInvalidTarget);
                    for (uint32_t i = 0; i < GetNumberOfPartyMembers(*(uintptr_t*)(enemyInfo + 0x8)); i++) {
                        status = Utils::Random(0, 23);
                        if (status == FEELING_FACELESS) status =+ 1;

                        uintptr_t selectMii = GetPartyMemberAtIndex(*(uintptr_t*)(enemyInfo + 0x8), i);
                        if (selectMii && IsPartyMemberAvailable(selectMii)) {
                            SetMiiFeeling(selectMii, &status, (int16_t*)(*(uintptr_t*)(selectMii + 0x4) + 0x60), 0);
                            _PlayBattleState(selectMii, "ErasedBananaEnd", (int16_t*)(*(uintptr_t*)(selectMii + 0x4) + 0x60));
                            _PlayBattleState(selectMii, "SkillResurrectHeal", (int16_t*)(*(uintptr_t*)(selectMii + 0x4) + 0x60));
                        }
                    }
                    return 1;
                }

                if ((float)(GetEnemyHp(enemyInfo) <= (float)(GetEnemyMaxHp(enemyInfo) * phase2HpThreshold))) {
                    for (int32_t i = GetNumberOfEnemies(*(uintptr_t*)(enemyInfo + 0x8)); i > -1; i--) {
                        uintptr_t selectEnemy = GetEnemyAtIndex(*(uintptr_t*)(enemyInfo + 0x8), i);
                        uint32_t (*IsEnemyDead)(uintptr_t) = (uint32_t (*)(uintptr_t))(*(uintptr_t*)(*(uintptr_t*)(enemyInfo)+0x30));
                        if (selectEnemy == enemyInfo)
                            continue;
                        if (!selectEnemy)
                            continue;
                        /* Only summon enemies if all other enemies are dead. */
                        if (!IsEnemyDead(selectEnemy)) {
                            goto start_wide_attack;
                        }
                        uint32_t state = 1;
                        SummonEnemy(enemyInfo, &state, selectEnemy, 0);
                    }
                    return 1;
                }

start_wide_attack:
                /* Fallback attack. */
                *skillIndex = ENEMY_SKILL_1_WIDE_ATTACK;
                return HookContext::GetCurrent().OriginalFunction<uint32_t>(enemyInfo);
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(enemyInfo);
    }

    void InstallSkills()
    {
        if (!config::gConf.mSkills.active) return;

        InstallHookAtPattern(enemyTargettingSkills_Pattern, 0x0, (MITM_MODE), (uint32_t)EnemyTargettingSkills, 0);
        InstallHookAtPattern(allyTargettingSkills_Pattern, 0x0, (MITM_MODE), (uint32_t)AllyTargettingSkills, 0);

        InstallHookAtPattern(doesSkillTargetEnemy_Pattern, 0x0, (MITM_MODE), (uint32_t)DoesSkillTargetEnemy, 0);
        InstallHookAtPattern(doesSkillTargetAlly_Pattern, 0x0, (MITM_MODE), (uint32_t)DoesSkillTargetAlly, 0);
        InstallHookAtPattern(canSkillSelectEnemy_Pattern, 0x0, (MITM_MODE), (uint32_t)CanSkillSelectEnemy, 0);
        InstallHookAtPattern(canSkillSelectAlly_Pattern, 0x0, (MITM_MODE), (uint32_t)CanSkillSelectAlly, 0);
        InstallHookAtPattern(getSkillStatus_Pattern, 0x0, (MITM_MODE), (uint32_t)GetSkillStatus, 0);
        InstallHookAtPattern(isNotAutoSkill_Pattern, 0x0, (MITM_MODE), (uint32_t)IsNotAutoSkill, 0);

        InstallHookAtPattern(CalcDamage_Pattern, 0x0, (MITM_MODE), (uint32_t)ScientistPreOptimize, 0);
        InstallHookAtPattern(CalcHealing_Pattern, 0x0, (MITM_MODE), (uint32_t)ScientistPreOptimize, 0);
        InstallHookAtPattern(SpendSkillMP_Pattern, 0x24, (USE_LR_TO_RETURN), (uint32_t)ScientistOptimize, 0);

        InstallHookAtPattern(enemySkills1_Pattern, 0x0, (MITM_MODE), (uint32_t)EnemySlot1Skills, 0);
    }

} // namespace patches

} // namespace CTRPluginFramework
