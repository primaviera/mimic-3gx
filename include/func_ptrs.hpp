#pragma once

#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "patterns.hpp"

#include "mimic_types.hpp"

namespace CTRPluginFramework {

// https://github.com/shadowninja108/s2-freecam

#define APPEND_IMPL(x, y) x##y
#define APPEND(x, y) APPEND_IMPL(x, y)

#define _FUNC_PREFIX(name) mimic::name

#define _FUNC_TYPE(name) APPEND(_FUNC_PREFIX(name), _t)

#define FUNC_PTR(name, ...)                   \
    namespace mimic {                         \
        using APPEND(name, _t) = __VA_ARGS__; \
    }                                         \
    extern "C" _FUNC_TYPE(name) name

// Hardcoded pattern name for now
#define SETUP_FUNC_PTR(name, offset) \
    _FUNC_TYPE(name)                 \
    name = (_FUNC_TYPE(name))(Utils::Search(0x00100000, 0x00709000, name##_pattern) + offset);

FUNC_PTR(sead_HashCRC32_calcHash, uint32_t (*)(const void*, uint32_t));
FUNC_PTR(CalcRandPercentage, uint32_t (*)(uintptr_t, uint32_t));

FUNC_PTR(GetNumberOfPartyMembers, uint32_t (*)(uintptr_t));
FUNC_PTR(GetPartyMemberAtIndex, uintptr_t (*)(uintptr_t, uint32_t));
FUNC_PTR(GetNumberOfEnemies, uint32_t (*)(uintptr_t));
FUNC_PTR(GetEnemyAtIndex, uintptr_t (*)(uintptr_t, uint32_t));

FUNC_PTR(CanEnemyBeHit, uint32_t (*)(uintptr_t));
FUNC_PTR(IsPartyMemberAvailable, uint32_t (*)(uintptr_t));
FUNC_PTR(GetRandomEnemy, uintptr_t (*)(uintptr_t, uint32_t*, int32_t, uint32_t));

FUNC_PTR(PlayState, void (*)(uintptr_t, uintptr_t, uint32_t*, uint32_t*, uint32_t));
FUNC_PTR(PlayBattleState, void (*)(uintptr_t, const char*, int16_t*));
FUNC_PTR(LoadSkillEffect, void (*)(uintptr_t, uint32_t*, uint32_t));
FUNC_PTR(PlaySkillEffect, void (*)(uintptr_t));
FUNC_PTR(SetMiiFeeling, void (*)(uintptr_t, uint32_t*, int16_t*, uint32_t));

FUNC_PTR(HasEnoughMPForSkill, uint32_t (*)(uintptr_t, uint32_t*, uint32_t));
FUNC_PTR(GetSkillMPCost, uint32_t (*)(uintptr_t, uint32_t*, uint32_t));
FUNC_PTR(SpendSkillMP, void (*)(uintptr_t, uint32_t*));
FUNC_PTR(ShowCutIn, void (*)(uintptr_t, uint32_t*));
FUNC_PTR(SetupSkillHelp, void (*)(uintptr_t, helping_mii_handle*, uint32_t));

FUNC_PTR(CalcDamage, void (*)(float, uint32_t*, uintptr_t, uint32_t*, uintptr_t, helping_mii_handle*));
FUNC_PTR(SetupDamageParams, void (*)(float, uint32_t*, uintptr_t, uint32_t*));
FUNC_PTR(DamageEnemy, uint32_t (*)(uintptr_t, uintptr_t, uint32_t*, uint32_t));

FUNC_PTR(CalcHealing, void (*)(float, uint32_t*, uintptr_t, uint32_t*, uintptr_t, helping_mii_handle*));
FUNC_PTR(SetupHealingParams, void (*)(float, uint32_t*, uintptr_t, uint32_t*));
FUNC_PTR(HealMiiHP, void (*)(uintptr_t, uint32_t*, int16_t*, uint32_t));
FUNC_PTR(HealMiiMP, void (*)(uintptr_t, uint32_t*, int16_t*, uint32_t));

FUNC_PTR(PlayHeartLikeEffect, void (*)(uintptr_t, uint32_t));
FUNC_PTR(UpdateLoveExp, void (*)(uintptr_t, uintptr_t, int32_t, uint32_t));

FUNC_PTR(SummonEnemy, uint32_t (*)(uintptr_t, uint32_t*, uintptr_t, uint32_t));
FUNC_PTR(MakeEnemyFlee, void (*)(uintptr_t));

} // namespace CTRPluginFramework
