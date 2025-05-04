#pragma once

#include "standalone/mimic_types.hpp"

namespace CTRPluginFramework {

namespace patches {

uint32_t FighterFlee(uintptr_t mii_info, uint32_t* skill_index, uintptr_t enemy_info);
uint32_t FighterHitAll(uintptr_t mii_info, uint32_t* skill_index);
uint32_t FighterSingleHeal(uintptr_t mii_info, uint32_t* skill_index, uintptr_t target_mii);
uint32_t FighterStatusAll(uintptr_t mii_info, uint32_t* skill_index);

}

}
