#pragma once

namespace CTRPluginFramework
{

namespace config
{

struct st_randomizer {
    bool active;
};

struct st_skills {
    bool active;
};

extern st_randomizer randomizer;
extern st_skills skills;

int init(const std::string& path);

} // namespace config

} // namespace CTRPluginFramework
