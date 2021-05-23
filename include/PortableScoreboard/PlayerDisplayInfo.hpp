#pragma once

#include <string>
#include "UnityEngine/Color.hpp"

struct PlayerDisplayInfo {
    std::string playerID = "";
    std::string nickName = "";
    UnityEngine::Color color;
    bool isTagged = false;
    bool isTalking = false;
};