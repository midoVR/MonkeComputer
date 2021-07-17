#pragma once

#pragma once

#include "custom-types/shared/macros.hpp"
#include "ViewLib/View.hpp"
#include "InputHandlers/UISelectionHandler.hpp"
#include "InputHandlers/UINumberInputHandler.hpp"

DECLARE_CLASS_CUSTOM(GorillaUI, MonkeComputerConfigView, GorillaUI::Components::View, 
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, DidActivate, bool firstActivation);
    DECLARE_INSTANCE_METHOD(void, UpdateColor, int num);
    DECLARE_INSTANCE_METHOD(void, Redraw);
    DECLARE_INSTANCE_METHOD(void, DrawHeader);
    DECLARE_INSTANCE_METHOD(void, DrawColors);
    DECLARE_INSTANCE_METHOD(void, OnKeyPressed, int key);

    public:
        GorillaUI::UISelectionHandler* selectionHandler = nullptr;
        GorillaUI::UINumberInputHandler* numberInputHandlers[3];
)