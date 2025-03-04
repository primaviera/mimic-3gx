#pragma once

enum job_skill {
    SKILL_FIGHTER_DOUBLE = 0,
    SKILL_FIGHTER_TWICE = 1,
    SKILL_FIGHTER_SPIN = 2,
    SKILL_FIGHTER_GUARD = 3,
    SKILL_FIGHTER_EYESLASH = 4,
    SKILL_FIGHTER_SPIN2 = 5,
    SKILL_FIGHTER_REVIVE_SLAP = 6,
    SKILL_FIGHTER_SLAP = 7,
    SKILL_FIGHTER_09 = 8,
    SKILL_FIGHTER_10 = 9,
    SKILL_FIGHTER_11 = 10,
    SKILL_FIGHTER_12 = 11,
    SKILL_WIZARD_FIRE = 12,
    SKILL_WIZARD_FIRE2 = 13,
    SKILL_WIZARD_FLAMETOWER = 14,
    SKILL_WIZARD_LIGHTNING = 15,
    SKILL_WIZARD_LIGHTNING2 = 16,
    SKILL_WIZARD_LIGHTNING3 = 17,
    SKILL_WIZARD_EXPLOSION = 18,
    SKILL_WIZARD_EXPLOSION2 = 19,
    SKILL_WIZARD_EXPLOSION3 = 20,
    SKILL_WIZARD_BARRIER = 21,
    SKILL_WIZARD_SLEEP = 22,
    SKILL_WIZARD_BIG_WEAPON = 23,
    SKILL_PRIEST_CURE = 24,
    SKILL_PRIEST_CURE2 = 25,
    SKILL_PRIEST_CURE3 = 26,
    SKILL_PRIEST_CURE_ALL = 27,
    SKILL_PRIEST_CURE_ALL2 = 28,
    SKILL_PRIEST_CALM = 29,
    SKILL_PRIEST_KARMA = 30,
    SKILL_PRIEST_RESURRECT = 31,
    SKILL_PRIEST_RESURRECT2 = 32,
    SKILL_PRIEST_HOLY = 33,
    SKILL_PRIEST_11 = 34,
    SKILL_PRIEST_12 = 35,
    SKILL_THIEF_DAGGER = 36,
    SKILL_THIEF_DAGGER2 = 37,
    SKILL_THIEF_ROCK = 38,
    SKILL_THIEF_TRAP = 39,
    SKILL_THIEF_LARGE_TRAP = 40,
    SKILL_THIEF_CREEP = 41,
    SKILL_THIEF_STEAL_ITEM = 42,
    SKILL_THIEF_HIGH_JUMP = 43,
    SKILL_THIEF_09 = 44,
    SKILL_THIEF_10 = 45,
    SKILL_THIEF_11 = 46,
    SKILL_THIEF_12 = 47,
    SKILL_IDOL_ENCORE = 48,
    SKILL_IDOL_FUN_SERVICE = 49,
    SKILL_IDOL_LOVE_CALL = 50,
    SKILL_IDOL_CONCERT = 51,
    SKILL_IDOL_HOWLING = 52,
    SKILL_IDOL_HOWLING2 = 53,
    SKILL_IDOL_DANCE = 54,
    SKILL_IDOL_ANGEL_SONG = 55,
    SKILL_IDOL_LOVE_AND_PEACE = 56,
    SKILL_IDOL_10 = 57,
    SKILL_IDOL_11 = 58,
    SKILL_IDOL_12 = 59,
    SKILL_VAMPIRE_EAT = 60,
    SKILL_VAMPIRE_BAT_ROSE = 61,
    SKILL_VAMPIRE_ZOMBIFY = 62,
    SKILL_VAMPIRE_BREATH = 63,
    SKILL_VAMPIRE_BREATH2 = 64,
    SKILL_VAMPIRE_BREATH3 = 65,
    SKILL_VAMPIRE_CURSE = 66,
    SKILL_VAMPIRE_REVIVE = 67,
    SKILL_VAMPIRE_09 = 68,
    SKILL_VAMPIRE_10 = 69,
    SKILL_VAMPIRE_11 = 70,
    SKILL_VAMPIRE_12 = 71,
    SKILL_COOK_COOKING = 72,
    SKILL_COOK_COOKING2 = 73,
    SKILL_COOK_COOKING3 = 74,
    SKILL_COOK_HEATING = 75,
    SKILL_COOK_SPICY = 76,
    SKILL_COOK_SPICY_ALL = 77,
    SKILL_COOK_BANQUET = 78,
    SKILL_COOK_COOK_MONSTER = 79,
    SKILL_COOK_BAKED_BANANA = 80,
    SKILL_COOK_10 = 81,
    SKILL_COOK_11 = 82,
    SKILL_COOK_12 = 83,
    SKILL_TANK_REPAIR = 84,
    SKILL_TANK_WILD_SHOT = 85,
    SKILL_TANK_HUMAN_CANNON = 86,
    SKILL_TANK_BEAM = 87,
    SKILL_TANK_BEAM2 = 88,
    SKILL_TANK_HEAT_SHOT = 89,
    SKILL_TANK_DEFENSE = 90,
    SKILL_TANK_08 = 91,
    SKILL_TANK_09 = 92,
    SKILL_TANK_10 = 93,
    SKILL_TANK_11 = 94,
    SKILL_TANK_12 = 95,
    SKILL_DEVIL_JOKE_FORK = 96,
    SKILL_DEVIL_PUNISH_FORK = 97,
    SKILL_DEVIL_GRIND_FORK = 98,
    SKILL_DEVIL_MAGIC_DRAIN = 99,
    SKILL_DEVIL_ENERGY_DRAIN = 100,
    SKILL_DEVIL_SEDUCE = 101,
    SKILL_DEVIL_PICK_HIP = 102,
    SKILL_DEVIL_WHISPER = 103,
    SKILL_DEVIL_DEATH_WHISPER = 104,
    SKILL_DEVIL_10 = 105,
    SKILL_DEVIL_11 = 106,
    SKILL_DEVIL_12 = 107,
    SKILL_ROYALTY_WAVE = 108,
    SKILL_ROYALTY_DOUBLE_WAVE = 109,
    SKILL_ROYALTY_WAVE_ALL = 110,
    SKILL_ROYALTY_TEA_TIME = 111,
    SKILL_ROYALTY_COLOGNE = 112,
    SKILL_ROYALTY_DANCE = 113,
    SKILL_ROYALTY_ESCORT = 114,
    SKILL_ROYALTY_BLIND = 115,
    SKILL_ROYALTY_09 = 116,
    SKILL_ROYALTY_10 = 117,
    SKILL_ROYALTY_11 = 118,
    SKILL_ROYALTY_12 = 119,
    SKILL_FLOWER_FLOWER = 120,
    SKILL_FLOWER_FLOWER2 = 121,
    SKILL_FLOWER_EARTH_ANGER = 122,
    SKILL_FLOWER_FLOWER_PARK = 123,
    SKILL_FLOWER_FLOWER_PARK2 = 124,
    SKILL_FLOWER_LIFE_DEW = 125,
    SKILL_FLOWER_FAN = 126,
    SKILL_FLOWER_FAN_ALL = 127,
    SKILL_FLOWER_WHISTLE = 128,
    SKILL_FLOWER_10 = 129,
    SKILL_FLOWER_11 = 130,
    SKILL_FLOWER_12 = 131,
    SKILL_SCIENTIST_CURE_CODE = 132,
    SKILL_SCIENTIST_BUG_CRUSH = 133,
    SKILL_SCIENTIST_JET_FLASK = 134,
    SKILL_SCIENTIST_JET_FLASK2 = 135,
    SKILL_SCIENTIST_BLACK_HOLE = 136,
    SKILL_SCIENTIST_GUARD_MASK = 137,
    SKILL_SCIENTIST_IGNITION = 138,
    SKILL_SCIENTIST_ABSORBENT = 139,
    SKILL_SCIENTIST_09 = 140,
    SKILL_SCIENTIST_10 = 141,
    SKILL_SCIENTIST_11 = 142,
    SKILL_SCIENTIST_12 = 143,
    SKILL_CAT_PUNCH = 144,
    SKILL_CAT_DOUBLE_SCRATCH = 145,
    SKILL_CAT_SCRATCH_ALL = 146,
    SKILL_CAT_LICK = 147,
    SKILL_CAT_POLISH_NAIL = 148,
    SKILL_CAT_FRISK = 149,
    SKILL_CAT_STEAL_DISH = 150,
    SKILL_CAT_GROOM = 151,
    SKILL_CAT_09 = 152,
    SKILL_CAT_10 = 153,
    SKILL_CAT_11 = 154,
    SKILL_CAT_12 = 155,
    SKILL_ELF_DANCE_ARROW = 156,
    SKILL_ELF_MAGIC_ARROW = 157,
    SKILL_ELF_ARROW_RAIN = 158,
    SKILL_ELF_FOREST_GRACE = 159,
    SKILL_ELF_PRAYER = 160,
    SKILL_ELF_CURE_MELODY = 161,
    SKILL_ELF_GUARD_ARROW = 162,
    SKILL_ELF_08 = 163,
    SKILL_ELF_09 = 164,
    SKILL_ELF_10 = 165,
    SKILL_ELF_11 = 166,
    SKILL_ELF_12 = 167
};

enum skill_status {
    SKILL_STATUS_ENABLE = 0,
    SKILL_STATUS_NO_NEED = 1,
    SKILL_STATUS_NO_MP = 2,
    SKILL_STATUS_NO_WEAPON = 3,
    SKILL_STATUS_NO_REVIVE = 4,
    SKILL_STATUS_NO_EFFECT = 5,
    SKILL_STATUS_NO_SLEEP = 6,
    SKILL_STATUS_ALREADY_TRAP = 7,
    SKILL_STATUS_FULL_ITEM = 8,
    SKILL_STATUS_NO_ENCORE = 9,
    SKILL_STATUS_NO_FUN = 10,
    SKILL_STATUS_NO_EAT = 11,
    SKILL_STATUS_NO_SHOT = 12,
    SKILL_STATUS_NO_FRISK = 13,
    SKILL_STATUS_NO_PICK = 14,
    SKILL_STATUS_NO_SEDUCE = 15,
    SKILL_STATUS_NO_TEA = 16
};

enum enemy_skills_1 {
    ENEMY_SKILL_1_NONE = 0,
    ENEMY_SKILL_1_SHOT = 1,
    ENEMY_SKILL_1_HARD_ATTACK = 2,
    ENEMY_SKILL_1_WIDE_ATTACK = 3,
    ENEMY_SKILL_1_WIDE_SHOT = 4,
    ENEMY_SKILL_1_KILL_ATTACK = 5,
    ENEMY_SKILL_1_MAGIC = 6,
    ENEMY_SKILL_1_MAGIC_ALL = 7,
    ENEMY_SKILL_1_CRY_ATTACK = 8,
    ENEMY_SKILL_1_EXCITE_ATTACK = 9,
    ENEMY_SKILL_1_TERROR_ATTACK = 10,
    ENEMY_SKILL_1_SPITE_ATTACK = 11,
    ENEMY_SKILL_1_WAVE_CANNON_1 = 12,
    ENEMY_SKILL_1_WAVE_CANNON_2 = 13,
    ENEMY_SKILL_1_WAVE_CANNON_3 = 14,
    ENEMY_SKILL_1_EAT = 15,
    ENEMY_SKILL_1_EAT_WEAPON = 16,
    ENEMY_SKILL_1_EAT_BANANA = 17,
    ENEMY_SKILL_1_ERASE_BANANA = 18,
    ENEMY_SKILL_1_HIDE = 19,
    ENEMY_SKILL_1_GUARD = 20,
    ENEMY_SKILL_1_HELP_ATTACK = 21,
    ENEMY_SKILL_1_COVER = 22,
    ENEMY_SKILL_1_GAZE = 23,
    ENEMY_SKILL_1_BOMB = 24,
    ENEMY_SKILL_1_SUMMON = 25,
    ENEMY_SKILL_1_RESURRECT = 26,
    ENEMY_SKILL_1_ESCAPE = 27,
    ENEMY_SKILL_1_SATAN = 28,
    ENEMY_SKILL_1_SUPER_SATAN = 29,
    // Custom Skills
    ENEMY_SKILL_1_LAST_MIMIT = 30
};

struct helping_mii_handle {
    uint32_t num;
    uint32_t unk_0x4; // Always set to 4
    uintptr_t* mii_infos[4];
};

struct enemy_status {
    uint16_t hp;
    uint16_t mp;
    uint16_t atk;
    uint16_t def;
    uint16_t mag;
    uint16_t spd;
    uint16_t lvl;
    uint16_t gold;
    uint16_t exp;
    uint16_t common_grub;
    uint16_t common_grub_chance;
    uint16_t rare_grub;
    uint16_t rare_grub_chance;
    uint16_t super_rare_grub;
    uint16_t super_rare_grub_chance;
};

struct enemy_param {
    uint32_t hash; // CRC32 hash of internal name
    const char* model;
    uint32_t id; // Some kind of decimal ID(?)
    uint16_t unk_0xC; // Possibly size?
    uint16_t unk_0xE;
    uint8_t unk_0x10[0x34];
    float distance[3]; // Distance between models (X, Y, Z)
    float scale; // Model scale
    float skill_cam[3]; // (X, Y, Z)
    const char* bone; // What bone camera targets
    uint8_t unk_0x64[0x14];
    enemy_status* status;
};
