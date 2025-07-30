#pragma once

namespace CTRPluginFramework {

namespace patches {

    struct EnemyEntry {
        const char* name;
        uint16_t level;
    };

    void InstallRandomizer();

}

} // namespace CTRPluginFramework
