#pragma once

#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "logger.hpp"
#include "patches/common.hpp"
#include "patches/controllable.hpp"
#include "patches/randomizer.hpp"
#include "patches/skills.hpp"

#define ORIG(return_t, ...) HookContext::GetCurrent().OriginalFunction<return_t>(__VA_ARGS__)
#define SEARCH_PATTERN(pattern_t, pattern) Utils::Search<pattern_t>(0x00100000, 0x00709000, pattern)

namespace CTRPluginFramework {

namespace patches {

    inline void InstallHookAtPattern(const std::vector<uint32_t>& pattern, int32_t offset, uint32_t flags,
        uint32_t callback, uint32_t afterCallback)
    {
        auto res = SEARCH_PATTERN(uint32_t, pattern);
        if (res) {
            Hook hook;
            hook.InitializeForSubWrap(res + offset, callback, afterCallback);
            hook.SetFlags(flags);
            if (hook.Enable() != HookResult::Success)
                logger::Write("Failed to enable hook\n");
            return;
        }
        logger::Write("Failed to find pattern\n");
    }

    inline void InstallHookAtAddress(uint32_t address, uint32_t flags, uint32_t callback, uint32_t afterCallback)
    {
        Hook hook;
        hook.InitializeForSubWrap(address, callback, afterCallback);
        hook.SetFlags(flags);
        if (hook.Enable() != HookResult::Success)
            logger::Write("Failed to enable hook\n");
    }

    inline void Install()
    {
        InstallCommon();
        InstallControllable();
        InstallSkills();
        InstallRandomizer();
    }

} // namespace patches

} // namespace CTRPluginFramework
