#pragma once

#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "patterns.hpp"

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

#define SETUP_FUNC_PTR(name, offset) \
    _FUNC_TYPE(name)                 \
    name = (_FUNC_TYPE(name))(       \
        Utils::Search(0x00100000, 0x00709000, name##_pattern) + offset);

FUNC_PTR(crc32, uint32_t (*)(const void*, uint32_t));
FUNC_PTR(rand_percentage, uint32_t (*)(uintptr_t, uint32_t));

FUNC_PTR(play_battle_state, void (*)(uintptr_t, const char*, uint16_t));
FUNC_PTR(set_mii_feeling, void (*)(uintptr_t, uint32_t*, uint16_t, uint32_t));
FUNC_PTR(enemy_flee, void (*)(uintptr_t));
FUNC_PTR(show_ptcl, void (*)(uintptr_t, const char*, uint32_t, uint32_t, uint32_t));

} // namespace CTRPluginFramework
