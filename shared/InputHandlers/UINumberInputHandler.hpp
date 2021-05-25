#pragma once

#include <functional>
#include <string>
#include "../EKeyboardKey.hpp"

namespace GorillaUI
{
    class UINumberInputHandler
    {
        public:
            /// @brief constructs a number inputhandler, which only reacts to numbers and the select key
            /// @param selectKey the key that is used to confirm an input
            /// @param canSelect whether or not the callback should be ran
            UINumberInputHandler(EKeyboardKey selectKey, bool canSelect);

            /// @brief handles key for this type of input
            /// @param key the key code to handle
            /// @return bool correct key found
            bool HandleKey(EKeyboardKey key);

            /// @brief the callback that gets called when _selectKey is pressed
            std::function<void(int)> numberCallback = &nullopt;

            /// @brief (inclusive) minimum value
            int min = 0;

            /// @brief (exclusive) maximum value
            int max = 0;

            /// @brief current value
            int number = 0;
        private:
            const EKeyboardKey _selectKey;
            const bool _canSelect;
            std::string numberString = "";
            static void nullopt(int) {};

            void DeleteNum();
            void TypeNum(EKeyboardKey key);
            void UpdateNum();
    };
}