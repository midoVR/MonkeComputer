#pragma once

#include <functional>
#include "../EKeyboardKey.hpp"

namespace GorillaUI
{
    class UIToggleInputHandler
    {
        public:
            /// @brief constructs an option input handler, which only reacts to options and saves the last pressed option
            /// @param selectKey the key that is used to confirm an input
            /// @param canSelect whether or not the callback should be ran
            UIToggleInputHandler(GorillaUI::EKeyboardKey toggleKey, GorillaUI::EKeyboardKey selectKey, bool canSelect);

            /// @brief the callback that gets called when _selectKey is pressed
            std::function<void(bool)> toggleCallback = &nullopt;

            /// @brief handles key for this type of input
            /// @param key the key code to handle
            /// @return bool correct key found
            bool HandleKey(EKeyboardKey key);

            /// @brief current Value
            bool state;
        private:
            const bool sameKey = false;
            const GorillaUI::EKeyboardKey _selectKey;
            const GorillaUI::EKeyboardKey _toggleKey;
            const bool _canSelect;
            static void nullopt(bool) {};
    };
}