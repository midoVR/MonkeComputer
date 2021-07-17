#pragma once

#include "custom-types/shared/macros.hpp"
#include "ViewLib/View.hpp"
#include "ViewLib/ViewManager.hpp"
#include "InputHandlers/UISelectionHandler.hpp"
#include "InputHandlers/UITextInputHandler.hpp"
#include "PortableScoreboard/PlayerDisplayInfo.hpp"
#include "GlobalNamespace/VRRig.hpp"
 
#include <vector>

DECLARE_CLASS_CUSTOM(GorillaUI, ScoreboardView, GorillaUI::Components::View, 
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, Update);
    DECLARE_INSTANCE_METHOD(void, UpdatePlayers);
    DECLARE_INSTANCE_METHOD(void, ShowPlayer, int index);
    DECLARE_INSTANCE_METHOD(void, DidActivate, bool firstActivation);
    DECLARE_INSTANCE_METHOD(void, Redraw);
    DECLARE_INSTANCE_METHOD(void, DrawHeader);
    DECLARE_INSTANCE_METHOD(void, DrawPlayers);
    DECLARE_INSTANCE_METHOD(void, OnKeyPressed, int key);

    DECLARE_INSTANCE_FIELD(GorillaUI::Components::ViewManager*, playerViewManager);

    public:
        GorillaUI::UISelectionHandler* selectionHandler = nullptr;
        std::vector<PlayerDisplayInfo> playerInfos = {};
        int counter = 0;
        //static bool get_isTagged(GlobalNamespace::VRRig* rig);
)