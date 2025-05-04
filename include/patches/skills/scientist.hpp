#pragma once

#include "standalone/mimic_types.hpp"

namespace CTRPluginFramework {

namespace patches {

void ScientistPreOptimize(float s0, uint32_t* outCalc, uintptr_t miiInfo, uint32_t* skillIndex, uintptr_t target, HelperInfo* helperInfo);
uint32_t ScientistOptimize(uintptr_t miiInfo, uint32_t* skillIndex);

}

}
