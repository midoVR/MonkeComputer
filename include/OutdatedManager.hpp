#pragma once
// a namespace to keep track of if the game is currently outdated, or is just up to date

#include <string>

namespace OutdatedManager
{
    bool get_isOutdated();
    std::string get_displayMessage();
    void set_displayMessage(std::string value);
}