#pragma once

#include "ViewLib/View.hpp"
#include "InputHandlers/UISelectionHandler.hpp"

DECLARE_CLASS_CUSTOM(GorillaUI, BackgroundsView, GorillaUI::Components::View, 
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, LoadTexture, int index);
    DECLARE_INSTANCE_METHOD(void, DidActivate, bool firstActivation);
    DECLARE_INSTANCE_METHOD(void, Redraw);
    DECLARE_INSTANCE_METHOD(void, DrawHeader);
    DECLARE_INSTANCE_METHOD(void, DrawBackgrounds);
    DECLARE_INSTANCE_METHOD(void, OnKeyPressed, int key);
    DECLARE_INSTANCE_FIELD(int, bgCount);
    DECLARE_INSTANCE_FIELD(int, pageCount);
    public:
        GorillaUI::UISelectionHandler* selectionHandler;
        GorillaUI::UISelectionHandler* pageSelectionHandler;
)