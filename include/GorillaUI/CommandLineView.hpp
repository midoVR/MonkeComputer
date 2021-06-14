#pragma once

#include "custom-types/shared/macros.hpp"
#include "ViewLib/View.hpp"
#include "InputHandlers/UISelectionHandler.hpp"
#include "InputHandlers/UITextInputHandler.hpp"
#include "GorillaUI/ModSettingsView/ModSettingsViewManager.hpp"
#include "GorillaUI/BaseGameViews/BaseGameViewManager.hpp"
#include "GorillaUI/DetailView.hpp"

DECLARE_CLASS_CODEGEN(GorillaUI, CommandLineView, GorillaUI::Components::View, 
    DECLARE_METHOD(void, Awake);
    DECLARE_METHOD(void, DidActivate, bool firstActivation);
    DECLARE_METHOD(void, Redraw);
    DECLARE_METHOD(void, DrawHeader);
    DECLARE_METHOD(void, DrawCommand);
    DECLARE_METHOD(void, OnKeyPressed, int key);

    public:
        void OnCommandInput(std::string value);
        UITextInputHandler* textInputHandler = nullptr;
        UISelectionHandler* historyHandler = nullptr;
        std::string lastOutput;
        std::vector<std::string> commandHistory = {};
        std::string GetCommand(int index);
        void AddCommandToHistory(std::string command);

    REGISTER_FUNCTION(
        REGISTER_METHOD(Awake);
        REGISTER_METHOD(DidActivate);
        REGISTER_METHOD(Redraw);
        REGISTER_METHOD(DrawHeader);
        REGISTER_METHOD(DrawCommand);
        REGISTER_METHOD(OnKeyPressed);
    )
)