#pragma once

#include <3ds.h>
#include <CTRPluginFramework.hpp>

namespace CTRPluginFramework
{

namespace hacks
{

/*
 * Here InitializeForSubWrap is used here
 * but you can still use other modes like
 * MITM by setting their respective flags
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

void install_randomizer();
void install_skills();

} // namespace hacks

} // namespace CTRPluginFramework
