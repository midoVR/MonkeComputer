#pragma once

#include "custom-types/shared/macros.hpp"
#include <string>
#include "UnityEngine/MonoBehaviour.hpp"

namespace GorillaUI
{
    class CustomComputer;
    class MonkeWatch;
    
    namespace Components
    {
        class ViewManager; 
        class View;
    }
}

DECLARE_CLASS_CODEGEN(GorillaUI::Components, View, UnityEngine::MonoBehaviour, 
    DECLARE_INSTANCE_METHOD(void, Activate);
    DECLARE_INSTANCE_METHOD(void, Deactivate);
    DECLARE_INSTANCE_FIELD(bool, activatedBefore);
        
    public:
        std::string text = "";
        GorillaUI::CustomComputer* computer = nullptr;
        GorillaUI::MonkeWatch* watch = nullptr;
)