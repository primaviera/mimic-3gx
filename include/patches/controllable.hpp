#pragma once

namespace CTRPluginFramework {

namespace patches {

    struct CustomMiiInfo {
        int32_t mSelectedMainEntry = 0;
        int32_t mSelectedEnemyEntry = 0;
        int32_t mSelectedPartyEntry = 0;
        int32_t mSelectedItemEntry = 0;
        int8_t mSelectedMagicPage = 0;
        int32_t mSelectedMagicEntry = 0;
    };
    extern CustomMiiInfo gCustomMiiInfo[4];

    void InstallControllable();

} // namespace patches

} // namespace CTRPluginFramework
