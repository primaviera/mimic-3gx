#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "func_ptrs.hpp"
#include "hacks/skills.hpp"
#include "hacks/custom_skills/warrior.hpp"

#include "mimic_types.hpp"

namespace CTRPluginFramework {

namespace hacks {

    uint32_t warrior_flee(uintptr_t mii_info, uint32_t* skill_index, uintptr_t enemy_info) {
        play_battle_state(mii_info, "SkillDanceStart", (int16_t*)(*(uintptr_t*)(enemy_info + 0x4) + 0x60));
        show_cut_in(mii_info, skill_index);
        spend_skill_mp(mii_info, skill_index);
        
        play_battle_state(mii_info, "SkillDance", (int16_t*)(*(uintptr_t*)(enemy_info + 0x4) + 0x60));
        play_battle_state(enemy_info, "ToFeelFever", (int16_t*)(*(uintptr_t*)(mii_info + 0x4) + 0x60));
        enemy_flee(enemy_info);
        return 1;
    }

    uint32_t warrior_hit_all(uintptr_t mii_info, uint32_t* skill_index) {
        uint32_t* damage_calc = (uint32_t*)malloc(0x24);
        uint32_t* damage_params = (uint32_t*)malloc(0x10);

        play_battle_state(mii_info, "SkillDanceStart", state_no_target);
        show_cut_in(mii_info, skill_index);
        spend_skill_mp(mii_info, skill_index);

        *skill_index = 0;
        calc_damage(1.0f, damage_calc, mii_info, skill_index, 0, 0);

        play_battle_state(mii_info, "SkillDance", state_no_target);
        play_battle_state(mii_info, "SkillArrowRainHit", state_no_target);
        for (uint32_t i = 0; i < get_number_of_enemies(*(uintptr_t*)(mii_info + 0x8)); i++) {
            uintptr_t select_enemy = get_enemy_at_index(*(uintptr_t*)(mii_info + 0x8), i);
            if (select_enemy && can_enemy_be_hit(select_enemy)) {
                setup_damage_params(1.0f, damage_params, select_enemy, damage_calc);
                damage_enemy(select_enemy, mii_info, damage_params, 1);
            }
        }

        free(damage_calc);
        free(damage_params);
        return 1;
    }

    uint32_t warrior_single_heal(uintptr_t mii_info, uint32_t* skill_index, uintptr_t target_mii) {
        uint32_t* heal_calc = (uint32_t*)malloc(0x24);
        uint32_t* healing_params = (uint32_t*)malloc(0x10);

        play_battle_state(mii_info, "DefeatEnemyHelp", (int16_t*)(*(uintptr_t*)(mii_info + 0x4) + 0x60));
        show_cut_in(mii_info, skill_index);
        spend_skill_mp(mii_info, skill_index);

        *skill_index = 26; // For testing purposes
        calc_healing(1.0f, heal_calc, mii_info, skill_index, target_mii, 0);
        setup_healing_params(1.0f, healing_params, target_mii, heal_calc);
        heal_mii_hp(target_mii, healing_params, (int16_t*)(*(uintptr_t*)(mii_info + 0x4) + 0x60), 1);

        if (mii_info == target_mii) {
            play_battle_state(target_mii, "CureSelf", (int16_t*)(*(uintptr_t*)(mii_info + 0x4) + 0x60));
        } else {
            play_battle_state(target_mii, "CureNormal", (int16_t*)(*(uintptr_t*)(mii_info + 0x4) + 0x60));
        }

        free(heal_calc);
        free(healing_params);
        return 1;
    }

    uint32_t warrior_status_all(uintptr_t mii_info, uint32_t* skill_index) {
        uint32_t* heal_calc = (uint32_t*)malloc(0x24);
        uint32_t* healing_params = (uint32_t*)malloc(0x10);
        uint32_t* status = (uint32_t*)malloc(0x4);

        play_battle_state(mii_info, "DefeatEnemyHelp", (int16_t*)(*(uintptr_t*)(mii_info + 0x4) + 0x60));
        show_cut_in(mii_info, skill_index);
        spend_skill_mp(mii_info, skill_index);

        *skill_index = 26; // For testing purposes
        calc_healing(1.0f, heal_calc, mii_info, skill_index, 0, 0);

        play_battle_state(mii_info, "SkillWhistleCureStart", state_no_target);
        for (uint32_t i = 0; i < get_number_of_party_members(*(uintptr_t*)(mii_info + 0x8)); i++) {
            *status = Utils::Random(0, 23);
            if (*status == 18) *status =+ 1;
            if (*status == 23) *status -= 1;
            uintptr_t select_mii = get_party_member_at_index(*(uintptr_t*)(mii_info + 0x8), i);
            if (select_mii && is_party_member_available(select_mii)) {
                setup_healing_params(1.0f, healing_params, select_mii, heal_calc);
                if (Utils::Random(0, 1)) {
                    heal_mii_hp(select_mii, healing_params, (int16_t*)(*(uintptr_t*)(mii_info + 0x4) + 0x60), 1);
                } else {
                    heal_mii_mp(select_mii, healing_params, (int16_t*)(*(uintptr_t*)(mii_info + 0x4) + 0x60), 1);
                }
                set_mii_feeling(select_mii, status, (int16_t*)(*(uintptr_t*)(select_mii + 0x4) + 0x60), 0);
                play_battle_state(select_mii, "ErasedBananaEnd", (int16_t*)(*(uintptr_t*)(select_mii + 0x4) + 0x60));
                play_battle_state(select_mii, "DogfightEndAttackHitL", (int16_t*)(*(uintptr_t*)(select_mii + 0x4) + 0x60));
            }
        }
        play_battle_state(mii_info, "SkillWhistleCureEnd", state_no_target);

        free(heal_calc);
        free(healing_params);
        free(status);
        return 1;
    }

} // namespace hacks

} // namespace CTRPluginFramework
