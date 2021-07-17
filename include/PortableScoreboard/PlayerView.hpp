#pragma once

#include "custom-types/shared/macros.hpp"
#include "ViewLib/View.hpp"

#include "GlobalNamespace/VRRig.hpp"

#include "InputHandlers/UISelectionHandler.hpp"
#include "InputHandlers/UITextInputHandler.hpp"
#include "GorillaUI/ModSettingsView/ModSettingsViewManager.hpp"
#include "GorillaUI/BaseGameViews/BaseGameViewManager.hpp"
#include "GorillaUI/DetailView.hpp"
#include "PortableScoreboard/PlayerDisplayInfo.hpp"

#include "GlobalNamespace/GorillaPlayerScoreboardLine.hpp"
#include <vector>

DECLARE_CLASS_CUSTOM(GorillaUI, PlayerView, GorillaUI::Components::View, 
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, Action, int index);
    DECLARE_INSTANCE_METHOD(void, DidActivate, bool firstActivation);
    DECLARE_INSTANCE_METHOD(void, Redraw);
    DECLARE_INSTANCE_METHOD(void, DrawHeader);
    DECLARE_INSTANCE_METHOD(void, DrawBody);
    DECLARE_INSTANCE_METHOD(void, OnKeyPressed, int key);
    DECLARE_INSTANCE_FIELD(GorillaUI::Components::View*, reportView);
    public:
        GorillaUI::UISelectionHandler* selectionHandler = nullptr;
        GorillaUI::UISelectionHandler* pttHandler = nullptr;
        PlayerDisplayInfo playerInfo;
        static std::vector<GlobalNamespace::GorillaPlayerScoreboardLine*> FindScoreboardLineForUserID(std::string userID);
)