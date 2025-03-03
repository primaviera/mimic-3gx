#include "func_ptrs.hpp"

namespace CTRPluginFramework
{

SETUP_FUNC_PTR(crc32, 0x0);
SETUP_FUNC_PTR(rand_percentage, 0x0);

SETUP_FUNC_PTR(get_number_of_party_members, 0x0);
SETUP_FUNC_PTR(get_party_member_at_index, 0x0);
SETUP_FUNC_PTR(get_number_of_enemies, 0x0);
SETUP_FUNC_PTR(get_enemy_at_index, 0x0);

SETUP_FUNC_PTR(can_enemy_be_hit, 0x0);
SETUP_FUNC_PTR(is_party_member_available, 0x0);
SETUP_FUNC_PTR(get_random_enemy, 0x0);

SETUP_FUNC_PTR(play_battle_state, 0x0);
SETUP_FUNC_PTR(set_mii_feeling, 0x0);

SETUP_FUNC_PTR(has_enough_mp_for_skill, 0x0);
SETUP_FUNC_PTR(spend_skill_mp, 0x0);
SETUP_FUNC_PTR(show_cut_in, 0x0);

SETUP_FUNC_PTR(calc_damage, 0x0);
SETUP_FUNC_PTR(setup_damage_params, 0x0);
SETUP_FUNC_PTR(damage_enemy, 0x0);

SETUP_FUNC_PTR(calc_healing, 0x0);
SETUP_FUNC_PTR(setup_healing_params, 0x0);
SETUP_FUNC_PTR(heal_mii_hp, 0x0);
SETUP_FUNC_PTR(heal_mii_mp, 0x0);

SETUP_FUNC_PTR(setup_skill_help, 0x0);

SETUP_FUNC_PTR(enemy_flee, 0x0);

} // namespace CTRPluginFramework
