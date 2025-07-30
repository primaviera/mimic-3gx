#include <3ds.h>
#include <CTRPluginFramework.hpp>

#include "config.hpp"
#include "func_ptrs.hpp"
#include "logger.hpp"
#include "standalone/mimic_types.hpp"
#include "standalone/patterns.hpp"

#include "patches.hpp"
#include "patches/controllable.hpp"

namespace CTRPluginFramework {

namespace patches {

    CustomMiiInfo gCustomMiiInfo[4];

    /* Required to track custom fields from gCustomMiiInfo. */
    int32_t GetMiiIndex(ActorInfo* miiInfo)
    {
        for (uint32_t i = 0; i < GetNumberOfPartyMembers(miiInfo->mBattleInfo); i++) {
            auto selectMii = GetPartyMemberAtIndex(miiInfo->mBattleInfo, i);
            if (!selectMii || selectMii != miiInfo)
                continue;
            return i;
        }
        return -1;
    }

    void OpenMagicMenu(MenuSelector* _this, uint32_t arg2)
    {
        auto& customInfo = gCustomMiiInfo[GetMiiIndex(_this->mMiiInfo)];
        _this->mSelectedMagicPage = customInfo.mSelectedMagicPage;
        _this->mSelectedMagicEntry = customInfo.mSelectedMagicEntry;
        ORIG(void, _this, arg2);
    }

    void LoadSkillInfoFromMenu(MenuSelector* _this, int8_t selectedPage, int32_t selectedEntry)
    {
        auto& customInfo = gCustomMiiInfo[GetMiiIndex(_this->mMiiInfo)];
        customInfo.mSelectedMagicPage = selectedPage;
        customInfo.mSelectedMagicEntry = selectedEntry;
        ORIG(void, _this, selectedPage, selectedEntry);
    }

    void OpenMainMenu(MenuSelector* _this, uint32_t arg2, uint32_t arg3)
    {
        auto& customInfo = gCustomMiiInfo[GetMiiIndex(_this->mMiiInfo)];
        _this->mSelectedMainEntry = customInfo.mSelectedMainEntry;
        ORIG(void, _this, arg2, arg3);
    }

    void HandleMainMenu(MenuSelector* _this, uintptr_t arg2, uint32_t arg3)
    {
        auto& customInfo = gCustomMiiInfo[GetMiiIndex(_this->mMiiInfo)];
        _this->mSelectedItemEntry = customInfo.mSelectedItemEntry;
        _this->mSelectedEnemyEntry = customInfo.mSelectedEnemyEntry;
        _this->mSelectedPartyEntry = customInfo.mSelectedPartyEntry;
        ORIG(void, _this, arg2, arg3);
        customInfo.mSelectedMainEntry = _this->mSelectedMainEntry;
        customInfo.mSelectedItemEntry = _this->mSelectedItemEntry;
        customInfo.mSelectedEnemyEntry = _this->mSelectedEnemyEntry;
        customInfo.mSelectedPartyEntry = _this->mSelectedPartyEntry;
    }

    void OpenItemMenu(MenuSelector* _this, uint32_t arg2)
    {
        auto& customInfo = gCustomMiiInfo[GetMiiIndex(_this->mMiiInfo)];
        _this->mSelectedItemEntry = customInfo.mSelectedItemEntry;
        ORIG(void, _this, arg2);
    }

    void OpenEnemyMenu(MenuSelector* _this, uintptr_t arg2, uint32_t arg3)
    {
        auto& customInfo = gCustomMiiInfo[GetMiiIndex(_this->mMiiInfo)];
        _this->mSelectedEnemyEntry = customInfo.mSelectedEnemyEntry;
        ORIG(void, _this, arg2, arg3);
    }

    void OpenPartyMenu(MenuSelector* _this, uintptr_t arg2, uint32_t arg3)
    {
        auto& customInfo = gCustomMiiInfo[GetMiiIndex(_this->mMiiInfo)];
        _this->mSelectedPartyEntry = customInfo.mSelectedPartyEntry;
        ORIG(void, _this, arg2, arg3);
    }

    void InstallControllable()
    {
        if (!config::gUserConfig.mControllable.active)
            return;

        logger::Write("[Controllable] Installing...\n");

        if (auto res = SEARCH_PATTERN(uint32_t, checkMainHero_Pattern); res)
            Process::Patch(res + 0x10, 0xE320F000); // nop

        if (auto res = SEARCH_PATTERN(uint32_t, autoBattleVisibility_Pattern);
            config::gUserConfig.mControllable.showAutoBattle && res)
            Process::Patch(res + 0x8, 0xE320F000); // nop

        if (config::gUserConfig.mControllable.saveMenuSelection) {
            InstallHookAtPattern(openMagicMenu_Pattern, 0x0, (MITM_MODE), (uint32_t)OpenMagicMenu, 0);
            InstallHookAtPattern(loadSkillInfoFromMenu_Pattern, 0x0, (MITM_MODE), (uint32_t)LoadSkillInfoFromMenu, 0);
            InstallHookAtPattern(openMainMenu_Pattern, 0x0, (MITM_MODE), (uint32_t)OpenMainMenu, 0);
            InstallHookAtPattern(handleMainMenu_Pattern, 0x0, (MITM_MODE), (uint32_t)HandleMainMenu, 0);
            InstallHookAtPattern(openItemMenu_Pattern, 0x0, (MITM_MODE), (uint32_t)OpenItemMenu, 0);
            InstallHookAtPattern(openEnemyMenu_Pattern, 0x0, (MITM_MODE), (uint32_t)OpenEnemyMenu, 0);
            InstallHookAtPattern(openPartyMenu_Pattern, 0x0, (MITM_MODE), (uint32_t)OpenPartyMenu, 0);
        }
    }

} // namespace patches

} // namespace CTRPluginFramework
