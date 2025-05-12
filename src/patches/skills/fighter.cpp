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
                uint32_t* res = DamageEnemy(selectEnemy, miiInfo, damageParams, 1);
                /* Deal damage to all enemies at the same time. */
                UnkDamageEnemyAfter(miiInfo->mBattleInfo, res);
                SetUnk0x58_ActorInfo(miiInfo, 1);
            }
        }
        PraiseOrCrankyAfterTurn(miiInfo);
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

        _PlayBattleState(miiInfo, "GameClear", &gNoTarget);
        ShowCutIn(miiInfo, skillIndex);
        SpendSkillMP(miiInfo, skillIndex);

        SetupHealingParams(1.0f, healParams, targetMii, healCalc);
        HealMiiHP(targetMii, healParams, &miiInfo->mBattleState->mTarget, 1);

        if (miiInfo == targetMii) {
            _PlayBattleState(targetMii, "CureSelf", &miiInfo->mBattleState->mTarget);
        } else {
            if (CheckRockyReject(targetMii)) {
                StartRockyReject(targetMii, miiInfo, 0, 0);
                RockyRejectAftermath(miiInfo, targetMii, 0);
                return 1;
            }
            PlayHeartLikeEffect(targetMii, 0x14);
            if (CheckHateRelationship(targetMii, miiInfo, 0)) {
                _PlayBattleState(targetMii, "CureHate", &miiInfo->mBattleState->mTarget);
            } else {
                _PlayBattleState(targetMii, "CureNormal", &miiInfo->mBattleState->mTarget);
            }
            UpdateLoveExp(targetMii, miiInfo, 5, 0);
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

        _PlayBattleState(miiInfo, "GameClear", &gNoTarget);
        ShowCutIn(miiInfo, skillIndex);
        SpendSkillMP(miiInfo, skillIndex);

        /* TODO: Change some of the battle states here, the camera looks very wonky. */
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
