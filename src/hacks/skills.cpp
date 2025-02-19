#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "config.hpp"
#include "func_ptrs.hpp"
#include "hacks.hpp"
#include "patterns.hpp"

namespace CTRPluginFramework {

namespace hacks {

    extern "C" uint32_t gleeful;

    uint32_t enemy_targetting_skills(uintptr_t mii_info, uint32_t* skill_index, uintptr_t enemy_info, uint32_t r3)
    {
        switch (*skill_index) {
            /*
             * PoC: Custom Skill for Warrior
             */
            case 8:
                if (!has_enough_mp_for_skill(mii_info, skill_index, 0))
                    return 0;
                if (!enemy_info)
                    return 0;
                play_battle_state(mii_info, "SkillDanceStart", (uint16_t*)(*(uintptr_t*)(enemy_info + 0x4) + 0x60));
                show_cut_in(mii_info, skill_index);
                spend_skill_mp(mii_info, skill_index);
                play_battle_state(mii_info, "SkillDance", (uint16_t*)(*(uintptr_t*)(enemy_info + 0x4) + 0x60));
                play_battle_state(enemy_info, "ToFeelFever", (uint16_t*)(*(uintptr_t*)(mii_info + 0x4) + 0x60));
                enemy_flee(enemy_info);
                return 1;
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(mii_info, skill_index, enemy_info, r3);
    }

    uint32_t ally_targetting_skills(uintptr_t mii_info, uint32_t* skill_index, uintptr_t target_mii, uint32_t r3)
    {
        switch (*skill_index) {
            /*
             * WIP: Custom skill for Warrior that targets allies
             */
            case 9:
                if (!has_enough_mp_for_skill(mii_info, skill_index, 0))
                    return 0;
                spend_skill_mp(mii_info, skill_index);
                return 1;
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(mii_info, skill_index, target_mii, r3);
    }

    uint32_t does_skill_target_enemy(uintptr_t r0, uint32_t* skill_index)
    {
        switch (*skill_index) {
            case 8:
                return 1;
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(r0, skill_index);
    }

    uint32_t does_skill_target_ally(uintptr_t r0, uint32_t* skill_index)
    {
        switch (*skill_index) {
            case 9:
                return 1;
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(r0, skill_index);
    }

    uint32_t can_skill_select_enemy(uintptr_t r0, uint32_t* skill_index, uintptr_t r2)
    {
        switch (*skill_index) {
            case 8:
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
            case 9:
                *skill_index = 26;
        }
        return HookContext::GetCurrent().OriginalFunction<uint32_t>(r0, skill_index, r2);
    }

    void get_skill_status(uint32_t* out_status, uint32_t r1, uint32_t* skill_index) {
        switch (*skill_index) {
            case 8:
            case 9:
                *out_status = 0;
                return;
        }
        HookContext::GetCurrent().OriginalFunction<uint32_t>(out_status, r1, skill_index);
    }

    // 'Playful Antics' makes the target gleeful
    uint32_t gleeful = 9;
    void NAKED frisk_gleeful()
    {
        asm volatile("push {r0-r4, lr} \n"
                     "mov r0, r4 \n"
                     "ldr r1, =gleeful \n"
                     "ldr r2, [r5, #4] \n"
                     "ldrh r2, [r2, #0x60] \n"
                     "mov r3, #0 \n"
                     "ldr r4, =set_mii_feeling \n"
                     "ldr r4, [r4] \n"
                     "blx r4 \n"
                     "pop {r0-r4, pc}");
    }

    // 'Sweet Whispers' has a chance to make the enemy flee
    void whisper_flee(uintptr_t enemy_info, uintptr_t mii_info, int r2, int r3, const char* caption_hit,
        const char* caption_already, int r6)
    {
        if (!strcmp(caption_hit, "SkillWhisperHitCpt")) {
            if (Utils::Random(0, 100) < 20) {
                enemy_flee(enemy_info);
                return;
            }
        }
        CTRPluginFramework::HookContext::GetCurrent().OriginalFunction<void>(enemy_info, mii_info, r2, r3, caption_hit,
            caption_already, r6);
    }

    void install_skills()
    {
        if (!config::skills.active) return;

        install_hook(enemy_targetting_skills_pattern, 0x0, (MITM_MODE), (uint32_t)enemy_targetting_skills, 0);
        install_hook(ally_targetting_skills_pattern, 0x0, (MITM_MODE), (uint32_t)ally_targetting_skills, 0);
        install_hook(does_skill_target_enemy_pattern, 0x0, (MITM_MODE), (uint32_t)does_skill_target_enemy, 0);
        install_hook(does_skill_target_ally_pattern, 0x0, (MITM_MODE), (uint32_t)does_skill_target_ally, 0);
        install_hook(can_skill_select_enemy_pattern, 0x0, (MITM_MODE), (uint32_t)can_skill_select_enemy, 0);
        install_hook(can_skill_select_ally_pattern, 0x0, (MITM_MODE), (uint32_t)can_skill_select_ally, 0);
        install_hook(get_skill_status_pattern, 0x0, (MITM_MODE), (uint32_t)get_skill_status, 0);

        install_hook(frisk_pattern, 0x40, (WRAP_SUB), (uint32_t)frisk_gleeful, 0);
        install_hook(vacancy_pattern, 0x0, (MITM_MODE), (uint32_t)whisper_flee, 0);
    }

} // namespace hacks

} // namespace CTRPluginFramework
