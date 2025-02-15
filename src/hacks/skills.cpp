#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "func_ptrs.hpp"
#include "hacks.hpp"
#include "patterns.hpp"

namespace CTRPluginFramework {

namespace hacks {

    extern "C" uint32_t gleeful;

    // 'Playful Antics' makes the target gleeful
    uint32_t gleeful = 9;
    void NAKED frisk_gleeful()
    {
        asm volatile("push {r0-r4, lr} \n"
                     "mov r0, r4 \n"
                     "ldr r1, =gleeful \n"
                     "ldr r2, [r5, #4] \n"
                     "ldrh r2, [r2, #0x60] \n"
                     "mov r3, #0 \n"
                     "ldr r4, =set_mii_feeling \n"
                     "ldr r4, [r4] \n"
                     "blx r4 \n"
                     "pop {r0-r4, pc}");
    }

    // 'Sweet Whispers' has a chance to make the enemy flee
    void whisper_flee(uintptr_t enemy_info, uintptr_t mii_info, int r2, int r3, const char* caption_hit,
        const char* caption_already, int r6)
    {
        if (!strcmp(caption_hit, "SkillWhisperHitCpt")) {
            if (Utils::Random(0, 100) < 20) {
                enemy_flee(enemy_info);
                return;
            }
        }
        CTRPluginFramework::HookContext::GetCurrent().OriginalFunction<void>(enemy_info, mii_info, r2, r3, caption_hit,
            caption_already, r6);
    }

    void install_skills()
    {
        install_hook(frisk_pattern, 0x40, (WRAP_SUB), (uint32_t)frisk_gleeful, 0);
        install_hook(vacancy_pattern, 0x0, (MITM_MODE), (uint32_t)whisper_flee, 0);
    }

} // namespace hacks

} // namespace CTRPluginFramework
