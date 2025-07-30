#pragma once

namespace CTRPluginFramework {

namespace patches {

    struct CustomEnemyInfo {
        uint32_t mTurnsUntilLastMimit = 0;
        uint32_t mExtraActionsPerTurn = 0;
        int32_t mCrashAttackIndex = -1;
    };
    extern CustomEnemyInfo gCustomEnemyInfo[8];

    extern int16_t gNoTarget;

    void InstallSkills();

} // namespace patches

} // namespace CTRPluginFramework
