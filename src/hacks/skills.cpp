#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "config.hpp"
#include "func_ptrs.hpp"
#include "hacks/hacks.hpp"
#include "hacks/skills.hpp"
#include "patterns.hpp"

#include "hacks/custom_skills/warrior.hpp"

#include "mimic_types.hpp"

namespace CTRPluginFramework {

namespace hacks {

    int16_t* state_no_target = (int16_t*)malloc(0x2);

    uint32_t enemy_targetting_skills(uintptr_t mii_info, uint32_t* skill_index, uintptr_t enemy_info, uintptr_t target_mii)
    {
        switch (*skill_index) {
            /* PoC: Custom skill for Warrior that targets an enemy */
            case SKILL_FIGHTER_09:
                if (!has_enough_mp_for_skill(mii_info, skill_index, 0))
                    return 0;
                if (!enemy_info)
                    return 0;
                return warrior_flee(mii_info, skill_index, enemy_info);

            /* WIP: Custom skill for Warrior that targets all enemies */
            case SKILL_FIGHTER_10:
                if (!has_enough_mp_for_skill(mii_info, skill_index, 0))
                    return 0;
                return warrior_hit_all(mii_info, skill_index);
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(mii_info, skill_index, enemy_info, target_mii);
    }

    uint32_t ally_targetting_skills(uintptr_t mii_info, uint32_t* skill_index, uint32_t r2, uintptr_t target_mii)
    {
        switch (*skill_index) {
            /* PoC: Custom skill for Warrior that targets an ally */
            case SKILL_FIGHTER_11:
                if (!has_enough_mp_for_skill(mii_info, skill_index, 0))
                    return 0;
                if (!target_mii)
                    return 0;
                return warrior_single_heal(mii_info, skill_index, target_mii);

            /* PoC: Custom skill for Warrior that targets all allies */
            case SKILL_FIGHTER_12:
                if (!has_enough_mp_for_skill(mii_info, skill_index, 0))
                    return 0;
                return warrior_status_all(mii_info, skill_index);
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(mii_info, skill_index, r2, target_mii);
    }

    uint32_t does_skill_target_enemy(uintptr_t r0, uint32_t* skill_index)
    {
        switch (*skill_index) {
            case SKILL_FIGHTER_09:
                return 1;
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(r0, skill_index);
    }

    uint32_t does_skill_target_ally(uintptr_t r0, uint32_t* skill_index)
    {
        switch (*skill_index) {
            case SKILL_FIGHTER_11:
                return 1;
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(r0, skill_index);
    }

    uint32_t can_skill_select_enemy(uintptr_t r0, uint32_t* skill_index, uintptr_t r2)
    {
        switch (*skill_index) {
            case SKILL_FIGHTER_09:
                *skill_index = 0;
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(r0, skill_index, r2);
    }

    /*
     * 26 - Cure (Select allies and yourself)
     * 122 - Flower Power (Select allies, but not yourself)
     * 31 - Resurrect (Select fallen allies)
     */
    uint32_t can_skill_select_ally(uintptr_t r0, uint32_t* skill_index, uintptr_t r2)
    {
        switch (*skill_index) {
            case SKILL_FIGHTER_11:
                *skill_index = 26;
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(r0, skill_index, r2);
    }

    void get_skill_status(uint32_t* out_status, uintptr_t mii_info, uint32_t* skill_index) {
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
                if (!has_enough_mp_for_skill(mii_info, skill_index, 0)) {
                    *out_status = SKILL_STATUS_NO_MP;
                    return;
                }
                return;
        }
        HookContext::GetCurrent().OriginalFunction<uint32_t>(out_status, mii_info, skill_index);
    }

    void install_skills()
    {
        if (!config::skills.active) return;
        *state_no_target = -1;

        install_hook(enemy_targetting_skills_pattern, 0x0, (MITM_MODE), (uint32_t)enemy_targetting_skills, 0);
        install_hook(ally_targetting_skills_pattern, 0x0, (MITM_MODE), (uint32_t)ally_targetting_skills, 0);
        install_hook(does_skill_target_enemy_pattern, 0x0, (MITM_MODE), (uint32_t)does_skill_target_enemy, 0);
        install_hook(does_skill_target_ally_pattern, 0x0, (MITM_MODE), (uint32_t)does_skill_target_ally, 0);
        install_hook(can_skill_select_enemy_pattern, 0x0, (MITM_MODE), (uint32_t)can_skill_select_enemy, 0);
        install_hook(can_skill_select_ally_pattern, 0x0, (MITM_MODE), (uint32_t)can_skill_select_ally, 0);
        install_hook(get_skill_status_pattern, 0x0, (MITM_MODE), (uint32_t)get_skill_status, 0);
    }

} // namespace hacks

} // namespace CTRPluginFramework
