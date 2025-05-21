#pragma once

#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "patches/randomizer.hpp"
#include "patches/skills.hpp"

#define ORIG(return_t, ...) HookContext::GetCurrent().OriginalFunction<return_t>(__VA_ARGS__)
#define SEARCH_PATTERN(pattern_t, pattern) Utils::Search<pattern_t>(0x00100000, 0x00709000, pattern)

namespace CTRPluginFramework {

namespace patches {

    inline std::vector<Hook> gRandomizerHooks;
    inline std::vector<Hook> gSkillsHooks;

    inline void InstallHookAtPattern(const std::vector<uint32_t>& pattern, int32_t offset, uint32_t flags,
        uint32_t callback, uint32_t afterCallback, std::vector<Hook>* hookVector = nullptr)
    {
        if (auto res = SEARCH_PATTERN(uint32_t, pattern)) {
            Hook hook;
            hook.InitializeForSubWrap(res + offset, (uint32_t)callback, (uint32_t)afterCallback);
            hook.SetFlags(flags);
            hook.Enable();

            if (hookVector)
                hookVector->push_back(hook);
        }
    }

    inline void Install()
    {
        InstallSkills();
        InstallRandomizer();
    }

} // namespace patches

} // namespace CTRPluginFramework
