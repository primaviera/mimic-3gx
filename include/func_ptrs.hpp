#pragma once

#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "patterns.hpp"

#include "mimic_types.hpp"

namespace CTRPluginFramework
{

// https://github.com/shadowninja108/s2-freecam

#define APPEND_IMPL(x, y) x##y
#define APPEND(x, y) APPEND_IMPL(x, y)

#define _FUNC_PREFIX(name) mimic::name

#define _FUNC_TYPE(name) APPEND(_FUNC_PREFIX(name), _t)

#define FUNC_PTR(name, ...)               \
    namespace mimic                       \
    {                                     \
    using APPEND(name, _t) = __VA_ARGS__; \
    }                                     \
    extern "C" _FUNC_TYPE(name) name

// Hardcoded pattern name for now
#define SETUP_FUNC_PTR(name, offset) \
    _FUNC_TYPE(name)                 \
    name = (_FUNC_TYPE(name))(       \
        Utils::Search(0x00100000, 0x00709000, name##_pattern) + offset);

FUNC_PTR(crc32, uint32_t (*)(const void*, uint32_t));
FUNC_PTR(rand_percentage, uint32_t (*)(uintptr_t, uint32_t));

FUNC_PTR(get_number_of_party_members, uint32_t (*)(uintptr_t));
FUNC_PTR(get_party_member_at_index, uintptr_t (*)(uintptr_t, uint32_t));
FUNC_PTR(get_number_of_enemies, uint32_t (*)(uintptr_t));
FUNC_PTR(get_enemy_at_index, uintptr_t (*)(uintptr_t, uint32_t));

FUNC_PTR(can_enemy_be_hit, uint32_t (*)(uintptr_t));
FUNC_PTR(is_party_member_available, uint32_t (*)(uintptr_t));
FUNC_PTR(get_random_enemy, uintptr_t (*)(uintptr_t, uint32_t*, int32_t, uint32_t));

FUNC_PTR(play_state, void (*)(uintptr_t, uintptr_t, uint32_t*, uint32_t*, uint32_t));
FUNC_PTR(play_battle_state, void (*)(uintptr_t, const char*, int16_t*));
FUNC_PTR(load_skill_effect, void (*)(uintptr_t, uint32_t*, uint32_t));
FUNC_PTR(play_skill_effect, void (*)(uintptr_t));
FUNC_PTR(set_mii_feeling, void (*)(uintptr_t, uint32_t*, int16_t*, uint32_t));

FUNC_PTR(has_enough_mp_for_skill, uint32_t (*)(uintptr_t, uint32_t*, uint32_t));
FUNC_PTR(get_skill_mp_cost, uint32_t (*)(uintptr_t, uint32_t*, uint32_t));
FUNC_PTR(spend_skill_mp, void (*)(uintptr_t, uint32_t*));
FUNC_PTR(show_cut_in, void (*)(uintptr_t, uint32_t*));
FUNC_PTR(setup_skill_help, void (*)(uintptr_t, helping_mii_handle*, uint32_t));

FUNC_PTR(calc_damage, void (*)(float, uint32_t*, uintptr_t, uint32_t*, uintptr_t, helping_mii_handle*));
FUNC_PTR(setup_damage_params, void (*)(float, uint32_t*, uintptr_t, uint32_t*));
FUNC_PTR(damage_enemy, uint32_t (*)(uintptr_t, uintptr_t, uint32_t*, uint32_t));

FUNC_PTR(calc_healing, void (*)(float, uint32_t*, uintptr_t, uint32_t*, uintptr_t, helping_mii_handle*));
FUNC_PTR(setup_healing_params, void (*)(float, uint32_t*, uintptr_t, uint32_t*));
FUNC_PTR(heal_mii_hp, void (*)(uintptr_t, uint32_t*, int16_t*, uint32_t));
FUNC_PTR(heal_mii_mp, void (*)(uintptr_t, uint32_t*, int16_t*, uint32_t));

FUNC_PTR(play_heart_like_effect, void (*)(uintptr_t, uint32_t));
FUNC_PTR(update_love_exp, void (*)(uintptr_t, uintptr_t, int32_t, uint32_t));

FUNC_PTR(summon_enemy, uint32_t (*)(uintptr_t, uint32_t*, uintptr_t, uint32_t));
FUNC_PTR(enemy_flee, void (*)(uintptr_t));

} // namespace CTRPluginFramework
