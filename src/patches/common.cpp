#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "func_ptrs.hpp"
#include "standalone/mimic_types.hpp"
#include "standalone/patterns.hpp"

#include "patches.hpp"
#include "patches/common.hpp"

namespace CTRPluginFramework {

namespace patches {

    void HandleBattleProgress(uintptr_t arg1, uint32_t state)
    {
        switch (state) {
            /* Battle started. */
            case 1:
            /* Battle ended by winning, escaping or losing. */
            case 16:
            case 17:
            case 18:
                /* Reset arrays meant to track extra information about party members and enemies. */
                for (uint32_t i = 0; i < 4; i++)
                    gCustomMiiInfo[i] = CustomMiiInfo();
                for (uint32_t i = 0; i < 8; i++)
                    gCustomEnemyInfo[i] = CustomEnemyInfo();
        }
        ORIG(void, arg1, state);
    }

    void InstallCommon()
    {
        logger::Write("[Common] Installing...\n");

        InstallHookAtPattern(handleBattleProgress_Pattern, 0x0, (MITM_MODE), (uint32_t)HandleBattleProgress, 0);
    }

} // namespace patches

} // namespace CTRPluginFramework
