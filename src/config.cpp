#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include <tomlplusplus/toml.hpp>

#include "config.hpp"
#include "logger.hpp"

#include "config_toml.h"

namespace CTRPluginFramework
{

namespace config
{

st_randomizer randomizer;
st_skills skills;

void load(const toml::table& table)
{
    randomizer.active = table["randomizer"]["active"].value_or(false);

    skills.active = table["skills"]["active"].value_or(false);
}

int init(const std::string& path)
{
    File config_file;
    void* config_buffer;
    uint32_t config_size;

    if (!File::Open(config_file, path, File::READ)) {
        config_size = config_file.GetSize();
        config_buffer = malloc(config_size);
        config_file.Read(config_buffer, config_size);
    } else {
        config_size = config_toml_size;
        config_buffer = malloc(config_size);
        memcpy(config_buffer, config_toml, config_size);
    }
    ((char*)config_buffer)[config_size - 1] = '\0'; // This is terrible

    toml::parse_result res = toml::parse((const char*)config_buffer);
    free(config_buffer);
    if (!res) {
        logger::write(res.error().description().data());
        logger::write("\n");
        return 1;
    }

    load(res.table());

    return 0;
}

} // namespace config

} // namespace CTRPluginFramework
