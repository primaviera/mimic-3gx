#pragma once

namespace CTRPluginFramework {

namespace hacks {

uint32_t warrior_flee(uintptr_t mii_info, uint32_t* skill_index, uintptr_t enemy_info);
uint32_t warrior_hit_all(uintptr_t mii_info, uint32_t* skill_index);
uint32_t warrior_single_heal(uintptr_t mii_info, uint32_t* skill_index, uintptr_t target_mii);
uint32_t warrior_status_all(uintptr_t mii_info, uint32_t* skill_index);

}

}
