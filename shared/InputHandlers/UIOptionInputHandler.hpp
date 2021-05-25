#pragma once

#include <functional>
#include <string>
#include <vector>
#include "../EKeyboardKey.hpp"

namespace GorillaUI
{
    class UIOptionInputHandler
    {
        public:
            /// @brief constructs an option input handler, which only reacts to options and saves the last pressed option
            /// @param selectKey the key that is used to confirm an input
            /// @param canSelect whether or not the callback should be ran
            UIOptionInputHandler(GorillaUI::EKeyboardKey selectKey, bool canSelect);

            /// @brief the callback that gets called when _selectKey is pressed
            std::function<void(EKeyboardKey)> optionCallback = &nullopt;

            /// @brief handles key for this type of input
            /// @param key the key code to handle
            /// @return bool correct key found
            bool HandleKey(GorillaUI::EKeyboardKey key);

            /// @brief current Value
            EKeyboardKey currentOption = EKeyboardKey::Option1;
        private:
            const GorillaUI::EKeyboardKey _selectKey;
            const bool _canSelect;
            
            static void nullopt(EKeyboardKey) {};
    };
}