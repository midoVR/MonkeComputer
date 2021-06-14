#pragma once
#include "UnityEngine/Color.hpp"
#include <string>
struct config_t {
    UnityEngine::Color screenColor = {0.0f, 0.0f, 0.0f};
    std::string lastActiveBackground = "";
    bool leftHanded = false;
    int commandHistoryLength = 10;
};

extern config_t config;
bool LoadConfig();
void SaveConfig();