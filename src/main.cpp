#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "config.hpp"
#include "logger.hpp"
#include "patches.hpp"

namespace CTRPluginFramework {

void PatchProcess(FwkSettings& settings)
{
    settings.AllowActionReplay = false;
    settings.AllowSearchEngine = false;
    settings.UseGameHidMemory = true;
    settings.TryLoadSDSounds = false;
    settings.WaitTimeToBoot = Seconds(1);
}

int main(void)
{
    if (logger::Initialize(Utils::Format("/luma/plugins/%016llX/logs/", Process::GetTitleID())))
        goto finalize;
    if (config::Initialize("/config/mimic-3gx/config.ini"))
        goto finalize;

    logger::Write(Utils::Format("Hello World!\nBuild Date: %s %s\n\n", __DATE__, __TIME__));

    patches::Install();

    Process::WaitForExit();

finalize:
    logger::Finalize();

    return 0;
}

} // namespace CTRPluginFramework
