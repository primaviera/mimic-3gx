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
        _PlayBattleState(miiInfo, "SkillDanceStart", &enemyInfo->mBattleState->mStateTarget);
        ShowCutIn(miiInfo, skillIndex);
        SpendSkillMP(miiInfo, skillIndex);

        _PlayBattleState(miiInfo, "SkillDance", &enemyInfo->mBattleState->mStateTarget);
        _PlayBattleState(enemyInfo, "ToFeelFever", &miiInfo->mBattleState->mStateTarget);
        MakeEnemyFlee(enemyInfo);
        return 1;
    }

    uint32_t FighterHitAll(ActorInfo* miiInfo, uint32_t* skillIndex)
    {
        uint32_t damageCalc[0x9]; // 0x24
        uint32_t damageParams[0x4]; // 0x10

        *skillIndex = SKILL_FIGHTER_DOUBLE; // I'm too lazy to make stats for these moves in skill.sarc
        CalcDamage(1.0f, damageCalc, miiInfo, skillIndex, 0, 0);
        *skillIndex = SKILL_FIGHTER_10;

        _PlayBattleState(miiInfo, "SkillDanceStart", &gInvalidTarget);
        ShowCutIn(miiInfo, skillIndex);
        SpendSkillMP(miiInfo, skillIndex);

        _PlayBattleState(miiInfo, "SkillDance", &gInvalidTarget);
        _PlayBattleState(miiInfo, "SkillArrowRainHit", &gInvalidTarget);
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
        uint32_t healCalc[0x9]; // 0x24
        uint32_t healParams[0x4]; // 0x10

        *skillIndex = SKILL_PRIEST_CURE; // I'm too lazy to make stats for these moves in skill.sarc
        CalcHealing(1.0f, healCalc, miiInfo, skillIndex, targetMii, helperInfo);
        *skillIndex = SKILL_FIGHTER_11;

        if (helperInfo->mNum) {
            _PlayBattleState(miiInfo, "SkillHelpStart", &gInvalidTarget);
            for (uint32_t i = 0; i < helperInfo->mNum; i++) {
                ActorInfo* selectMii = (*helperInfo->mMiiInfos)[i];
                PlayBattleStateForHelp(selectMii, miiInfo, i);
                if (i == 0) {
                    ShowBattleCaption(selectMii, 0, "SkillMagicHelpMsg");
                } else {
                    ShowBattleCaption(selectMii, 0, "SkillMagicHelpFollowMsg");
                }
                _PlayBattleState(selectMii, "SkillHelp", &miiInfo->mBattleState->mStateTarget);
            }
            _PlayBattleState(miiInfo, "SkillHelpEnd", &gInvalidTarget);
        }

        _PlayBattleState(miiInfo, "DefeatEnemyHelp", &miiInfo->mBattleState->mStateTarget);
        ShowCutIn(miiInfo, skillIndex);
        SpendSkillMP(miiInfo, skillIndex);

        SetupHealingParams(1.0f, healParams, targetMii, healCalc);
        HealMiiHP(targetMii, healParams, &miiInfo->mBattleState->mStateTarget, 1);

        if (miiInfo == targetMii) {
            _PlayBattleState(targetMii, "CureSelf", &miiInfo->mBattleState->mStateTarget);
        } else {
            _PlayBattleState(targetMii, "CureNormal", &miiInfo->mBattleState->mStateTarget);
        }
        return 1;
    }

    uint32_t FighterStatusAll(ActorInfo* miiInfo, uint32_t* skillIndex, HelperInfo* helperInfo)
    {
        uint32_t healCalc[0x9]; // 0x24
        uint32_t healParams[0x4]; // 0x10
        uint32_t status = FEELING_NORMAL;

        *skillIndex = SKILL_PRIEST_CURE3; // I'm too lazy to make stats for these moves in skill.sarc
        CalcHealing(1.0f, healCalc, miiInfo, skillIndex, 0, helperInfo);
        *skillIndex = SKILL_FIGHTER_12;

        if (helperInfo->mNum) {
            _PlayBattleState(miiInfo, "SkillHelpStart", &gInvalidTarget);
            for (uint32_t i = 0; i < helperInfo->mNum; i++) {
                ActorInfo* selectMii = (*helperInfo->mMiiInfos)[i];
                PlayBattleStateForHelp(selectMii, miiInfo, i);
                if (i == 0) {
                    ShowBattleCaption(selectMii, 0, "SkillMagicHelpMsg");
                } else {
                    ShowBattleCaption(selectMii, 0, "SkillMagicHelpFollowMsg");
                }
                _PlayBattleState(selectMii, "SkillHelp", &miiInfo->mBattleState->mStateTarget);
            }
            _PlayBattleState(miiInfo, "SkillHelpEnd", &gInvalidTarget);
        }

        _PlayBattleState(miiInfo, "DefeatEnemyHelp", &miiInfo->mBattleState->mStateTarget);
        ShowCutIn(miiInfo, skillIndex);
        SpendSkillMP(miiInfo, skillIndex);

        _PlayBattleState(miiInfo, "SkillWhistleCureStart", &gInvalidTarget);
        for (uint32_t i = 0; i < GetNumberOfPartyMembers(miiInfo->mBattleInfo); i++) {
            status = Utils::Random(0, 23);
            if (status == FEELING_FACELESS)
                status = FEELING_NORMAL;
            ActorInfo* selectMii = GetPartyMemberAtIndex(miiInfo->mBattleInfo, i);
            if (selectMii && IsPartyMemberAvailable(selectMii)) {
                SetupHealingParams(1.0f, healParams, selectMii, healCalc);
                if (Utils::Random(0, 1)) {
                    HealMiiHP(selectMii, healParams, &miiInfo->mBattleState->mStateTarget, 1);
                } else {
                    HealMiiMP(selectMii, healParams, &miiInfo->mBattleState->mStateTarget, 1);
                }
                SetMiiFeeling(selectMii, &status, &selectMii->mBattleState->mStateTarget, 0);
                _PlayBattleState(selectMii, "ErasedBananaEnd", &selectMii->mBattleState->mStateTarget);
                _PlayBattleState(selectMii, "DogfightEndAttackHitL", &selectMii->mBattleState->mStateTarget);
            }
        }
        _PlayBattleState(miiInfo, "SkillWhistleCureEnd", &gInvalidTarget);
        return 1;
    }

} // namespace patches

} // namespace CTRPluginFramework
