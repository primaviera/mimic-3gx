#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include <cmath>

#include "func_ptrs.hpp"
#include "standalone/mimic_types.hpp"

#include "patches/skills.hpp"
#include "patches/skills/scientist.hpp"

namespace CTRPluginFramework {

namespace patches {

    bool isOptimizeActive = false;
    ActorInfo* targetMii;

    /* This skill was created by Kobazco, originally modded in the switch version, credits to them! */
    void ScientistPreOptimize(float s0, uint32_t* outCalc, ActorInfo* miiInfo, uint32_t* skillIndex, ActorInfo* target,
        HelperInfo* helperInfo)
    {
        HookContext::GetCurrent().OriginalFunction<void>(s0, outCalc, miiInfo, skillIndex, target, helperInfo);
        if (!CalcRandPercentage(miiInfo, 20) || GetSkillMPCost(miiInfo, skillIndex, 0) == 0)
            return;

        /* Check if MiiInfo is not a traveler. */
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
            if (!isOptimizeActive && selectMii && selectMii != miiInfo
                && HasEnoughMPForSkill(selectMii, &optimizeSkillId, 0)) {
                uint32_t cureCodeSkillId = SKILL_SCIENTIST_CURE_CODE;
                /* Use Cure.exe effects. */
                LoadSkillEffect(selectMii, &cureCodeSkillId, 1);
                PlaySkillEffect(selectMii);

                _PlayBattleState(selectMii, "SkillCureCodeStart", &gNoTarget);
                ShowCutIn(selectMii, &optimizeSkillId);
                SpendSkillMP(selectMii, &optimizeSkillId);

                /* BUG: The scientist says "Cure.exe!" instead of the actual skill name, this probably happens due to
                 * LoadSkillEffect but I don't really know how to fix this. */
                _PlayBattleState(selectMii, "SkillCureCode", &miiInfo->mBattleState->mTarget);
                _PlayBattleState(miiInfo, "AvoidFeelCutInReady", &gNoTarget);
                PlayHeartLikeEffect(miiInfo, 0x14);
                UpdateLoveExp(miiInfo, selectMii, 5, 0);

                targetMii = miiInfo;
                isOptimizeActive = true;
            }
        }
    }

    uint32_t ScientistOptimize(ActorInfo* miiInfo, uint32_t* skillIndex)
    {
        uint32_t cost = GetSkillMPCost(miiInfo, skillIndex, 0);
        if (isOptimizeActive && miiInfo == targetMii) {
            isOptimizeActive = false;
            targetMii = 0;
            return (uint32_t)std::round(cost / 2);
        }
        return cost;
    }

} // namespace patches

} // namespace CTRPluginFramework
