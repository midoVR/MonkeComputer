#pragma once

#include <functional>
#include <string>
#include "../EKeyboardKey.hpp"

namespace GorillaUI
{
    class UITextInputHandler
    {
        public:
            /// @brief constructs an option input handler, which only reacts to options and saves the last pressed option
            /// @param selectKey the key that is used to confirm an input
            /// @param canSelect whether or not the callback should be ran
            UITextInputHandler(GorillaUI::EKeyboardKey selectKey, bool canSelect);

            /// @brief the callback that gets called when _selectKey is pressed
            std::function<void(std::string)> textCallback = &nullopt;

            /// @brief handles key for this type of input
            /// @param key the key code to handle
            /// @return bool correct key found
            bool HandleKey(EKeyboardKey key);

            /// @brief current value
            std::string text;
        private:
            const GorillaUI::EKeyboardKey _selectKey;
            const bool _canSelect;
            void TypeChar(EKeyboardKey key);
            void AddSpace();
            void DeleteChar();
            static void nullopt(std::string) {};
    };
}