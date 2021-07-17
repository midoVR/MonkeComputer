#pragma once

#include "custom-types/shared/macros.hpp"
#include "ViewLib/View.hpp"
#include "InputHandlers/UIToggleInputHandler.hpp"

DECLARE_CLASS_CUSTOM(GorillaUI, VoiceChatView, GorillaUI::Components::View, 
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, DidActivate, bool firstActivation);
    DECLARE_INSTANCE_METHOD(void, UpdateVC, bool value);
    DECLARE_INSTANCE_METHOD(void, Redraw);
    DECLARE_INSTANCE_METHOD(void, DrawHeader);
    DECLARE_INSTANCE_METHOD(void, DrawVC);
    DECLARE_INSTANCE_METHOD(void, OnKeyPressed, int key);

    public:
        GorillaUI::UIToggleInputHandler* toggleInputHandler = nullptr;
)