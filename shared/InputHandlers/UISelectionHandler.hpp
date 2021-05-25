#pragma once

#include <functional>
#include <string>
#include <vector>
#include "../EKeyboardKey.hpp"

namespace GorillaUI
{
    class UISelectionHandler
    {
        public:
            /// @brief constructs a selection input handler, which only reacts to the given keys
            /// @param upKey the key that is used to increase the index
            /// @param downKey the key that is used to decrease the index
            /// @param selectKey the key that is used to confirm an input
            /// @param canSelect whether or not the callback should be ran
            /// @param wrapAround whether or not when increased (or decreased) beyond limits, it should wrap around to the other extreme
            UISelectionHandler(GorillaUI::EKeyboardKey upKey, GorillaUI::EKeyboardKey downKey, GorillaUI::EKeyboardKey selectKey, bool canSelect, bool wrapAround = false);

            /// @brief the callback that gets called when _selectKey is pressed
            std::function<void(int)> selectionCallback = &nullopt;

            /// @brief (inclusive) minimum value
            int min = 0;
            
            /// @brief (exclusive) maximum value
            int max = 0;

            /// @brief current value
            int currentSelectionIndex = 0;

            /// @brief handles key for this type of input
            /// @param key the key code to handle
            /// @return bool correct key found
            bool HandleKey(GorillaUI::EKeyboardKey key);
        private:
            const GorillaUI::EKeyboardKey _upKey;
            const GorillaUI::EKeyboardKey _downKey;
            const GorillaUI::EKeyboardKey _selectKey;
            const bool _canSelect;
            const bool _wrapAround;
            void MoveSelectionUp();
            void MoveSelectionDown();
            void ClampSelection();
            static void nullopt(int) {};
    };
}