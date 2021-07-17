#pragma once

#include "custom-types/shared/macros.hpp"
#include "ViewLib/View.hpp"
#include "InputHandlers/UISelectionHandler.hpp"
#include "InputHandlers/UITextInputHandler.hpp"
#include "GorillaUI/ModSettingsView/ModSettingsViewManager.hpp"
#include "GorillaUI/BaseGameViews/BaseGameViewManager.hpp"
#include "GorillaUI/DetailView.hpp"
#include "PortableScoreboard/PlayerDisplayInfo.hpp"
#include <vector>

DECLARE_CLASS_CUSTOM(GorillaUI, ReportView, GorillaUI::Components::View, 
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, Report, int index);
    DECLARE_INSTANCE_METHOD(void, DidActivate, bool firstActivation);
    DECLARE_INSTANCE_METHOD(void, Redraw);
    DECLARE_INSTANCE_METHOD(void, DrawHeader);
    DECLARE_INSTANCE_METHOD(void, DrawReportOptions);
    DECLARE_INSTANCE_METHOD(void, OnKeyPressed, int key);

    public:
        GorillaUI::UISelectionHandler* selectionHandler = nullptr;
        PlayerDisplayInfo playerInfo;
)