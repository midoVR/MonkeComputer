#include "ViewLib/ViewManager.hpp"
#include "ViewLib/MonkeWatch.hpp"
#include "KeyExtension.hpp"
#include "ViewLib/CustomComputer.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-functions.hpp"

DEFINE_TYPE(GorillaUI::Components, ViewManager);

extern Logger& getLogger();

using namespace UnityEngine;

namespace GorillaUI::Components
{
    void ViewManager::ctor()
    {
        parentViewManager = nullptr;
        childViewManager = nullptr;
        computer = nullptr;
        watch = nullptr;
    }
    
    void ViewManager::Activate()
    {
        auto* didActivate = il2cpp_functions::class_get_method_from_name(il2cpp_utils::ExtractClass (this), "DidActivate", 1);
        if (didActivate)
            il2cpp_utils::RunMethod(this, didActivate, !activatedBefore);

        if (activeView)
        {
            if (computer)
            {
                activeView->computer = computer;
            }
            else if (watch)
            {
                activeView->watch = watch;
            }

            activeView->Activate();
        }
        activatedBefore = true;
    }

    void ViewManager::Deactivate()
    {
        auto* didDeactivate = il2cpp_functions::class_get_method_from_name(il2cpp_utils::ExtractClass (this), "DidDeactivate", 0);
        if (didDeactivate)
            il2cpp_utils::RunMethod(this, didDeactivate);
    }
    
    void ViewManager::PresentViewManager(GorillaUI::Components::ViewManager* manager)
    {
        childViewManager = manager;
        manager->parentViewManager = this;

        if (computer)
        {
            manager->computer = computer;
            computer->activeViewManager = manager;
        }
        else if (watch)
        {
            manager->watch = watch;
            watch->activeViewManager = manager;
        }

        activeView->Deactivate();
        manager->Activate();
    }

    void ViewManager::DismissViewManager(GorillaUI::Components::ViewManager* manager)
    {
        childViewManager = nullptr;
        manager->activeView->Deactivate();
        manager->Deactivate();
        activeView->Activate();
        if (computer)
        {
            computer->activeViewManager = this;
            CustomComputer::Redraw();
        }
        else if (watch)
        {
            watch->activeViewManager = this;
            MonkeWatch::Redraw();
        }
    }

    void ViewManager::ReplaceTopView(GorillaUI::Components::View* view)
    {
        if (activeView) activeView->Deactivate();
        if (computer)
        {
            view->computer = computer;
        }
        else if (watch)
        {
            view->watch = watch;
        }
        activeView = view;
        activeView->Activate();

        if (computer)
        {
            CustomComputer::Redraw();
        }
        else if (watch)
        {
            MonkeWatch::Redraw();
        }
    }
}