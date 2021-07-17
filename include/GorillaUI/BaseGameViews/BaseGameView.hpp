#pragma once

#include "custom-types/shared/macros.hpp"
#include "ViewLib/View.hpp"
#include "InputHandlers/UISelectionHandler.hpp"

DECLARE_CLASS_CUSTOM(GorillaUI, BaseGameView, GorillaUI::Components::View, 
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, ShowView, int index);
    DECLARE_INSTANCE_METHOD(void, DidActivate, bool firstActivation);
    DECLARE_INSTANCE_METHOD(void, Redraw);
    DECLARE_INSTANCE_METHOD(void, DrawHeader);
    DECLARE_INSTANCE_METHOD(void, DrawViews);
    DECLARE_INSTANCE_METHOD(void, OnKeyPressed, int key);
    DECLARE_INSTANCE_FIELD(Components::View*, customRoomView);
    DECLARE_INSTANCE_FIELD(Components::View*, nameChangeView);
    DECLARE_INSTANCE_FIELD(Components::View*, colorChangeView);
    DECLARE_INSTANCE_FIELD(Components::View*, turnChangeView);
    DECLARE_INSTANCE_FIELD(Components::View*, micChangeView);
    DECLARE_INSTANCE_FIELD(Components::View*, groupChangeView);
    DECLARE_INSTANCE_FIELD(Components::View*, queueChangeView);
    DECLARE_INSTANCE_FIELD(Components::View*, voiceChatView);

    public:
        GorillaUI::UISelectionHandler* selectionHandler = nullptr;
        bool old = true;
)