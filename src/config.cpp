#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include <inih/ini.h>

#include "config.hpp"
#include "logger.hpp"

namespace CTRPluginFramework {

namespace config {

    config::Configuration gUserConfig;

    bool IniGetBoolean(const char* value, bool defaultValue)
    {
        if (!strcmp(value, "true"))
            return true;
        else if (!strcmp(value, "false"))
            return false;
        return defaultValue;
    }

#define MATCH(s, n) !strcmp(section, s) && !strcmp(name, n)
#define HANDLE(section, key, field, func)                                                                              \
    if (MATCH(section, key)) {                                                                                         \
        userConfig->field = func(value, gUserConfig.field);                                                            \
        return 1;                                                                                                      \
    }
#define HANDLE_BOOL(section, key, field) HANDLE(section, key, field, IniGetBoolean)

    int IniHandler(void* user, const char* section, const char* name, const char* value)
    {
        config::Configuration* userConfig = (config::Configuration*)user;

        HANDLE_BOOL("controllable", "active", mControllable.active);
        HANDLE_BOOL("controllable", "showAutoBattle", mControllable.showAutoBattle);
        HANDLE_BOOL("controllable", "saveMenuSelection", mControllable.saveMenuSelection);
        HANDLE_BOOL("randomizer", "active", mRandomizer.active);
        HANDLE_BOOL("skills", "active", mSkills.active);
        HANDLE_BOOL("skills", "jobSkills", mSkills.jobSkills);

        return 0;
    }

#undef HANDLE_BOOL
#undef HANDLE
#undef MATCH

    int Initialize(const std::string& path)
    {
        File configFile;
        void* configBuffer;
        uint32_t configSize;

        /* If this fails, use default config. */
        if (File::Open(configFile, path, File::READ)) {
            logger::Write("Failed to open config, using default config...\n");
            return 0;
        }

        /* Allocate space for the file (and a null terminator). */
        configSize = configFile.GetSize();
        configBuffer = malloc(configSize + 1);
        if (!configBuffer) {
            logger::Write("Failed to allocate space for config, exiting...\n");
            return 1;
        }

        /* Read the file and use the extra byte to insert a null terminator. */
        configFile.Read(configBuffer, configSize);
        ((char*)configBuffer)[configSize] = '\0';

        auto res = ini_parse_string((const char*)configBuffer, IniHandler, &gUserConfig);
        free(configBuffer);
        if (res) {
            logger::Write("Failed to parse configuration file, exiting...\n");
            return 1;
        }

        return 0;
    }

} // namespace config

} // namespace CTRPluginFramework
