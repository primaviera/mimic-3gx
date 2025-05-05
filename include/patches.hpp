#pragma once

#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "patches/randomizer.hpp"
#include "patches/skills.hpp"

#define ORIG(return_t, ...) HookContext::GetCurrent().OriginalFunction<return_t>(__VA_ARGS__);

namespace CTRPluginFramework {

namespace patches {

    inline void InstallHookAtPattern(const std::vector<uint32_t>& pattern, uint32_t offset, uint32_t flags,
        uint32_t callback, uint32_t afterCallback)
    {
        if (auto res = Utils::Search<uint32_t>(0x00100000, 0x00709000, pattern)) {
            Hook hook;
            hook.InitializeForSubWrap(res + offset, (uint32_t)callback, (uint32_t)afterCallback);
            hook.SetFlags(flags);
            hook.Enable();
        }
    }

    inline void Install()
    {
        InstallSkills();
        InstallRandomizer();
    }

} // namespace patches

} // namespace CTRPluginFramework
