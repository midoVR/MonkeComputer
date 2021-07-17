#pragma once

#include "custom-types/shared/macros.hpp"
#include "ViewLib/View.hpp"
#include "InputHandlers/UITextInputHandler.hpp"
#include "InputHandlers/UIOptionInputHandler.hpp"

DECLARE_CLASS_CUSTOM(GorillaUI, CustomRoomView, GorillaUI::Components::View, 
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, DidActivate, bool firstActivation);
    DECLARE_INSTANCE_METHOD(void, DidDeactivate);
    DECLARE_INSTANCE_METHOD(void, Redraw);
    DECLARE_INSTANCE_METHOD(void, DrawHeader);
    DECLARE_INSTANCE_METHOD(void, DrawCode);
    DECLARE_INSTANCE_METHOD(void, OnKeyPressed, int key);
    DECLARE_INSTANCE_METHOD(void, Update);
    DECLARE_INSTANCE_FIELD(float, lastUpdatedTime);


    public:
        void EnterCode(std::string code);
        GorillaUI::UITextInputHandler* textInputHandler = nullptr;
        GorillaUI::UIOptionInputHandler* optionInputHandler = nullptr;
    private:
        void OnJoinRoomFailed(short error, std::string message);
        void OnJoinedRoom();
)