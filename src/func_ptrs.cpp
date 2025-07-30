#include "func_ptrs.hpp"

namespace CTRPluginFramework {

SETUP_FUNC_PTR(sead_HashCRC32_calcHash, 0x0);
SETUP_FUNC_PTR(sead_Random_init, 0x0);
SETUP_FUNC_PTR(sead_Random_getU32, 0x0);
SETUP_FUNC_PTR(CalcRandPercentage, 0x0);

SETUP_FUNC_PTR(GetBGDataWithHash, 0x0);
SETUP_FUNC_PTR(GetBGMId, -0x4);

SETUP_FUNC_PTR(GetNumberOfPartyMembers, 0x0);
SETUP_FUNC_PTR(GetPartyMemberAtIndex, 0x0);
SETUP_FUNC_PTR(GetNumberOfEnemies, 0x0);
SETUP_FUNC_PTR(GetEnemyAtIndex, 0x0);

SETUP_FUNC_PTR(CanEnemyBeHit, 0x0);
SETUP_FUNC_PTR(IsPartyMemberAvailable, 0x0);

SETUP_FUNC_PTR(PlayBattleState, -0x10);
SETUP_FUNC_PTR(_PlayBattleState, 0x0);
SETUP_FUNC_PTR(LoadSkillEffect, 0x0);
SETUP_FUNC_PTR(PlaySkillEffect, 0x0);
SETUP_FUNC_PTR(SetMiiFeeling, 0x0);

SETUP_FUNC_PTR(HasEnoughMPForSkill, 0x0);
SETUP_FUNC_PTR(GetSkillMPCost, 0x0);
SETUP_FUNC_PTR(SpendSkillMP, 0x0);
SETUP_FUNC_PTR(ShowCutIn, 0x0);

SETUP_FUNC_PTR(SetupSkillHelp, 0x0);
SETUP_FUNC_PTR(PlayBattleStateForHelp, 0x0);

SETUP_FUNC_PTR(ShowBattleCaption, -0x1C);

SETUP_FUNC_PTR(CalcFixedDamageOrHealing, 0x0);

SETUP_FUNC_PTR(CalcDamage, 0x0);
SETUP_FUNC_PTR(SetupDamageParams, 0x0);
SETUP_FUNC_PTR(DamageEnemy, 0x0);
SETUP_FUNC_PTR(DamageMii, 0x0);
SETUP_FUNC_PTR(DamageMiiWithAnim, 0x0);

SETUP_FUNC_PTR(UnkDamageEnemyAfter, 0x0);
SETUP_FUNC_PTR(SetUnk0x58_ActorInfo, 0x0);

SETUP_FUNC_PTR(CalcHealing, 0x0);
SETUP_FUNC_PTR(SetupHealingParams, 0x0);
SETUP_FUNC_PTR(HealMiiHP, 0x0);
SETUP_FUNC_PTR(HealMiiMP, 0x0);

SETUP_FUNC_PTR(PlayHeartLikeEffect, 0x0);
SETUP_FUNC_PTR(UpdateLoveExp, 0x0);
SETUP_FUNC_PTR(PraiseOrCrankyAfterTurn, 0x0);
SETUP_FUNC_PTR(CheckHateRelationship, 0x0);

SETUP_FUNC_PTR(CheckRockyAgain, 0x0);
SETUP_FUNC_PTR(CheckRockyReject, 0x0);
SETUP_FUNC_PTR(StartRockyReject, 0x0);
SETUP_FUNC_PTR(RockyRejectAftermath, 0x0);

SETUP_FUNC_PTR(GetRandomEnemy, 0x0);
SETUP_FUNC_PTR(IsEnemyBoss, 0x0);
SETUP_FUNC_PTR(MakeEnemyFlee, 0x0);
SETUP_FUNC_PTR(SummonEnemy, 0x0);

} // namespace CTRPluginFramework
