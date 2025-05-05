#pragma once

#include "standalone/mimic_types.hpp"

namespace CTRPluginFramework {

namespace patches {

    void ScientistPreOptimize(float s0, uint32_t* outCalc, ActorInfo* miiInfo, uint32_t* skillIndex, ActorInfo* target,
        HelperInfo* helperInfo);
    uint32_t ScientistOptimize(ActorInfo* miiInfo, uint32_t* skillIndex);

} // namespace patches

} // namespace CTRPluginFramework
