#pragma once

namespace CTRPluginFramework {

namespace config {

    struct st_randomizer {
        bool active;
    };

    struct st_skills {
        bool active;
    };

    struct st_controllable {
        bool active;
    };

    extern st_randomizer randomizer;
    extern st_skills skills;
    extern st_controllable controllable;

    int init(const std::string& path);

} // namespace config

} // namespace CTRPluginFramework
