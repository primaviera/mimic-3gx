#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "config.hpp"
#include "hacks/hacks.hpp"
#include "logger.hpp"

namespace CTRPluginFramework
{

void PatchProcess(FwkSettings& settings)
{
    settings.AllowActionReplay = false;
    settings.AllowSearchEngine = false;
    settings.UseGameHidMemory = true;
    settings.TryLoadSDSounds = false;
    settings.WaitTimeToBoot = Seconds(0);
}

int main(void)
{
    if (logger::init(Utils::Format("/luma/plugins/%016llX/", Process::GetTitleID())))
        return 1;
    if (config::init("/config/mimic-3gx/config.toml"))
        return 1;

    logger::write(Utils::Format("Hello World!\nPlugin was built at %s %s\n\n", __DATE__, __TIME__));

    hacks::install();

    Process::WaitForExit();

    logger::deinit();

    return 0;
}

} // namespace CTRPluginFramework
