#pragma once

#include "custom-types/shared/macros.hpp"
#include "ViewLib/View.hpp"
#include "ViewLib/ViewManager.hpp"
#include "InputHandlers/UISelectionHandler.hpp"
#include "InputHandlers/UITextInputHandler.hpp"
#include "PortableScoreboard/PlayerDisplayInfo.hpp"
#include "GlobalNamespace/VRRig.hpp"
 
#include <vector>

DECLARE_CLASS_CODEGEN(GorillaUI, ScoreboardView, GorillaUI::Components::View, 
    DECLARE_METHOD(void, Awake);
    DECLARE_METHOD(void, Update);
    DECLARE_METHOD(void, UpdatePlayers);
    DECLARE_METHOD(void, ShowPlayer, int index);
    DECLARE_METHOD(void, DidActivate, bool firstActivation);
    DECLARE_METHOD(void, Redraw);
    DECLARE_METHOD(void, DrawHeader);
    DECLARE_METHOD(void, DrawPlayers);
    DECLARE_METHOD(void, OnKeyPressed, int key);

    DECLARE_INSTANCE_FIELD(GorillaUI::Components::ViewManager*, playerViewManager);

    public:
        GorillaUI::UISelectionHandler* selectionHandler = nullptr;
        std::vector<PlayerDisplayInfo> playerInfos = {};
        int counter = 0;
        //static bool get_isTagged(GlobalNamespace::VRRig* rig);

    REGISTER_FUNCTION(
        REGISTER_METHOD(Awake);
        REGISTER_METHOD(Update);
        REGISTER_METHOD(UpdatePlayers);
        REGISTER_METHOD(ShowPlayer);
        REGISTER_METHOD(DidActivate);
        REGISTER_METHOD(Redraw);
        REGISTER_METHOD(DrawHeader);
        REGISTER_METHOD(DrawPlayers);
        REGISTER_METHOD(OnKeyPressed);
        REGISTER_FIELD(playerViewManager);
    )
)