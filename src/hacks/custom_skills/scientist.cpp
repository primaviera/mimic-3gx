#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include <cmath>

#include "func_ptrs.hpp"
#include "hacks/skills.hpp"
#include "hacks/custom_skills/scientist.hpp"

#include "mimic_types.hpp"

namespace CTRPluginFramework {

namespace hacks {

    bool is_optimize_active = false;
    uintptr_t target_mii = 0;

    // This is a "backport" of a custom skill that Kobazco created for the switch version of Miitopia, this idea is theirs!
    // Scientist - Optimize (Cut the MP cost of an ally's skill by half)
    void scientist_pre_optimize(float s0, uint32_t* out_calc, uintptr_t mii_info, uint32_t* skill_index, uintptr_t target, helping_mii_handle* helping_miis) {
        HookContext::GetCurrent().OriginalFunction<void>(s0, out_calc, mii_info, skill_index, target, helping_miis);
        if (Utils::Random(0, 5) || get_skill_mp_cost(mii_info, skill_index, 0) == 0)
            return;

        uint32_t optimize_skill = SKILL_SCIENTIST_09;
        for (uint32_t i = 0; i < get_number_of_party_members(*(uintptr_t*)(mii_info + 0x8)); i++) {
            uintptr_t select_mii = get_party_member_at_index(*(uintptr_t*)(mii_info + 0x8), i);
            if (!is_optimize_active && select_mii && select_mii != mii_info && has_enough_mp_for_skill(select_mii, &optimize_skill, 0)) {
                uint32_t cure_code_skill = SKILL_SCIENTIST_CURE_CODE;
                load_skill_effect(select_mii, &cure_code_skill, 1); // Use Cure.exe effect
                play_skill_effect(select_mii);

                play_battle_state(select_mii, "SkillCureCodeStart", &battle_state_no_target);
                show_cut_in(select_mii, &optimize_skill);
                spend_skill_mp(select_mii, &optimize_skill);

                // Here the scientist says "Cure.exe" instead of the actual skill name
                // Don't know how to fix that
                play_battle_state(select_mii, "SkillCureCode", (int16_t*)(*(uintptr_t*)(mii_info + 0x4) + 0x60));
                play_battle_state(mii_info, "AvoidFeelCutInReady", &battle_state_no_target);
                play_heart_like_effect(mii_info, 0x14);
                update_love_exp(mii_info, select_mii, 5, 0);

                target_mii = mii_info;
                is_optimize_active = true;
            }
        }
    }

    uint32_t scientist_optimize(uintptr_t mii_info, uint32_t* skill_index) {
        uint32_t cost = get_skill_mp_cost(mii_info, skill_index, 0);
        if (is_optimize_active && mii_info == target_mii) {
            is_optimize_active = false;
            target_mii = 0;
            return (uint32_t)std::round(cost / 2);
        }
        return cost;
    }

} // namespace hacks

} // namespace CTRPluginFramework
