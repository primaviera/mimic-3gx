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

    int16_t gNoTarget = -1;

    uint32_t EnemyTargettingSkills(ActorInfo* miiInfo, uint32_t* skillIndex, ActorInfo* enemyInfo, ActorInfo* targetMii)
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
        return ORIG(uint32_t, miiInfo, skillIndex, enemyInfo, targetMii);
    }

    uint32_t AllyTargettingSkills(ActorInfo* miiInfo, uint32_t* skillIndex, uint32_t r2, ActorInfo* targetMii)
    {
        ActorInfo* actorArray[4] = { nullptr, nullptr, nullptr, nullptr };
        HelperInfo helperInfo = { 0, 4, &actorArray };
        switch (*skillIndex) {
            case SKILL_FIGHTER_11:
                if (!HasEnoughMPForSkill(miiInfo, skillIndex, 0))
                    return 0;
                if (!targetMii)
                    return 0;
                SetupSkillHelp(miiInfo, &helperInfo, targetMii);
                return FighterSingleHeal(miiInfo, skillIndex, targetMii, &helperInfo);

            case SKILL_FIGHTER_12:
                if (!HasEnoughMPForSkill(miiInfo, skillIndex, 0))
                    return 0;
                SetupSkillHelp(miiInfo, &helperInfo, 0);
                return FighterStatusAll(miiInfo, skillIndex, &helperInfo);
        }
        return ORIG(uint32_t, miiInfo, skillIndex, r2, targetMii);
    }

    uint32_t DoesSkillTargetEnemy(uintptr_t r0, uint32_t* skillIndex)
    {
        switch (*skillIndex) {
            case SKILL_FIGHTER_09:
                return 1;
        }
        return ORIG(uint32_t, r0, skillIndex);
    }

    uint32_t DoesSkillTargetAlly(uintptr_t r0, uint32_t* skillIndex)
    {
        switch (*skillIndex) {
            case SKILL_FIGHTER_11:
                return 1;
        }
        return ORIG(uint32_t, r0, skillIndex);
    }

    uint32_t CanSkillSelectEnemy(uintptr_t r0, uint32_t* skillIndex, uintptr_t r2)
    {
        switch (*skillIndex) {
            case SKILL_FIGHTER_09:
                /* Select any enemy(?) */
                *skillIndex = SKILL_FIGHTER_DOUBLE;
        }
        return ORIG(uint32_t, r0, skillIndex, r2);
    }

    uint32_t CanSkillSelectAlly(uintptr_t r0, uint32_t* skillIndex, uintptr_t r2)
    {
        switch (*skillIndex) {
            case SKILL_FIGHTER_11:
                /* Select anyone, including yourself. */
                *skillIndex = SKILL_PRIEST_CURE;
        }
        return ORIG(uint32_t, r0, skillIndex, r2);
    }

    void GetSkillStatus(uint32_t* outStatus, ActorInfo* miiInfo, uint32_t* skillIndex)
    {
        switch (*skillIndex) {
            case SKILL_FIGHTER_09:
            case SKILL_FIGHTER_10:
            case SKILL_FIGHTER_11:
            case SKILL_FIGHTER_12:
                *outStatus = SKILL_STATUS_ENABLE;
                if (-1 < miiInfo->mWeaponStatus) {
                    *outStatus = SKILL_STATUS_NO_WEAPON;
                    return;
                }
                if (!HasEnoughMPForSkill(miiInfo, skillIndex, 0)) {
                    *outStatus = SKILL_STATUS_NO_MP;
                    return;
                }
                return;
        }
        ORIG(uint32_t, outStatus, miiInfo, skillIndex);
    }

    uint32_t IsNotAutoSkill(uint32_t r0, uint32_t* skillIndex)
    {
        switch (*skillIndex) {
            case SKILL_SCIENTIST_09:
                return 0;
        }
        return ORIG(uint32_t, r0, skillIndex);
    }

#define CHECK_SKILL(str, id, slot)                                                                                     \
    if (!strcmp(skill, str)) {                                                                                         \
        enemyParam->mSkill##slot##Id = id;                                                                             \
        return;                                                                                                        \
    }

    void LoadEnemyInfoParams(EnemyParam* enemyParam, uint32_t r1, uint32_t row, uintptr_t r4)
    {
        switch (row) {
            /* Enemy Slot 1 Skill */
            case 11:
                const char* skill = *(const char**)(r4 + 0x4);
                CHECK_SKILL("LastMimit", ENEMY_SKILL_1_LAST_MIMIT, 1);
                CHECK_SKILL("CrashAttack", ENEMY_SKILL_1_CRASH_ATTACK, 1);
        }
        ORIG(void, enemyParam, r1, row, r4);
    }

#undef CHECK_SKILL

    uint32_t EnemySlot1Skills(ActorInfo* enemyInfo)
    {
        uint8_t* skillIndex = &enemyInfo->mEnemyParam->mSkill1Id;

        switch (*skillIndex) {
            /* Recreation of Boss Snurp's moves. */
            case ENEMY_SKILL_1_LAST_MIMIT: {
                float curHp = (float)enemyInfo->mBattleHelpers->GetCurHp(enemyInfo);
                float maxHp = (float)enemyInfo->mBattleHelpers->GetMaxHp(enemyInfo);
                float phase2Hp = maxHp * 0.5f;
                float phase3Hp = maxHp * 0.3f;
                uint32_t statuses[] = { FEELING_CRY, FEELING_EXCITE, FEELING_NIGHTMARE, FEELING_AGING, FEELING_STONE };

                if (!enemyInfo->unk_0x61 && curHp <= phase3Hp) {
                    enemyInfo->mEnemyParam->mActionsPerTurn = 3;
                    /* This field gets subtracted by 1 if it is not 0 at the end of every turn, I'm not sure if this is
                     * used by anything important, but it's perfect for our use case. */
                    enemyInfo->unk_0x61 = 4; /* Block enemy from inflicting random statuses for 4 more turns. */

                    _PlayBattleState(enemyInfo, "MagicLock", &gNoTarget);
                    for (uint32_t i = 0; i < GetNumberOfPartyMembers(enemyInfo->mBattleInfo); i++) {
                        uint32_t status = statuses[Utils::Random(0, (sizeof(statuses) / sizeof(statuses[0])) - 1)];

                        ActorInfo* selectMii = GetPartyMemberAtIndex(enemyInfo->mBattleInfo, i);
                        if (selectMii && IsPartyMemberAvailable(selectMii)) {
                            SetMiiFeeling(selectMii, &status, &selectMii->mBattleState->mTarget, 0);
                            _PlayBattleState(selectMii, "ErasedBananaEnd", &selectMii->mBattleState->mTarget);
                            _PlayBattleState(selectMii, "SkillResurrectHeal", &selectMii->mBattleState->mTarget);
                        }
                    }
                    return 1;
                }

                if (curHp <= phase2Hp) {
                    for (uint32_t i = 0; i < GetNumberOfEnemies(enemyInfo->mBattleInfo); i++) {
                        ActorInfo* selectEnemy = GetEnemyAtIndex(enemyInfo->mBattleInfo, i);
                        if (!selectEnemy || selectEnemy == enemyInfo)
                            continue;
                        /* Only summon enemies if all other enemies are dead, otherwise return 0. (Fallbacks to Wide
                         * Attack) */
                        if (!selectEnemy->mBattleHelpers->IsDead(selectEnemy)) {
                            return 0;
                        }
                        uint32_t state = 1; /* Not really sure what this value is for. */
                        SummonEnemy(enemyInfo, &state, selectEnemy, 0);
                    }
                    return 1;
                }
                return 0;
            }
            case ENEMY_SKILL_1_CRASH_ATTACK: {
                if (enemyInfo->unk_0x3F > 0) {
                    ActorInfo* selectMii = GetPartyMemberAtIndex(enemyInfo->mBattleInfo, enemyInfo->unk_0x3F - 1);
                    enemyInfo->unk_0x3F = 0;
                    if (!selectMii || (selectMii->mLockedOnStatus & 32) == 0 || selectMii->mBattleHelpers->IsDead(selectMii)) {
                        goto lock_on;
                    }
                    selectMii->mLockedOnStatus &= (255 - 32);
                    uint32_t dmgParams[0x10 / sizeof(uint32_t)];
                    CalcFixedDamageOrHealing(dmgParams, selectMii->mBattleHelpers->GetCurHp(selectMii) - 1, 1, 1);
                    _PlayBattleState(enemyInfo, "MagicLockHit", &gNoTarget);
                    const char* damageAnim;
                    if (selectMii->mFeeling == FEELING_VACANCY)
                        damageAnim = "DamageVacancyByMagic";
                    else
                        damageAnim = "DamageByMagic";
                    uint32_t removeLockState = 0x5a;
                    uint32_t unk_0x0 = 0x0;
                    PlayBattleState(selectMii->mBattleInfo, selectMii->mBattleState, &removeLockState, &unk_0x0, 0);
                    if (!IsPartyMemberAvailable(selectMii)) {
                        if (selectMii->mFeeling != FEELING_STONE
                            && selectMii->mFeeling != FEELING_NIGHTMARE
                            && selectMii->mFeeling != FEELING_SLEEP
                            && selectMii->mFeeling != FEELING_ICE)
                            _PlayBattleState(selectMii, "EscapeWaveCannon", &enemyInfo->mBattleState->mTarget);
                        return 1;
                    }
                    DamageMiiWithAnim(selectMii, enemyInfo, dmgParams, damageAnim, 1, 2);
                    return 1;
                } else {
                lock_on:
                    std::vector<uint32_t> miiIndexes;
                    for (uint32_t i = 0; i < GetNumberOfPartyMembers(enemyInfo->mBattleInfo); i++) {
                        ActorInfo* selectMii = GetPartyMemberAtIndex(enemyInfo->mBattleInfo, i);
                        if (selectMii && selectMii->mBattleHelpers->CanBeLockedOn(selectMii)
                            && (selectMii->mLockedOnStatus & 32) == 0 && IsPartyMemberAvailable(selectMii)) {
                            miiIndexes.push_back(i);
                        }
                    }
                    if (miiIndexes.size()) {
                        uint32_t selectId = miiIndexes[Utils::Random(0, miiIndexes.size() - 1)];
                        ActorInfo* selectMii = GetPartyMemberAtIndex(enemyInfo->mBattleInfo, selectId);
                        selectMii->mLockedOnStatus |= 32;
                        enemyInfo->unk_0x3F = selectId + 1;

                        uint32_t lockState = 0x59;
                        uint32_t unk_0x0 = 0x0;
                        _PlayBattleState(enemyInfo, "LockOn", &selectMii->mBattleState->mTarget);
                        PlayBattleState(selectMii->mBattleInfo, selectMii->mBattleState, &lockState, &unk_0x0, 0);
                        _PlayBattleState(selectMii, "ToLockOn", &enemyInfo->mBattleState->mTarget);
                        return 1;
                    }
                    return 0;
                }
            }
        }
        return ORIG(uint32_t, enemyInfo);
    }

    uint32_t EnemyPassives(ActorInfo* enemyInfo)
    {
        uint8_t* skillIndex = &enemyInfo->mEnemyParam->mSkill1Id;
        switch (*skillIndex) {
            /* BUG: For some reason this move desyncs in battle replays, not sure why this happens. */
            case ENEMY_SKILL_1_LAST_MIMIT:
                *skillIndex = ENEMY_SKILL_1_WIDE_ATTACK;
                auto res = ORIG(uint32_t, enemyInfo);
                *skillIndex = ENEMY_SKILL_1_LAST_MIMIT;
                return res;
        }
        return ORIG(uint32_t, enemyInfo);
    }

    uint32_t HideSurroundingEnemies(EnemyParam* enemyParam)
    {
        uint8_t skillIndex = enemyParam->mSkill1Id;
        switch (skillIndex) {
            case ENEMY_SKILL_1_LAST_MIMIT:
                return 1;
        }
        return ORIG(uint32_t, enemyParam);
    }

    /* The Juvenile Snurp's Crash Attack bypasses shields, however DamageMiiWithAnim doesn't account for this. Since
     * r0 is supposed to be a boolean, the game never assigns it a value larger than 1. In this case, we can set it
     * to 2 for our custom check where the shield is completely untouched. */
    void NAKED DamageMiiIgnoreShield()
    {
        asm("cmp r0, #2 \n"
            "bxeq lr \n"
            "tst r1, #8 \n"
            "bx lr");
    }

    /* If a Juvenile Snurp dies, reset the locked-on field and effect. */
    uint32_t HandleEnemyDeath(ActorInfo* miiInfo, ActorInfo* enemyInfo, uint32_t r3, uint32_t r4) {
        if (enemyInfo->mEnemyParam->mSkill1Id == ENEMY_SKILL_1_CRASH_ATTACK) {
            uint8_t miiIndex = enemyInfo->unk_0x3F;
            if (miiIndex > 0) {
                ActorInfo* selectMii = GetPartyMemberAtIndex(enemyInfo->mBattleInfo, miiIndex - 1);
                enemyInfo->unk_0x3F = 0;
                if (selectMii) {
                    selectMii->mLockedOnStatus &= (255 - 32);
                    uint32_t removeLockState = 0x5a;
                    uint32_t unk_0x0 = 0x0;
                    PlayBattleState(selectMii->mBattleInfo, selectMii->mBattleState, &removeLockState, &unk_0x0, 0);
                }
            }
        }
        return ORIG(uint32_t, miiInfo, enemyInfo, r3, r4);
    }

    void InstallSkills()
    {
        if (!config::gConf.mSkills.active)
            return;

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

        InstallHookAtPattern(loadEnemyInfoParams_Pattern, 0x0, (MITM_MODE), (uint32_t)LoadEnemyInfoParams, 0);
        InstallHookAtPattern(enemySkills1_Pattern, 0x0, (MITM_MODE), (uint32_t)EnemySlot1Skills, 0);
        InstallHookAtPattern(enemyPassives_Pattern, -0x20, (MITM_MODE), (uint32_t)EnemyPassives, 0);
        InstallHookAtPattern(hideSurroundingEnemies_Pattern, 0x0, (MITM_MODE), (uint32_t)HideSurroundingEnemies, 0);
        InstallHookAtPattern(handleEnemyDeath_Pattern, 0x0, (MITM_MODE), (uint32_t)HandleEnemyDeath, 0);

        InstallHookAtPattern(DamageMiiWithAnim_Pattern, 0x1C, (USE_LR_TO_RETURN), (uint32_t)DamageMiiIgnoreShield, 0);
    }

} // namespace patches

} // namespace CTRPluginFramework
