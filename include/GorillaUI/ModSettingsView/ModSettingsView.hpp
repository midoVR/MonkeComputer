#pragma once

#include "custom-types/shared/macros.hpp"
#include "ViewLib/View.hpp"
#include "InputHandlers/UISelectionHandler.hpp"

DECLARE_CLASS_CUSTOM(GorillaUI, ModSettingsView, GorillaUI::Components::View, 
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, ShowModView, int index);
    DECLARE_INSTANCE_METHOD(void, DidActivate, bool firstActivation);
    DECLARE_INSTANCE_METHOD(void, Redraw);
    DECLARE_INSTANCE_METHOD(void, DrawHeader);
    DECLARE_INSTANCE_METHOD(void, DrawMods);
    DECLARE_INSTANCE_METHOD(void, OnKeyPressed, int key);
    DECLARE_INSTANCE_FIELD(int, modCount);
    DECLARE_INSTANCE_FIELD(int, pageCount);
    public:
        GorillaUI::UISelectionHandler* selectionHandler = nullptr;
        GorillaUI::UISelectionHandler* pageSelectionHandler = nullptr;
)