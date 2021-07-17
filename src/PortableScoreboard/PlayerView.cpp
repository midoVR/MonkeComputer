#include "PortableScoreboard/PlayerView.hpp"
#include "PortableScoreboard/ReportView.hpp"
#include "GorillaUI.hpp"
#include "Register.hpp"
#include "ViewLib/MonkeWatch.hpp"
#include "Helpers/SelectionHelper.hpp"
#include "Helpers/PageHelper.hpp"
#include "GorillaUI/BaseGameInterface.hpp"

#include "Photon/Pun/PhotonNetwork.hpp"
#include "Photon/Pun/PhotonView.hpp"
#include "Photon/Realtime/Player.hpp"

#include "GlobalNamespace/GorillaParent.hpp"
#include "GlobalNamespace/VRRig.hpp"

#include "UnityEngine/Material.hpp"
#include "UnityEngine/PlayerPrefs.hpp"
#include "UnityEngine/Resources.hpp"

#include "GlobalNamespace/GorillaScoreBoard.hpp"
#include "GlobalNamespace/GorillaPlayerLineButton.hpp"

DEFINE_TYPE(GorillaUI, PlayerView);

extern Logger& getLogger();

#define MENU_PAGE_SIZE 5

using namespace UnityEngine;
using namespace Photon::Pun;
using namespace Photon::Realtime;

namespace GorillaUI
{
    void PlayerView::Awake()
    {
        selectionHandler = new UISelectionHandler(EKeyboardKey::Up, EKeyboardKey::Down, EKeyboardKey::Enter, true, true);
        pttHandler = new UISelectionHandler(EKeyboardKey::Left, EKeyboardKey::Right, EKeyboardKey::Enter, true, true);
        selectionHandler->max = 2;
        pttHandler->max = 3;
        pttHandler->currentSelectionIndex = BaseGameInterface::Mic::pttTypeToIndex(BaseGameInterface::Mic::get_pttType());
    }

    void PlayerView::DidActivate(bool firstActivation)
    {
        std::function<void(int)> fun = std::bind(&PlayerView::Action, this, std::placeholders::_1);
        selectionHandler->selectionCallback = fun;
        
        Redraw();
    }

    void PlayerView::Action(int index)
    {
        if (!PhotonNetwork::get_InRoom()) return;

        Il2CppString* localUsercs = PhotonNetwork::get_LocalPlayer()->get_UserId();
        std::string localUser = to_utf8(csstrtostr(localUsercs));

        // if clicked user is our player
        if (localUser.find(playerInfo.playerID) != std::string::npos) 
        {
            BaseGameInterface::SetVoiceChat(!BaseGameInterface::Voice::get_voiceChat());
            return;
        }

        GlobalNamespace::VRRig* rig = nullptr;
        std::vector<GlobalNamespace::GorillaPlayerScoreboardLine*> lines = {};
        switch (index)
        {
            // mute / unmute player
            case 0:
                rig = BaseGameInterface::Player::get_VRRig(playerInfo.playerID);
                if (!rig)
                {
                    break;
                }

                rig->muted ^= 1;

                lines = FindScoreboardLineForUserID(playerInfo.playerID);
                if (lines.size() > 0)
                {
                    for (auto l : lines)
                    {
                        l->muteButton->isOn = rig->muted;
                        l->muteButton->UpdateColor();
                    }
                }

                PlayerPrefs::SetInt(il2cpp_utils::createcsstr(playerInfo.playerID), rig->muted ? 1 : 0);
                PlayerPrefs::Save();
                
                break;
            case 1:
                // report player
                if (!reportView) reportView = CreateView<ReportView*>();
                ((ReportView*)reportView)->playerInfo = playerInfo;
                watch->activeViewManager->ReplaceTopView(reportView);
                break;
            default:
                break;
        }
    }

    void PlayerView::Redraw()
    {
        text = "";

        DrawHeader();
        DrawBody();

        watch->Redraw();
    }
    
    void PlayerView::DrawHeader()
    {
        text += "<color=#ffff00>== <color=#fdfdfd>Player</color> ==</color>\n";
    }
    
    void PlayerView::DrawBody()
    {
        auto* rig = BaseGameInterface::Player::get_VRRig(playerInfo.playerID);
        if (!rig)
        {
            text += "\nUser was no longer in this room";
            return;
        }

        Il2CppString* localUsercs = PhotonNetwork::get_LocalPlayer()->get_UserId();
        std::string localUser = to_utf8(csstrtostr(localUsercs));

        // if clicked user is our player
        if (localUser.find(playerInfo.playerID) != std::string::npos)
        {

            text += "\nThis is You!,\nyou can't mute or\nreport yourself";
            text += "\n\nPress enter to toggle:\n";
            text += string_format("Voice Chat %s", BaseGameInterface::Voice::get_voiceChat() ? "<color=#00fd00>On</color>" : "<color=#fd0000>Off</color>");
            text += "\n\n use < / > to change:\n";
            text += string_format("<color=adbdad><</color> %s <color=adbdad>></color>", BaseGameInterface::Mic::get_pttType().c_str());
            return;
        }

        text += string_format("\n Player: %s\n\n", playerInfo.nickName.c_str());

        bool isMuted = rig->muted; 

        std::vector<std::string> options = {
            isMuted ? "Unmute" : "Mute",
            "Report"
        };

        SelectionHelper::DrawSelection(options, selectionHandler->currentSelectionIndex, text);

        text += string_format("\n\n Color (0 - 9):\n %d %d %d\n", (int)(playerInfo.color.r * 9.0f), (int)(playerInfo.color.g * 9.0f), (int)(playerInfo.color.b * 9.0f));
        text += string_format("\n Color (0 - 255):\n %d %d %d\n", (int)(playerInfo.color.r * 255.0f), (int)(playerInfo.color.g * 255.0f), (int)(playerInfo.color.b * 255.0f));
    }
    
    void PlayerView::OnKeyPressed(int key)
    {
        selectionHandler->HandleKey((EKeyboardKey)key);

        Il2CppString* localUsercs = PhotonNetwork::get_LocalPlayer()->get_UserId();
        std::string localUser = to_utf8(csstrtostr(localUsercs));

        if (localUser.find(playerInfo.playerID) != std::string::npos) 
        {
            pttHandler->currentSelectionIndex = BaseGameInterface::Mic::pttTypeToIndex(BaseGameInterface::Mic::get_pttType());
            if (pttHandler->HandleKey((EKeyboardKey)key))
            {
                BaseGameInterface::SetpttType(pttHandler->currentSelectionIndex);
            }
        }

        Redraw();
    }

    std::vector<GlobalNamespace::GorillaPlayerScoreboardLine*> PlayerView::FindScoreboardLineForUserID(std::string userID)
    {
        using namespace GlobalNamespace;
        Array<GorillaScoreBoard*>* scoreboards = Resources::FindObjectsOfTypeAll<GorillaScoreBoard*>();
        
        std::vector<GlobalNamespace::GorillaPlayerScoreboardLine*> myLines = {};

        Il2CppString* userIDcs = il2cpp_utils::createcsstr(userID);
        for (int i = 0; i < scoreboards->Length(); i++)
        {
            GorillaScoreBoard* board = scoreboards->values[i];

            Array<GorillaPlayerScoreboardLine*>* lines = board->GetComponentsInChildren<GorillaPlayerScoreboardLine*>();
            if (!lines) continue;

            for (int j = 0; j < lines->Length(); j++)
            {
                GorillaPlayerScoreboardLine* line = lines->values[j];
                
                // if we found the line for the player we want to check
                if (userIDcs->Contains(line->linePlayer->get_UserId()))
                {
                    myLines.push_back(line);
                }
            }
        }

        if (myLines.size() == 0)
            getLogger().info("Couldn't find scoreboardline for player %s", userID.c_str());
        return myLines;
    } 
}