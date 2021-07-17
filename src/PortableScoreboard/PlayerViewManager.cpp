#include "PortableScoreboard/PlayerViewManager.hpp"
#include "PortableScoreboard/PlayerView.hpp"

#include "GorillaUI.hpp"
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

DEFINE_TYPE(GorillaUI, PlayerViewManager);

using namespace GorillaUI::Components;

extern Logger& getLogger();

namespace GorillaUI
{
    void PlayerViewManager::DidActivate(bool firstActivation)
    {
        if (firstActivation)
        {
            playerView = CreateView<PlayerView*>();
            activeView = playerView;
        }
        ((PlayerView*)playerView)->playerInfo = playerInfo;
    }

    void PlayerViewManager::NotifyOfKeyPress(int key)
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

    void PlayerViewManager::BackButtonWasPressed(GorillaUI::Components::View* view)
    {
        if (view == playerView)
        {
            if (parentViewManager)
                parentViewManager->DismissViewManager(this);
        }
        else 
        {
            ReplaceTopView(playerView);
        }
    }
} 