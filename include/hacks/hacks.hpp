#pragma once

#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "hacks/controllable.hpp"
#include "hacks/randomizer.hpp"
#include "hacks/skills.hpp"

#define ORIG(return_t, ...) \
    HookContext::GetCurrent().OriginalFunction<return_t>(__VA_ARGS__);

namespace CTRPluginFramework
{

namespace hacks
{

/*
 * InitializeForSubWrap is used here but
 * you can still use other modes like MITM
 * by setting their respective flags
 */
inline void install_hook(const std::vector<uint32_t>& pattern, uint32_t offset,
                         uint32_t flags, uint32_t callback,
                         uint32_t after_callback)
{
    if (auto res = Utils::Search<uint32_t>(0x00100000, 0x00709000, pattern)) {
        Hook hook;
        hook.InitializeForSubWrap(res + offset, (uint32_t)callback,
                                  (uint32_t)after_callback);
        hook.SetFlags(flags);
        hook.Enable();
    }
}

inline void patch_u32(const std::vector<uint32_t>& pattern, uint32_t offset,
                      uint32_t instruction)
{
    if (auto res = Utils::Search<uint32_t>(0x00100000, 0x00709000, pattern))
        Process::Patch(res + offset, instruction);
}

inline void install() {
    install_skills();
    install_randomizer();
    install_controllable();
}

} // namespace hacks

} // namespace CTRPluginFramework
