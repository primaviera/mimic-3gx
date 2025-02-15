#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "hacks.hpp"
#include "logger.hpp"

namespace CTRPluginFramework
{

void PatchProcess(FwkSettings& settings)
{
    settings.AllowActionReplay = false;
    settings.AllowSearchEngine = false;
    settings.UseGameHidMemory = true;
    settings.TryLoadSDSounds = false;
}

int main(void)
{
    if (logger::init(Utils::Format("/luma/plugins/%016llX/", Process::GetTitleID())))
        return 1;

    logger::write("Hello World!\n");

    hacks::install_randomizer();
    hacks::install_skills();

    Process::WaitForExit();

    logger::deinit();

    return 0;
}

} // namespace CTRPluginFramework
