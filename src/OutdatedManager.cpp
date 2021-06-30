#include "OutdatedManager.hpp"

namespace OutdatedManager
{
    std::string message = "";
    bool outdated = false;

    bool get_isOutdated()
    {
        return outdated;
    }

    std::string get_displayMessage()
    {
        return message;
    }

    void set_displayMessage(std::string value)
    {
        message = value;
        outdated = true;
    }
}