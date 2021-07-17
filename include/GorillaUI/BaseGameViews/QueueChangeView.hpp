#pragma once

#include "custom-types/shared/macros.hpp"
#include "ViewLib/View.hpp"
#include "InputHandlers/UISelectionHandler.hpp"

DECLARE_CLASS_CUSTOM(GorillaUI, QueueChangeView, GorillaUI::Components::View, 
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, DidActivate, bool firstActivation);
    DECLARE_INSTANCE_METHOD(void, Redraw);
    DECLARE_INSTANCE_METHOD(void, DrawHeader);
    DECLARE_INSTANCE_METHOD(void, DrawQueues);
    DECLARE_INSTANCE_METHOD(void, OnKeyPressed, int key);
    DECLARE_INSTANCE_METHOD(void, ChangeQueue, int num);
    public:
        GorillaUI::UISelectionHandler* selectionHandler = nullptr;
)