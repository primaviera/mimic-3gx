#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include <cmath>

#include "func_ptrs.hpp"
#include "standalone/mimic_types.hpp"

#include "patches/skills.hpp"
#include "patches/skills/scientist.hpp"

namespace CTRPluginFramework {

namespace patches {

    bool is_optimize_active = false;
    uintptr_t target_mii = 0;

    /* This skill was created by Kobazco, originally modded in the switch version, credits to them! */
    void ScientistPreOptimize(float s0, uint32_t* out_calc, uintptr_t mii_info, uint32_t* skill_index, uintptr_t target, HelperInfo* helping_miis) {
        HookContext::GetCurrent().OriginalFunction<void>(s0, out_calc, mii_info, skill_index, target, helping_miis);
        if (Utils::Random(0, 5) || GetSkillMPCost(mii_info, skill_index, 0) == 0)
            return;

        // Check if mii_info is a party member (excluding travelers)
        for (uint32_t i = 0; i < GetNumberOfPartyMembers(*(uintptr_t*)(mii_info + 0x8)); i++) {
            uintptr_t select_mii = GetPartyMemberAtIndex(*(uintptr_t*)(mii_info + 0x8), i);
            if (mii_info == select_mii)
                goto check_scientist;
        }
        return;

check_scientist:
        uint32_t optimize_skill = SKILL_SCIENTIST_09;
        for (uint32_t i = 0; i < GetNumberOfPartyMembers(*(uintptr_t*)(mii_info + 0x8)); i++) {
            uintptr_t select_mii = GetPartyMemberAtIndex(*(uintptr_t*)(mii_info + 0x8), i);
            if (!is_optimize_active && select_mii && select_mii != mii_info && HasEnoughMPForSkill(select_mii, &optimize_skill, 0)) {
                uint32_t cure_code_skill = SKILL_SCIENTIST_CURE_CODE;
                LoadSkillEffect(select_mii, &cure_code_skill, 1); // Use Cure.exe effect
                PlaySkillEffect(select_mii);

                _PlayBattleState(select_mii, "SkillCureCodeStart", &gInvalidTarget);
                ShowCutIn(select_mii, &optimize_skill);
                SpendSkillMP(select_mii, &optimize_skill);

                // Here the scientist says "Cure.exe" instead of the actual skill name
                // Don't know how to fix that
                _PlayBattleState(select_mii, "SkillCureCode", (int16_t*)(*(uintptr_t*)(mii_info + 0x4) + 0x60));
                _PlayBattleState(mii_info, "AvoidFeelCutInReady", &gInvalidTarget);
                PlayHeartLikeEffect(mii_info, 0x14);
                UpdateLoveExp(mii_info, select_mii, 5, 0);

                target_mii = mii_info;
                is_optimize_active = true;
            }
        }
    }

    uint32_t ScientistOptimize(uintptr_t mii_info, uint32_t* skill_index) {
        uint32_t cost = GetSkillMPCost(mii_info, skill_index, 0);
        if (is_optimize_active && mii_info == target_mii) {
            is_optimize_active = false;
            target_mii = 0;
            return (uint32_t)std::round(cost / 2);
        }
        return cost;
    }

} // namespace hacks

} // namespace CTRPluginFramework
