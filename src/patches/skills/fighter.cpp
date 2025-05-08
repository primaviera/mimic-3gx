#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "func_ptrs.hpp"
#include "standalone/mimic_types.hpp"

#include "patches/skills.hpp"
#include "patches/skills/fighter.hpp"

namespace CTRPluginFramework {

namespace patches {

    uint32_t FighterFlee(ActorInfo* miiInfo, uint32_t* skillIndex, ActorInfo* enemyInfo)
    {
        _PlayBattleState(miiInfo, "SkillDanceStart", &enemyInfo->mBattleState->mTarget);
        ShowCutIn(miiInfo, skillIndex);
        SpendSkillMP(miiInfo, skillIndex);

        _PlayBattleState(miiInfo, "SkillDance", &enemyInfo->mBattleState->mTarget);
        _PlayBattleState(enemyInfo, "ToFeelFever", &miiInfo->mBattleState->mTarget);
        MakeEnemyFlee(enemyInfo);
        return 1;
    }

    uint32_t FighterHitAll(ActorInfo* miiInfo, uint32_t* skillIndex)
    {
        uint32_t damageCalc[0x24 / sizeof(uint32_t)];
        uint32_t damageParams[0x10 / sizeof(uint32_t)];

        /* Use Jump Slash stats for now. */
        uint32_t tmpSkill = SKILL_FIGHTER_DOUBLE;
        CalcDamage(1.0f, damageCalc, miiInfo, &tmpSkill, 0, 0);

        _PlayBattleState(miiInfo, "SkillDanceStart", &gNoTarget);
        ShowCutIn(miiInfo, skillIndex);
        SpendSkillMP(miiInfo, skillIndex);

        _PlayBattleState(miiInfo, "SkillDance", &gNoTarget);
        _PlayBattleState(miiInfo, "SkillArrowRainHit", &gNoTarget);
        for (uint32_t i = 0; i < GetNumberOfEnemies(miiInfo->mBattleInfo); i++) {
            ActorInfo* selectEnemy = GetEnemyAtIndex(miiInfo->mBattleInfo, i);
            if (selectEnemy && CanEnemyBeHit(selectEnemy)) {
                SetupDamageParams(1.0f, damageParams, selectEnemy, damageCalc);
                DamageEnemy(selectEnemy, miiInfo, damageParams, 1);
            }
        }
        return 1;
    }

    uint32_t FighterSingleHeal(ActorInfo* miiInfo, uint32_t* skillIndex, ActorInfo* targetMii, HelperInfo* helperInfo)
    {
        uint32_t healCalc[0x24 / sizeof(uint32_t)];
        uint32_t healParams[0x10 / sizeof(uint32_t)];

        uint32_t tmpSkill = SKILL_PRIEST_CURE;
        CalcHealing(1.0f, healCalc, miiInfo, &tmpSkill, targetMii, helperInfo);

        if (helperInfo->mNum) {
            _PlayBattleState(miiInfo, "SkillHelpStart", &gNoTarget);
            for (uint32_t i = 0; i < helperInfo->mNum; i++) {
                ActorInfo* selectMii = (*helperInfo->mMiiInfos)[i];
                PlayBattleStateForHelp(selectMii, miiInfo, i);
                if (i == 0) {
                    ShowBattleCaption(selectMii, 0, "SkillMagicHelpMsg");
                } else {
                    ShowBattleCaption(selectMii, 0, "SkillMagicHelpFollowMsg");
                }
                _PlayBattleState(selectMii, "SkillHelp", &miiInfo->mBattleState->mTarget);
            }
            _PlayBattleState(miiInfo, "SkillHelpEnd", &gNoTarget);
        }

        _PlayBattleState(miiInfo, "DefeatEnemyHelp", &miiInfo->mBattleState->mTarget);
        ShowCutIn(miiInfo, skillIndex);
        SpendSkillMP(miiInfo, skillIndex);

        SetupHealingParams(1.0f, healParams, targetMii, healCalc);
        HealMiiHP(targetMii, healParams, &miiInfo->mBattleState->mTarget, 1);

        if (miiInfo == targetMii) {
            _PlayBattleState(targetMii, "CureSelf", &miiInfo->mBattleState->mTarget);
        } else {
            _PlayBattleState(targetMii, "CureNormal", &miiInfo->mBattleState->mTarget);
        }
        return 1;
    }

    uint32_t FighterStatusAll(ActorInfo* miiInfo, uint32_t* skillIndex, HelperInfo* helperInfo)
    {
        uint32_t healCalc[0x24 / sizeof(uint32_t)];
        uint32_t healParams[0x10 / sizeof(uint32_t)];
        uint32_t status = FEELING_NORMAL;

        uint32_t tmpSkill = SKILL_PRIEST_CURE3;
        CalcHealing(1.0f, healCalc, miiInfo, &tmpSkill, 0, helperInfo);

        if (helperInfo->mNum) {
            _PlayBattleState(miiInfo, "SkillHelpStart", &gNoTarget);
            for (uint32_t i = 0; i < helperInfo->mNum; i++) {
                ActorInfo* selectMii = (*helperInfo->mMiiInfos)[i];
                PlayBattleStateForHelp(selectMii, miiInfo, i);
                if (i == 0) {
                    ShowBattleCaption(selectMii, 0, "SkillMagicHelpMsg");
                } else {
                    ShowBattleCaption(selectMii, 0, "SkillMagicHelpFollowMsg");
                }
                _PlayBattleState(selectMii, "SkillHelp", &miiInfo->mBattleState->mTarget);
            }
            _PlayBattleState(miiInfo, "SkillHelpEnd", &gNoTarget);
        }

        _PlayBattleState(miiInfo, "DefeatEnemyHelp", &miiInfo->mBattleState->mTarget);
        ShowCutIn(miiInfo, skillIndex);
        SpendSkillMP(miiInfo, skillIndex);

        _PlayBattleState(miiInfo, "SkillWhistleCureStart", &gNoTarget);
        for (uint32_t i = 0; i < GetNumberOfPartyMembers(miiInfo->mBattleInfo); i++) {
            status = Utils::Random(0, 23);
            if (status == FEELING_FACELESS)
                status = FEELING_NORMAL;
            ActorInfo* selectMii = GetPartyMemberAtIndex(miiInfo->mBattleInfo, i);
            if (selectMii && IsPartyMemberAvailable(selectMii)) {
                SetupHealingParams(1.0f, healParams, selectMii, healCalc);
                if (CalcRandPercentage(miiInfo, 50)) {
                    HealMiiHP(selectMii, healParams, &miiInfo->mBattleState->mTarget, 1);
                } else {
                    HealMiiMP(selectMii, healParams, &miiInfo->mBattleState->mTarget, 1);
                }
                SetMiiFeeling(selectMii, &status, &selectMii->mBattleState->mTarget, 0);
                _PlayBattleState(selectMii, "ErasedBananaEnd", &selectMii->mBattleState->mTarget);
                _PlayBattleState(selectMii, "DogfightEndAttackHitL", &selectMii->mBattleState->mTarget);
            }
        }
        _PlayBattleState(miiInfo, "SkillWhistleCureEnd", &gNoTarget);
        return 1;
    }

} // namespace patches

} // namespace CTRPluginFramework
