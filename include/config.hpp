#pragma once

namespace CTRPluginFramework {

namespace config {

    struct Configuration {
        struct {
            bool active = true;
            bool showAutoBattle = true;
            bool saveMenuSelection = true;
        } mControllable;
        struct {
            bool active = false;
        } mRandomizer;
        struct {
            bool active = false;
            bool jobSkills = false;
        } mSkills;
    };

    extern Configuration gUserConfig;

    int Initialize(const std::string& path);

} // namespace config

} // namespace CTRPluginFramework
