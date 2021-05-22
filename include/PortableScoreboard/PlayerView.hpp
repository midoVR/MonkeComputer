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

DECLARE_CLASS_CODEGEN(GorillaUI, PlayerView, GorillaUI::Components::View, 
    DECLARE_METHOD(void, Awake);
    DECLARE_METHOD(void, Action, int index);
    DECLARE_METHOD(void, DidActivate, bool firstActivation);
    DECLARE_METHOD(void, Redraw);
    DECLARE_METHOD(void, DrawHeader);
    DECLARE_METHOD(void, DrawBody);
    DECLARE_METHOD(void, OnKeyPressed, int key);
    DECLARE_INSTANCE_FIELD(GorillaUI::Components::View*, reportView);
    public:
        GorillaUI::UISelectionHandler* selectionHandler = nullptr;
        PlayerDisplayInfo playerInfo;
        static GlobalNamespace::VRRig* FindVRRigForUserID(std::string userID);
        static std::vector<GlobalNamespace::GorillaPlayerScoreboardLine*> FindScoreboardLineForUserID(std::string userID);

    REGISTER_FUNCTION(
        REGISTER_METHOD(Awake);
        REGISTER_METHOD(Action);
        REGISTER_METHOD(DidActivate);
        REGISTER_METHOD(Redraw);
        REGISTER_METHOD(DrawHeader);
        REGISTER_METHOD(DrawBody);
        REGISTER_METHOD(OnKeyPressed);
        REGISTER_FIELD(reportView);
    )
)