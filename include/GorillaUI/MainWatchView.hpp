#pragma once

#include "custom-types/shared/macros.hpp"
#include "ViewLib/View.hpp"
#include "InputHandlers/UISelectionHandler.hpp"
#include "InputHandlers/UITextInputHandler.hpp"
#include "GorillaUI/ModSettingsView/ModSettingsViewManager.hpp"
#include "GorillaUI/BaseGameViews/BaseGameViewManager.hpp"
#include "GorillaUI/DetailView.hpp"

DECLARE_CLASS_CUSTOM(GorillaUI, MainWatchView, GorillaUI::Components::View, 
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, ShowView, int index);
    DECLARE_INSTANCE_METHOD(void, DidActivate, bool firstActivation);
    DECLARE_INSTANCE_METHOD(void, Redraw);
    DECLARE_INSTANCE_METHOD(void, DrawHeader);
    DECLARE_INSTANCE_METHOD(void, DrawSubMenus);
    DECLARE_INSTANCE_METHOD(void, OnKeyPressed, int key);
    DECLARE_INSTANCE_FIELD(int, entryCount);
    DECLARE_INSTANCE_FIELD(int, pageCount);

    public:
        GorillaUI::UISelectionHandler* selectionHandler = nullptr;
        GorillaUI::UISelectionHandler* pageSelectionHandler = nullptr;
        bool screenSaver = true;
)