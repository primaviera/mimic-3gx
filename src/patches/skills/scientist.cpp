#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "func_ptrs.hpp"
#include "standalone/mimic_types.hpp"

#include "patches.hpp"
#include "patches/skills.hpp"
#include "patches/skills/scientist.hpp"

namespace CTRPluginFramework {

namespace patches {

    ActorInfo* targetMii;

    /* This skill was created by Kobazco, originally modded in the switch version, credits to them! */
    void ScientistPreOptimize(float arg1, uint32_t* outCalc, ActorInfo* miiInfo, uint32_t* skillIndex,
        ActorInfo* target, HelperInfo* helperInfo)
    {
        ORIG(void, arg1, outCalc, miiInfo, skillIndex, target, helperInfo);
        if (!CalcRandPercentage(miiInfo, 20) || GetSkillMPCost(miiInfo, skillIndex, 0) == 0)
            return;

        /* Check if miiInfo is not a traveler. */
        for (uint32_t i = 0; i < GetNumberOfPartyMembers(miiInfo->mBattleInfo); i++) {
            ActorInfo* selectMii = GetPartyMemberAtIndex(miiInfo->mBattleInfo, i);
            if (miiInfo == selectMii)
                goto check_scientist;
        }
        return;

    check_scientist:
        uint32_t optimizeSkillId = SKILL_SCIENTIST_09;
        for (uint32_t i = 0; i < GetNumberOfPartyMembers(miiInfo->mBattleInfo); i++) {
            ActorInfo* selectMii = GetPartyMemberAtIndex(miiInfo->mBattleInfo, i);
            if (selectMii && selectMii != miiInfo && HasEnoughMPForSkill(selectMii, &optimizeSkillId, 0)) {
                if (!IsPartyMemberAvailable(selectMii) || selectMii->mBattleHelpers->IsDead(selectMii))
                    return;
                if (CheckHateRelationship(selectMii, miiInfo, 0))
                    return;

                /* There is probably a way to create custom skill effects, which would be better than borrowing effects
                 * from other skills. */
                uint32_t cureCodeSkillId = SKILL_SCIENTIST_CURE_CODE;
                LoadSkillEffect(selectMii, &cureCodeSkillId, 1);
                PlaySkillEffect(selectMii);

                _PlayBattleState(selectMii, "SkillCureCodeStart", &gNoTarget);
                ShowCutIn(selectMii, &optimizeSkillId);
                SpendSkillMP(selectMii, &optimizeSkillId);

                /* BUG: The scientist says "Cure.exe!" instead of the actual skill name, this probably happens due to
                 * LoadSkillEffect but I don't really know how to fix this. */
                _PlayBattleState(selectMii, "SkillCureCode", &miiInfo->mBattleState->mTarget);

                if (CheckRockyReject(miiInfo)) {
                    StartRockyReject(miiInfo, selectMii, 0, 0);
                    RockyRejectAftermath(selectMii, miiInfo, 0);
                    return;
                }

                /* Here I wanted to play just the MP heal effect but it didn't work, so now the scientist heals 0 MP,
                 * basically the same thing. */
                uint8_t healParams[0x10];
                CalcFixedDamageOrHealing(healParams, 0, 0, 0);
                HealMiiMP(miiInfo, healParams, &selectMii->mBattleState->mTarget, 1);
                PlayHeartLikeEffect(miiInfo, 0x14);
                _PlayBattleState(miiInfo, "CureNormal", &selectMii->mBattleState->mTarget);
                UpdateLoveExp(miiInfo, selectMii, 5, 0);

                targetMii = miiInfo;
            }
        }
    }

    uint32_t ScientistOptimize(ActorInfo* miiInfo, uint32_t* skillIndex)
    {
        uint32_t cost = GetSkillMPCost(miiInfo, skillIndex, 0);
        if (miiInfo == targetMii) {
            targetMii = 0;
            return cost / 2;
        }
        return cost;
    }

} // namespace patches

} // namespace CTRPluginFramework
