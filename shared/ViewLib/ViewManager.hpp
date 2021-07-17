#pragma once

#include "../ViewLib/View.hpp"
#include "custom-types/shared/macros.hpp"
#include <map> 

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

DECLARE_CLASS_CODEGEN(GorillaUI::Components, ViewManager, UnityEngine::MonoBehaviour, 
    DECLARE_CTOR(ctor);
    DECLARE_INSTANCE_METHOD(void, Activate);
    DECLARE_INSTANCE_METHOD(void, Deactivate);
    DECLARE_INSTANCE_METHOD(void, PresentViewManager, GorillaUI::Components::ViewManager* manager);
    DECLARE_INSTANCE_METHOD(void, DismissViewManager, GorillaUI::Components::ViewManager* manager);
    DECLARE_INSTANCE_METHOD(void, ReplaceTopView, GorillaUI::Components::View* view);
    
    DECLARE_INSTANCE_FIELD(GorillaUI::Components::View*, activeView);
    DECLARE_INSTANCE_FIELD(bool, activatedBefore);

    public:
        GorillaUI::Components::ViewManager* parentViewManager = nullptr;
        GorillaUI::Components::ViewManager* childViewManager = nullptr;
        GorillaUI::CustomComputer* computer = nullptr;
        GorillaUI::MonkeWatch* watch = nullptr;
)