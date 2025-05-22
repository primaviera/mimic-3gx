#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include <inih/ini.h>

#include "config.hpp"
#include "logger.hpp"

namespace CTRPluginFramework {

namespace config {

    config::Data gConf;

    bool IniGetBoolean(const char* value, bool defaultValue)
    {
        if (!strcmp(value, "true"))
            return true;
        else if (!strcmp(value, "false"))
            return false;
        return defaultValue;
    }

    int IniHandler(void* user, const char* section, const char* name, const char* value)
    {
        config::Data* userConfig = (config::Data*)user;

#define MATCH(s, n) !strcmp(section, s) && !strcmp(name, n)
        if (MATCH("randomizer", "active")) {
            userConfig->mRandomizer.active = IniGetBoolean(value, false);
        } else if (MATCH("skills", "active")) {
            userConfig->mSkills.active = IniGetBoolean(value, false);
        } else {
            return 0;
        }
#undef MATCH
        return 1;
    }

    int Initialize(const std::string& path)
    {
        File configFile;
        void* configBuffer;
        uint32_t configSize;

        /* If this fails, use default config. */
        if (File::Open(configFile, path, File::READ))
            return 0;

        /* Allocate space for the file (and a null terminator). */
        configSize = configFile.GetSize();
        configBuffer = malloc(configSize + 1);
        if (!configBuffer)
            return 1;

        /* Read the file and use the extra byte to insert a null terminator. */
        configFile.Read(configBuffer, configSize);
        ((char*)configBuffer)[configSize] = '\0';

        auto res = ini_parse_string((const char*)configBuffer, IniHandler, &gConf);
        free(configBuffer);
        if (res) {
            logger::Write("Failed to parse INI\n");
            return 1;
        }

        return 0;
    }

} // namespace config

} // namespace CTRPluginFramework
