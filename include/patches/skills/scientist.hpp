#pragma once

#include "standalone/mimic_types.hpp"

namespace CTRPluginFramework {

namespace patches {

void ScientistPreOptimize(float s0, uint32_t* out_calc, uintptr_t mii_info, uint32_t* skill_index, uintptr_t target, HelperInfo* helping_miis);
uint32_t ScientistOptimize(uintptr_t mii_info, uint32_t* skill_index);

}

}
