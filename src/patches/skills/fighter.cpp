#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "func_ptrs.hpp"
#include "standalone/mimic_types.hpp"

#include "patches/skills.hpp"
#include "patches/skills/fighter.hpp"

namespace CTRPluginFramework {

namespace patches {

    uint32_t FighterFlee(uintptr_t miiInfo, uint32_t* skillIndex, uintptr_t enemyInfo) {
        _PlayBattleState(miiInfo, "SkillDanceStart", (int16_t*)(*(uintptr_t*)(enemyInfo + 0x4) + 0x60));
        ShowCutIn(miiInfo, skillIndex);
        SpendSkillMP(miiInfo, skillIndex);
        
        _PlayBattleState(miiInfo, "SkillDance", (int16_t*)(*(uintptr_t*)(enemyInfo + 0x4) + 0x60));
        _PlayBattleState(enemyInfo, "ToFeelFever", (int16_t*)(*(uintptr_t*)(miiInfo + 0x4) + 0x60));
        MakeEnemyFlee(enemyInfo);
        return 1;
    }

    uint32_t FighterHitAll(uintptr_t miiInfo, uint32_t* skillIndex) {
        uint32_t* damageCalc = (uint32_t*)malloc(0x24);
        uint32_t* damageParams = (uint32_t*)malloc(0x10);

        *skillIndex = SKILL_FIGHTER_DOUBLE; // I'm too lazy to make stats for these moves in skill.sarc
        CalcDamage(1.0f, damageCalc, miiInfo, skillIndex, 0, 0);
        *skillIndex = SKILL_FIGHTER_10;

        _PlayBattleState(miiInfo, "SkillDanceStart", &gInvalidTarget);
        ShowCutIn(miiInfo, skillIndex);
        SpendSkillMP(miiInfo, skillIndex);

        _PlayBattleState(miiInfo, "SkillDance", &gInvalidTarget);
        _PlayBattleState(miiInfo, "SkillArrowRainHit", &gInvalidTarget);
        for (uint32_t i = 0; i < GetNumberOfEnemies(*(uintptr_t*)(miiInfo + 0x8)); i++) {
            uintptr_t selectEnemy = GetEnemyAtIndex(*(uintptr_t*)(miiInfo + 0x8), i);
            if (selectEnemy && CanEnemyBeHit(selectEnemy)) {
                SetupDamageParams(1.0f, damageParams, selectEnemy, damageCalc);
                DamageEnemy(selectEnemy, miiInfo, damageParams, 1);
            }
        }

        free(damageCalc);
        free(damageParams);
        return 1;
    }

    uint32_t FighterSingleHeal(uintptr_t miiInfo, uint32_t* skillIndex, uintptr_t targetMii) {
        uint32_t* healCalc = (uint32_t*)malloc(0x24);
        uint32_t* healParams = (uint32_t*)malloc(0x10);

        *skillIndex = SKILL_PRIEST_CURE; // I'm too lazy to make stats for these moves in skill.sarc
        CalcHealing(1.0f, healCalc, miiInfo, skillIndex, targetMii, 0);
        *skillIndex = SKILL_FIGHTER_11;

        _PlayBattleState(miiInfo, "DefeatEnemyHelp", (int16_t*)(*(uintptr_t*)(miiInfo + 0x4) + 0x60));
        ShowCutIn(miiInfo, skillIndex);
        SpendSkillMP(miiInfo, skillIndex);

        SetupHealingParams(1.0f, healParams, targetMii, healCalc);
        HealMiiHP(targetMii, healParams, (int16_t*)(*(uintptr_t*)(miiInfo + 0x4) + 0x60), 1);

        if (miiInfo == targetMii) {
            _PlayBattleState(targetMii, "CureSelf", (int16_t*)(*(uintptr_t*)(miiInfo + 0x4) + 0x60));
        } else {
            _PlayBattleState(targetMii, "CureNormal", (int16_t*)(*(uintptr_t*)(miiInfo + 0x4) + 0x60));
        }

        free(healCalc);
        free(healParams);
        return 1;
    }

    uint32_t FighterStatusAll(uintptr_t miiInfo, uint32_t* skillIndex) {
        uint32_t* healCalc = (uint32_t*)malloc(0x24);
        uint32_t* healParams = (uint32_t*)malloc(0x10);
        uint32_t status = FEELING_NORMAL;

        *skillIndex = SKILL_PRIEST_CURE3; // I'm too lazy to make stats for these moves in skill.sarc
        CalcHealing(1.0f, healCalc, miiInfo, skillIndex, 0, 0);
        *skillIndex = SKILL_FIGHTER_12;

        _PlayBattleState(miiInfo, "DefeatEnemyHelp", (int16_t*)(*(uintptr_t*)(miiInfo + 0x4) + 0x60));
        ShowCutIn(miiInfo, skillIndex);
        SpendSkillMP(miiInfo, skillIndex);

        _PlayBattleState(miiInfo, "SkillWhistleCureStart", &gInvalidTarget);
        for (uint32_t i = 0; i < GetNumberOfPartyMembers(*(uintptr_t*)(miiInfo + 0x8)); i++) {
            status = Utils::Random(0, 23);
            if (status == FEELING_FACELESS) status =+ 1;
            uintptr_t selectMii = GetPartyMemberAtIndex(*(uintptr_t*)(miiInfo + 0x8), i);
            if (selectMii && IsPartyMemberAvailable(selectMii)) {
                SetupHealingParams(1.0f, healParams, selectMii, healCalc);
                if (Utils::Random(0, 1)) {
                    HealMiiHP(selectMii, healParams, (int16_t*)(*(uintptr_t*)(miiInfo + 0x4) + 0x60), 1);
                } else {
                    HealMiiMP(selectMii, healParams, (int16_t*)(*(uintptr_t*)(miiInfo + 0x4) + 0x60), 1);
                }
                SetMiiFeeling(selectMii, &status, (int16_t*)(*(uintptr_t*)(selectMii + 0x4) + 0x60), 0);
                _PlayBattleState(selectMii, "ErasedBananaEnd", (int16_t*)(*(uintptr_t*)(selectMii + 0x4) + 0x60));
                _PlayBattleState(selectMii, "DogfightEndAttackHitL", (int16_t*)(*(uintptr_t*)(selectMii + 0x4) + 0x60));
            }
        }
        _PlayBattleState(miiInfo, "SkillWhistleCureEnd", &gInvalidTarget);

        free(healCalc);
        free(healParams);
        return 1;
    }

} // namespace patches

} // namespace CTRPluginFramework
