#pragma once

#include "standalone/mimic_types.hpp"

namespace CTRPluginFramework {

namespace patches {

    uint32_t FighterFlee(ActorInfo* miiInfo, uint32_t* skillIndex, ActorInfo* enemyInfo);
    uint32_t FighterHitAll(ActorInfo* miiInfo, uint32_t* skillIndex);
    uint32_t FighterSingleHeal(ActorInfo* miiInfo, uint32_t* skillIndex, ActorInfo* targetMii, HelperInfo* helperInfo);
    uint32_t FighterStatusAll(ActorInfo* miiInfo, uint32_t* skillIndex, HelperInfo* helperInfo);

} // namespace patches

} // namespace CTRPluginFramework
