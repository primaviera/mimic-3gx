#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "hacks/hacks.hpp"
#include "hacks/controllable.hpp"
#include "patterns.hpp"

namespace CTRPluginFramework {

namespace hacks {

    void install_controllable() {
        patch_u32(is_main_hero_pattern, 0xC, 0xE320F000); // NOP
        patch_u32(is_main_hero_pattern, 0x10, 0xE320F000); // NOP
        patch_u32(autobattle_availability_pattern, -0x4, 0xE320F000); // NOP
    }

}

}
