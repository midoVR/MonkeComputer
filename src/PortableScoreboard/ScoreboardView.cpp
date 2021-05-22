#include "PortableScoreboard/ScoreboardView.hpp"
#include "PortableScoreboard/PlayerViewManager.hpp"
#include "GorillaUI.hpp"
#include "Register.hpp"
#include "ViewLib/MonkeWatch.hpp"
#include "Helpers/SelectionHelper.hpp"
#include "Helpers/PageHelper.hpp"
#include "GorillaUI/BaseGameInterface.hpp"

#include "Photon/Pun/PhotonNetwork.hpp"
#include "Photon/Realtime/Player.hpp"
#include "Photon/Realtime/Room.hpp"
#include "Photon/Pun/PhotonView.hpp"

#include "GlobalNamespace/GorillaParent.hpp"
#include "GlobalNamespace/VRRig.hpp"
#include "GlobalNamespace/GorillaTagManager.hpp"
#include "UnityEngine/Material.hpp"

#include <sstream>
#include <iomanip>

DEFINE_TYPE(GorillaUI::ScoreboardView);

extern Logger& getLogger();

#define MENU_PAGE_SIZE 5

using namespace UnityEngine;
using namespace Photon::Pun;
using namespace Photon::Realtime;

extern Logger& getLogger();

namespace GorillaUI
{
    void ScoreboardView::Awake()
    {
        if (!selectionHandler) selectionHandler = new UISelectionHandler(EKeyboardKey::Up, EKeyboardKey::Down, EKeyboardKey::Enter, true, true);
        playerInfos.clear();
        
    }

    void ScoreboardView::DidActivate(bool firstActivation)
    {
        std::function<void(int)> fun = std::bind(&ScoreboardView::ShowPlayer, this, std::placeholders::_1);
        selectionHandler->selectionCallback = fun;

        if (!PhotonNetwork::get_InRoom())
        {
            selectionHandler->max = 0;
        }
        else 
        {
            playerInfos.clear();
            Array<GlobalNamespace::VRRig*>* rigs = GlobalNamespace::GorillaParent::_get_instance()->GetComponentsInChildren<GlobalNamespace::VRRig*>();
            selectionHandler->max = rigs->Length();

            for (int i = 0; i < rigs->Length(); i++)
            {
                GlobalNamespace::VRRig* rig = rigs->values[i];

                PlayerDisplayInfo info;
                info.color = rig->materialsToChangeTo->values[0]->get_color();
                info.isTagged = BaseGameInterface::Player::get_isInfected(rig);
                Photon::Realtime::Player* player = rig->get_photonView() ? rig->get_photonView()->get_Owner() : nullptr;

                if (player)
                {
                    info.playerID = to_utf8(csstrtostr(player->get_UserId()));
                    info.nickName = to_utf8(csstrtostr(player->get_NickName()));
                }
                playerInfos.push_back(info);
            }

            std::sort(playerInfos.begin(), playerInfos.end(), [](PlayerDisplayInfo& first, PlayerDisplayInfo& second){ return first.nickName < second.nickName; });
        }

        Redraw();
    }

    void ScoreboardView::ShowPlayer(int index)
    {
        if (!PhotonNetwork::get_InRoom()) return;

        if (!playerViewManager) playerViewManager = CreateViewManager<PlayerViewManager*>();

        ((PlayerViewManager*)playerViewManager)->playerInfo = playerInfos[index];
        watch->activeViewManager->PresentViewManager(playerViewManager);
    }

    void ScoreboardView::Redraw()
    {
        text = "";

        DrawHeader();
        DrawPlayers();

        watch->Redraw();
    }
    
    void ScoreboardView::DrawHeader()
    {
        text += "<color=#ffff00>== <color=#fdfdfd>Scoreboard</color> ==</color>\n";
        Photon::Realtime::Room* currentRoom = PhotonNetwork::get_CurrentRoom();
        if (currentRoom)
        {
            text += string_format("<size=60>Room ID: - %s -</size>\n", !currentRoom->get_IsVisible() ? "Private" : BaseGameInterface::Room::get_roomID().c_str());
            text += "\n";
        }
    }
    
    std::string ColorToHex(Color color)
    {
        std::stringstream sstream;
        sstream << std::setfill ('0');
        sstream << std::hex << std::setw(sizeof(char)*2) << (int)(color.r * 255.0f);
        sstream << std::hex << std::setw(sizeof(char)*2) << (int)(color.g * 255.0f);
        sstream << std::hex << std::setw(sizeof(char)*2) << (int)(color.b * 255.0f);
        return sstream.str();
    }

    void ScoreboardView::DrawPlayers()
    {
        if (!PhotonNetwork::get_InRoom())
        {
            text += "<size=60>\n   You are not in a room!\n   Please enter a room for\n   the scoreboard to work\n   properly\n</size>";
        }

        std::vector<std::string> players = {};
        for (auto p : playerInfos)
        {
            std::string displayName = string_format("<color=#%s>%s</color>", p.isTagged ? "ff8888" : "ffffff", p.nickName.c_str());

            displayName += string_format(" <color=#%s>#</color>", ColorToHex(p.color).c_str());
            
            players.push_back(displayName);
        }

        SelectionHelper::DrawSelection(players, selectionHandler->currentSelectionIndex, text);
    }
    
    void ScoreboardView::OnKeyPressed(int key)
    {
        if (playerInfos.size() == 0) return;
        selectionHandler->HandleKey((EKeyboardKey)key);
        Redraw();
    }
    /*
    bool ScoreboardView::get_isTagged(GlobalNamespace::VRRig* rig)
    {
        using namespace GlobalNamespace;
        if (!PhotonNetwork::get_InRoom()) return false;
        GorillaTagManager* gorillaTagManager = GorillaTagManager::_get_instance();

        gorillaTagManager->CopyRoomDataToLocalData();

        if (gorillaTagManager->isCurrentlyTag)
        {
            return gorillaTagManager->currentIt->Equals(rig->get_photonView()->get_Owner());
        }
        else
        {
            return gorillaTagManager->currentInfected->Contains(rig->get_photonView()->get_Owner());
        }
    }
    */
}