#pragma once

#include "custom-types/shared/macros.hpp"
#include "ViewLib/View.hpp"
#include "InputHandlers/UITextInputHandler.hpp"

DECLARE_CLASS_CUSTOM(GorillaUI, NameChangeView, GorillaUI::Components::View, 
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, DidActivate, bool firstActivation);
    DECLARE_INSTANCE_METHOD(void, Redraw);
    DECLARE_INSTANCE_METHOD(void, DrawHeader);
    DECLARE_INSTANCE_METHOD(void, DrawName);
    DECLARE_INSTANCE_METHOD(void, OnKeyPressed, int key);
    DECLARE_INSTANCE_FIELD(bool, lastNameFailed);
    public:
        void EnterName(std::string code);
        GorillaUI::UITextInputHandler* textInputHandler = nullptr;
)