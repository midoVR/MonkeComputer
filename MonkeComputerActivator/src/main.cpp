#include "beatsaber-hook/shared/utils/utils.h"
#include "modloader/shared/modloader.hpp"
#include "monkecomputer/shared/GorillaUI.hpp"

ModInfo modInfo;

extern "C" void setup(ModInfo info)
{
    info.id = ID;
    info.version = VERSION;

    modInfo = info;
}

extern "C" void load()
{
    GorillaUI::Init();
}