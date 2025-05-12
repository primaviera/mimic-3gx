#pragma once

#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "standalone/mimic_types.hpp"
#include "standalone/patterns.hpp"

namespace CTRPluginFramework {

/* https://github.com/shadowninja108/s2-freecam */

#define APPEND_IMPL(x, y) x##y
#define APPEND(x, y) APPEND_IMPL(x, y)

#define _FUNC_PREFIX(name) mimic::name

#define _FUNC_TYPE(name) APPEND(_FUNC_PREFIX(name), _t)

#define FUNC_PTR(name, ...)                                                                                            \
    namespace mimic {                                                                                                  \
        using APPEND(name, _t) = __VA_ARGS__;                                                                          \
    }                                                                                                                  \
    extern _FUNC_TYPE(name) name

#define SETUP_FUNC_PTR(name, offset)                                                                                   \
    _FUNC_TYPE(name)                                                                                                   \
    name = (_FUNC_TYPE(name))(Utils::Search(0x00100000, 0x00709000, name##_Pattern) + offset);

FUNC_PTR(sead_HashCRC32_calcHash, uint32_t (*)(const void*, uint32_t));
FUNC_PTR(sead_Random_init, void (*)(sead::Random*));
FUNC_PTR(sead_Random_getU32, uint32_t (*)(sead::Random*));
FUNC_PTR(CalcRandPercentage, bool (*)(ActorInfo*, uint32_t));

FUNC_PTR(GetNumberOfPartyMembers, uint32_t (*)(BattleInfo*));
FUNC_PTR(GetPartyMemberAtIndex, ActorInfo* (*)(BattleInfo*, uint32_t));
FUNC_PTR(GetNumberOfEnemies, uint32_t (*)(BattleInfo*));
FUNC_PTR(GetEnemyAtIndex, ActorInfo* (*)(BattleInfo*, uint32_t));

FUNC_PTR(CanEnemyBeHit, uint32_t (*)(ActorInfo*));
FUNC_PTR(IsPartyMemberAvailable, uint32_t (*)(ActorInfo*));

FUNC_PTR(PlayBattleState, void (*)(BattleInfo*, BattleState*, uint32_t*, uint32_t*, uint32_t));
FUNC_PTR(_PlayBattleState, void (*)(ActorInfo*, const char*, int16_t*));
FUNC_PTR(LoadSkillEffect, void (*)(ActorInfo*, uint32_t*, uint32_t));
FUNC_PTR(PlaySkillEffect, void (*)(ActorInfo*));
FUNC_PTR(SetMiiFeeling, void (*)(ActorInfo*, uint32_t*, int16_t*, uint32_t));

FUNC_PTR(HasEnoughMPForSkill, uint32_t (*)(ActorInfo*, uint32_t*, uint32_t));
FUNC_PTR(GetSkillMPCost, uint32_t (*)(ActorInfo*, uint32_t*, uint32_t));
FUNC_PTR(SpendSkillMP, void (*)(ActorInfo*, uint32_t*));
FUNC_PTR(ShowCutIn, void (*)(ActorInfo*, uint32_t*));

FUNC_PTR(SetupSkillHelp, void (*)(ActorInfo*, HelperInfo*, ActorInfo*));
FUNC_PTR(PlayBattleStateForHelp, void (*)(ActorInfo*, ActorInfo*, uint32_t));

FUNC_PTR(ShowBattleCaption, void (*)(ActorInfo*, uint32_t, const char*));

FUNC_PTR(CalcFixedDamageOrHealing, void (*)(uint32_t*, uint32_t, uint32_t, uint32_t));

FUNC_PTR(CalcDamage, void (*)(float, uint32_t*, ActorInfo*, uint32_t*, ActorInfo*, HelperInfo*));
FUNC_PTR(SetupDamageParams, void (*)(float, uint32_t*, ActorInfo*, uint32_t*));
FUNC_PTR(DamageEnemy, uint32_t* (*)(ActorInfo*, ActorInfo*, uint32_t*, uint32_t));

FUNC_PTR(UnkDamageEnemyAfter, void (*)(BattleInfo*, uint32_t*));
FUNC_PTR(SetUnk0x58_ActorInfo, void (*)(ActorInfo*, uint32_t));

FUNC_PTR(CalcHealing, void (*)(float, uint32_t*, ActorInfo*, uint32_t*, ActorInfo*, HelperInfo*));
FUNC_PTR(SetupHealingParams, void (*)(float, uint32_t*, ActorInfo*, uint32_t*));
FUNC_PTR(HealMiiHP, void (*)(ActorInfo*, uint32_t*, int16_t*, uint32_t));
FUNC_PTR(HealMiiMP, void (*)(ActorInfo*, uint32_t*, int16_t*, uint32_t));

FUNC_PTR(PlayHeartLikeEffect, void (*)(ActorInfo*, uint32_t));
FUNC_PTR(UpdateLoveExp, void (*)(ActorInfo*, ActorInfo*, int32_t, uint32_t));
FUNC_PTR(PraiseOrCrankyAfterTurn, void (*)(ActorInfo*));
FUNC_PTR(CheckHateRelationship, bool (*)(ActorInfo*, ActorInfo*, uint32_t));

FUNC_PTR(CheckRockyAgain, bool (*)(ActorInfo*, uint32_t*));
FUNC_PTR(CheckRockyReject, bool (*)(ActorInfo*));
FUNC_PTR(StartRockyReject, void (*)(ActorInfo*, ActorInfo*, uint32_t, uint32_t));
FUNC_PTR(RockyRejectAftermath, void (*)(ActorInfo*, ActorInfo*, uint32_t));

FUNC_PTR(GetRandomEnemy, ActorInfo* (*)(BattleInfo*, uint32_t*, int32_t, uint32_t));
FUNC_PTR(IsEnemyBoss, bool (*)(ActorInfo*));
FUNC_PTR(MakeEnemyFlee, void (*)(ActorInfo*));
FUNC_PTR(SummonEnemy, uint32_t (*)(ActorInfo*, uint32_t*, ActorInfo*, uint32_t));

} // namespace CTRPluginFramework
