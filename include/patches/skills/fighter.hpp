#pragma once

#include "standalone/mimic_types.hpp"

namespace CTRPluginFramework {

namespace patches {

uint32_t FighterFlee(uintptr_t miiInfo, uint32_t* skillIndex, uintptr_t enemyInfo);
uint32_t FighterHitAll(uintptr_t miiInfo, uint32_t* skillIndex);
uint32_t FighterSingleHeal(uintptr_t miiInfo, uint32_t* skillIndex, uintptr_t targetMii);
uint32_t FighterStatusAll(uintptr_t miiInfo, uint32_t* skillIndex);

}

}
