#include "PortableScoreboard/ScoreboardViewManager.hpp"
#include "PortableScoreboard/ScoreboardView.hpp"
#include "GorillaUI.hpp"
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

DEFINE_TYPE(GorillaUI, ScoreboardViewManager);

using namespace GorillaUI::Components;

extern Logger& getLogger();

namespace GorillaUI
{
    void ScoreboardViewManager::DidActivate(bool firstActivation)
    {
        if (firstActivation)
        {
            mainView = CreateView<ScoreboardView*>();
            activeView = mainView;
        }
    }

    void ScoreboardViewManager::NotifyOfKeyPress(int key)
    {
        if (key == (int)EKeyboardKey::Back)
        {
            il2cpp_utils::RunMethod(this, "BackButtonWasPressed", activeView);
        }
        else
        {
            if (activeView) 
            {
                il2cpp_utils::RunMethod(activeView, "OnKeyPressed", key);
            }
            else getLogger().error("No active view found");
        }
    }

    void ScoreboardViewManager::BackButtonWasPressed(GorillaUI::Components::View* view)
    {
        if (view == mainView)
        {
            if (parentViewManager)
                parentViewManager->DismissViewManager(this);
        }
        else 
        {
            ReplaceTopView(mainView);
        }
    }
}