#pragma once

#include "mimic_types.hpp"

namespace CTRPluginFramework {

namespace hacks {

void scientist_pre_optimize(float s0, uint32_t* out_calc, uintptr_t mii_info, uint32_t* skill_index, uintptr_t target, helping_mii_handle* helping_miis);
uint32_t scientist_optimize(uintptr_t mii_info, uint32_t* skill_index);

}

}
