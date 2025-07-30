#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "config.hpp"
#include "logger.hpp"
#include "patches.hpp"

namespace CTRPluginFramework {

void PatchProcess(FwkSettings& settings)
{
    settings.WaitTimeToBoot = Seconds(1);
}

int main(void)
{
    if (logger::Initialize(Utils::Format("/luma/plugins/%016llX/logs/", Process::GetTitleID())))
        goto finalize;

    if (config::Initialize("/config/mimic-3gx/config.ini"))
        goto finalize;

    logger::Write(Utils::Format("[mimic-3gx] Build Date: %s %s\n", __DATE__, __TIME__));

    patches::Install();

    Process::WaitForExit();

finalize:
    logger::Finalize();

    return 0;
}

} // namespace CTRPluginFramework
