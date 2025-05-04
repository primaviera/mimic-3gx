#pragma once

namespace CTRPluginFramework {

namespace config {

    struct Data {
        struct Randomizer {
            bool active = false;
        };
        struct Skills {
            bool active = false;
        };

        Randomizer mRandomizer;
        Skills mSkills;
    };

    extern Data gConf;

    int Initialize(const std::string& path);

} // namespace config

} // namespace CTRPluginFramework
