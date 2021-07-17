#pragma once

#include "custom-types/shared/macros.hpp"
#include "../ViewLib/CustomComputer.hpp"
#include "../EKeyboardKey.hpp"

#include "UnityEngine/Material.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Collider.hpp"
#include "GlobalNamespace/GorillaTriggerBox.hpp"
namespace GorillaUI
{
    class CustomComputer;   
}
DECLARE_CLASS_CODEGEN(GorillaUI::Components, GorillaKeyboardButton, GlobalNamespace::GorillaTriggerBox,
    DECLARE_INSTANCE_METHOD(void, OnTriggerEnter, UnityEngine::Collider* collider);
    DECLARE_INSTANCE_METHOD(void, OnTriggerExit, UnityEngine::Collider* collider);
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, BumpIn);
    DECLARE_INSTANCE_METHOD(void, BumpOut);
    DECLARE_INSTANCE_FIELD(bool, isOnCooldown);
    DECLARE_INSTANCE_FIELD(bool, functionKey);
    DECLARE_INSTANCE_FIELD(UnityEngine::Material*, material);
    DECLARE_INSTANCE_FIELD(float, pressTime);
    
    public:
        EKeyboardKey key;
        void Init(GorillaUI::CustomComputer* computer, EKeyboardKey key);
        void Init(GorillaUI::CustomComputer* computer, EKeyboardKey key, std::string text);
        void Init(GorillaUI::CustomComputer* computer, EKeyboardKey key, std::string text, UnityEngine::Color buttonColor);

    private:
        UnityEngine::Color originalColor;
        GorillaUI::CustomComputer* computer;
)