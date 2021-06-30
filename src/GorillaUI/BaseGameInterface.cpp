#include "GorillaUI/BaseGameInterface.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "beatsaber-hook/shared/utils/utils.h"
#include <vector>
#include <algorithm>

#include "Photon/Pun/PhotonNetwork.hpp"
#include "Photon/Pun/PhotonView.hpp"
#include "Photon/Pun/RpcTarget.hpp"
#include "Photon/Realtime/Player.hpp"
#include "Photon/Realtime/Room.hpp"

#include "UnityEngine/PlayerPrefs.hpp"
#include "UnityEngine/UI/Text.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/SkinnedMeshRenderer.hpp"

#include "GorillaLocomotion/Player.hpp"

#include "GlobalNamespace/GorillaNetworkJoinTrigger.hpp"
#include "GlobalNamespace/GorillaLevelScreen.hpp"
#include "GlobalNamespace/GorillaFriendCollider.hpp"
#include "GlobalNamespace/GorillaComputer.hpp"
#include "GlobalNamespace/GorillaTagManager.hpp"
#include "GlobalNamespace/GorillaTagger.hpp"
#include "GlobalNamespace/GorillaParent.hpp"
#include "GlobalNamespace/GorillaScoreboardSpawner.hpp"
#include "GlobalNamespace/VRRig.hpp"
#include "GlobalNamespace/PhotonNetworkController.hpp"

#include "System/Collections/Generic/Dictionary_2.hpp"
#include "ExitGames/Client/Photon/Hashtable.hpp"


#include "UnityEngine/XR/Interaction/Toolkit/GorillaSnapTurn.hpp"
#include "gorilla-utils/shared/Utils/RPC.hpp"

extern Logger& getLogger();

using namespace UnityEngine;
using namespace GlobalNamespace;
using namespace Photon::Pun;
using namespace Photon::Realtime;

#define RPC(val...) GorillaUtils::RPC::RPC(val);

namespace GorillaUI::BaseGameInterface
{
    static Il2CppString* redValue = nullptr;
    static Il2CppString* greenValue = nullptr;
    static Il2CppString* blueValue = nullptr;
    static Il2CppString* playerName = nullptr;
    static Il2CppString* stickTurning = nullptr;
    static Il2CppString* turnFactor = nullptr;
    static Il2CppString* pttType = nullptr;
    static Il2CppString* currentQueue = nullptr;
    static Il2CppString* groupMapJoin = nullptr;
    static Il2CppString* voiceChatOn = nullptr;

    static float* red = nullptr;
    static float* green = nullptr;
    static float* blue = nullptr;

    void SetColor(float r, float g, float b)
    {
        getLogger().info("Updating color");
        using namespace GlobalNamespace;

        if (!redValue) redValue = il2cpp_utils::createcsstr("redValue", il2cpp_utils::StringType::Manual);
        if (!greenValue) greenValue = il2cpp_utils::createcsstr("greenValue", il2cpp_utils::StringType::Manual);
        if (!blueValue) blueValue = il2cpp_utils::createcsstr("blueValue", il2cpp_utils::StringType::Manual);

        PlayerPrefs::SetFloat(redValue, r);
        PlayerPrefs::SetFloat(greenValue, g);
        PlayerPrefs::SetFloat(blueValue, b);

        GorillaTagger* gorillaTagger = GorillaTagger::get_Instance();
        gorillaTagger->UpdateColor(r, g, b);
        PlayerPrefs::Save();

        if (PhotonNetwork::get_InRoom())
        {
            VRRig* myVRRig = gorillaTagger->myVRRig;
            RPC(PhotonView::Get(myVRRig), "InitializeNoobMaterial", RpcTarget::All, r, g, b);
        }
    }

    bool SetName(std::string name)
    {
        GorillaComputer* gorillaComputer = GorillaComputer::_get_instance();
        if (!gorillaComputer) return false;

        Il2CppString* csName = il2cpp_utils::createcsstr(name);

        if (!gorillaComputer->CheckAutoBanList(csName)) return false;

        PhotonNetwork::get_LocalPlayer()->set_NickName(csName);

        gorillaComputer->offlineVRRigNametagText->set_text(csName);
        gorillaComputer->savedName = csName;

        if (!playerName) playerName = il2cpp_utils::createcsstr("playerName", il2cpp_utils::StringType::Manual);
        PlayerPrefs::SetString(playerName, csName);
        PlayerPrefs::Save();

        if (PhotonNetwork::get_InRoom())
        {
            GorillaTagger* gorillaTagger = GorillaTagger::get_Instance();
            VRRig* myVRRig = gorillaTagger->myVRRig;
            RPC(PhotonView::Get(myVRRig), "InitializeNoobMaterial", RpcTarget::All, gorillaComputer->redValue, gorillaComputer->greenValue, gorillaComputer->blueValue);
        }
        return true;
    }

    void Disconnect()
    {
        if (PhotonNetwork::get_InRoom())
        {
            PhotonNetworkController::_get_instance()->AttemptDisconnect();
        }
    }

    void JoinRoom(std::string roomID)
    {
        GorillaComputer* gorillaComputer = GorillaComputer::_get_instance();
        if (!gorillaComputer) return;
        if (roomID == "") return;        
        PhotonNetworkController* networkController = gorillaComputer->networkController;

        Photon::Realtime::Room* currentRoom = PhotonNetwork::get_CurrentRoom();

        std::string currentRoomName = "";
        if (currentRoom)
        {
            Il2CppString* currentRoomCsName = currentRoom->get_Name();
            currentRoomName = to_utf8(csstrtostr(currentRoomCsName));
        }

        if (currentRoomName != roomID)
        {
            PhotonNetworkController::_get_instance()->AttemptToJoinSpecificRoom(il2cpp_utils::createcsstr(roomID));
        }
    }

    void ChangeTurnMode(std::string turnType, int turnValue)
    {
        Il2CppString* turnTypeCS = il2cpp_utils::createcsstr(turnType);

        if (!stickTurning) stickTurning = il2cpp_utils::createcsstr("stickTurning", il2cpp_utils::StringType::Manual);
        if (!turnFactor) turnFactor = il2cpp_utils::createcsstr("turnFactor", il2cpp_utils::StringType::Manual);

        PlayerPrefs::SetString(stickTurning, turnTypeCS);
        PlayerPrefs::SetInt(turnFactor, turnValue);

        PlayerPrefs::Save();

        GorillaComputer* gorillaComputer = GorillaComputer::_get_instance();

        UnityEngine::XR::Interaction::Toolkit::GorillaSnapTurn* gorillaTurn = gorillaComputer->gorillaTurn;

        gorillaComputer->turnType = turnTypeCS;
        gorillaComputer->turnValue = turnValue;
        if (gorillaTurn) gorillaTurn->ChangeTurnMode(turnTypeCS, turnValue);
    }

    void InitColorState()
    {
        Color color = PlayerColor::get_color();

        GorillaTagger* gorillaTagger = GorillaTagger::get_Instance();
        gorillaTagger->UpdateColor(color.r, color.g, color.b);
    }

    void InitNameState()
    {
        if (!playerName) playerName = il2cpp_utils::createcsstr("playerName", il2cpp_utils::StringType::Manual);
        Il2CppString* csName = PlayerPrefs::GetString(playerName, il2cpp_utils::createcsstr(string_format("gorilla%d", rand() % 10000)));
        std::string name = to_utf8(csstrtostr(csName));
        SetName(name);
    }

    void InitTurnState()
    {
        if (!stickTurning) stickTurning = il2cpp_utils::createcsstr("stickTurning", il2cpp_utils::StringType::Manual);
        if (!turnFactor) turnFactor = il2cpp_utils::createcsstr("turnFactor", il2cpp_utils::StringType::Manual);

        GorillaTagger* gorillaTagger = GorillaTagger::get_Instance();
        using namespace UnityEngine::XR::Interaction::Toolkit;
        GorillaSnapTurn* gorillaTurn = gorillaTagger->GetComponent<GorillaSnapTurn*>();
        Il2CppString* turnType = PlayerPrefs::GetString(stickTurning, il2cpp_utils::createcsstr("NONE"));
        int turnValue = PlayerPrefs::GetInt(turnFactor, 4);
        gorillaTurn->ChangeTurnMode(turnType, turnValue);
    }

    void InitAll()
    {
        InitColorState();
        InitNameState();
        InitTurnState();
    }

    std::string get_gameVersion()
    {
        PhotonNetworkController* photonNetworkController = PhotonNetworkController::_get_instance();
        Il2CppString* gameVersion = photonNetworkController->gameVersion;
        return to_utf8(csstrtostr(gameVersion));
    }

    void SetpttType(int index)
    {
        SetpttType(Mic::indexToPttType(index));
    }

    void SetpttType(std::string pttType)
    {
        if (!BaseGameInterface::pttType) BaseGameInterface::pttType = il2cpp_utils::createcsstr("pttType", il2cpp_utils::StringType::Manual);
        Il2CppString* pttTypeCS = il2cpp_utils::createcsstr(pttType);
        GorillaComputer* gorillaComputer = GorillaComputer::_get_instance();
        gorillaComputer->pttType = pttTypeCS;

        PlayerPrefs::SetString(BaseGameInterface::pttType, pttTypeCS);
        PlayerPrefs::Save();
    }

    void SetQueue(std::string queue)
    {
        if (!currentQueue) currentQueue = il2cpp_utils::createcsstr("currentQueue", il2cpp_utils::StringType::Manual);
        Il2CppString* queueCS = il2cpp_utils::createcsstr(queue);
        GorillaComputer* gorillaComputer = GorillaComputer::_get_instance();
        gorillaComputer->currentQueue = queueCS;

        PlayerPrefs::SetString(currentQueue, queueCS);
        PlayerPrefs::Save();
    }

    void SetGroup(std::string group)
    {
        if (!groupMapJoin) groupMapJoin = il2cpp_utils::createcsstr("groupMapJoin", il2cpp_utils::StringType::Manual);
        GorillaComputer* gorillaComputer = GorillaComputer::_get_instance();
        Il2CppString* toJoin = il2cpp_utils::createcsstr(group);
        gorillaComputer->groupMapJoin = toJoin;

        PlayerPrefs::SetString(groupMapJoin, toJoin);

        bool inRoom = PhotonNetwork::get_InRoom();
        
        Photon::Realtime::Room* currentRoom = PhotonNetwork::get_CurrentRoom();
        bool isVisible = currentRoom ? currentRoom->get_IsVisible() : false;

        bool flag = inRoom && !isVisible && (group == "FOREST" || group == "CAVE" || group == "CANYON");

		if (flag)
		{
            PhotonNetworkController* networkController = gorillaComputer->networkController;

            // get all player ids for checking who to send
            List<Il2CppString*>* playerIDsCurrentlyTouching = gorillaComputer->friendJoinCollider->playerIDsCurrentlyTouching;
            std::vector<std::string> playerIDsCurrentlyTouchingVector = {};

            for (int i = 0; i < playerIDsCurrentlyTouching->size; i++)
            {
                Il2CppString* playerIDcs = playerIDsCurrentlyTouching->items->values[i];
                if (!playerIDcs) continue;
                std::string playerID = to_utf8(csstrtostr(playerIDcs));
                playerIDsCurrentlyTouchingVector.push_back(playerID);
            }
            
            // set list
            networkController->friendIDList = playerIDsCurrentlyTouching;

            static Il2CppString* empty = il2cpp_utils::createcsstr("", il2cpp_utils::StringType::Manual);
            networkController->currentGameType = empty;

            Array<Photon::Realtime::Player*>* playerList = PhotonNetwork::get_PlayerList();

            Photon::Realtime::Player* localPlayer = PhotonNetwork::get_LocalPlayer();
            GorillaTagManager* gorillaTagManager = GorillaTagManager::_get_instance();
            PhotonView* photonView = gorillaTagManager->get_photonView();

            for (int i = 0; i < playerList->Length(); i++)
            {
                Photon::Realtime::Player* roomPlayer = playerList->values[i];
                if (!roomPlayer) continue;
                Il2CppString* userIDCS = roomPlayer->get_UserId();
                std::string userID = to_utf8(csstrtostr(userIDCS));
                std::vector<std::string>::iterator it = std::find(playerIDsCurrentlyTouchingVector.begin(), playerIDsCurrentlyTouchingVector.end(), userID);

                // if player is close, that means join them
                if (it != playerIDsCurrentlyTouchingVector.end() && !roomPlayer->Equals(localPlayer))
                {
                    RPC(photonView, "JoinPubWithFreinds", roomPlayer);
                }
            }

            PhotonNetwork::SendAllOutgoingCommands();

            GorillaNetworkJoinTrigger* triggeredTrigger = nullptr;

			if (group == "FOREST")
			{
				triggeredTrigger = gorillaComputer->forestMapTrigger;
			}
			else if (group == "CAVE")
			{
				triggeredTrigger = gorillaComputer->caveMapTrigger;
			}
            else if (group == "CANYON")
			{
				triggeredTrigger = gorillaComputer->canyonMapTrigger;
			}

            PhotonNetworkController::_get_instance()->AttemptJoinPublicWithFriends(triggeredTrigger);
		}
    }

    void SetVoiceChat(bool value)
    {
        GorillaComputer* gorillaComputer = GorillaComputer::_get_instance();
        if (!gorillaComputer) return;

        if (!voiceChatOn) voiceChatOn = il2cpp_utils::createcsstr("voiceChatOn", il2cpp_utils::StringType::Manual);

        std::string stringToSet = value ? "TRUE" : "FALSE";
        Il2CppString* stringToSetCS = il2cpp_utils::createcsstr(stringToSet);

        gorillaComputer->voiceChatOn = stringToSetCS;
        PlayerPrefs::SetString(voiceChatOn, stringToSetCS);
        PlayerPrefs::Save();
    }


    namespace SnapTurn
    {
        int get_turnValue()
        {
            if (!turnFactor) turnFactor = il2cpp_utils::createcsstr("turnFactor", il2cpp_utils::StringType::Manual);
            return PlayerPrefs::GetInt(turnFactor, 4);
        }

        void set_turnValue(int turnValue)
        {
            if (!turnFactor) turnFactor = il2cpp_utils::createcsstr("turnFactor", il2cpp_utils::StringType::Manual);
            PlayerPrefs::SetInt(turnFactor, turnValue);
        }

        std::string get_turnType()
        {
            if (!stickTurning) stickTurning = il2cpp_utils::createcsstr("stickTurning", il2cpp_utils::StringType::Manual);
            static Il2CppString* defaultValue = il2cpp_utils::createcsstr("NONE");
            Il2CppString* turnTypeCS = PlayerPrefs::GetString(stickTurning, defaultValue);
            return to_utf8(csstrtostr(turnTypeCS));
        }

        void set_turnType(std::string turnType)
        {
            if (!stickTurning) stickTurning = il2cpp_utils::createcsstr("stickTurning", il2cpp_utils::StringType::Manual);
            PlayerPrefs::SetString(stickTurning, il2cpp_utils::createcsstr(turnType));
        }

        int turnTypeToIndex(std::string turnType)
        {
            if (turnType.find("SNAP") != std::string::npos) return 0;
            else if (turnType.find("SMOOTH") != std::string::npos) return 1;
            else return 2;
        }
    }

    namespace PlayerColor
    {
        Color get_color()
        {
            if (!redValue) redValue = il2cpp_utils::createcsstr("redValue", il2cpp_utils::StringType::Manual);
            if (!greenValue) greenValue = il2cpp_utils::createcsstr("greenValue", il2cpp_utils::StringType::Manual);
            if (!blueValue) blueValue = il2cpp_utils::createcsstr("blueValue", il2cpp_utils::StringType::Manual);

            float r = PlayerPrefs::GetFloat(redValue, 0.0f);
            float g = PlayerPrefs::GetFloat(greenValue, 0.0f);
            float b = PlayerPrefs::GetFloat(blueValue, 0.0f);
            return {r, g, b};
        }

        Array<Il2CppObject*>* get_colorArray(float r, float g, float b)
        {
            Array<Il2CppObject*>* arr = reinterpret_cast<Array<Il2CppObject*>*>(il2cpp_functions::array_new(classof(Il2CppObject*), 3));

            if (!red) red = new float;
            if (!green) green = new float;
            if (!blue) blue = new float;

            *red = r;
            *green = g;
            *blue = b;
            
            arr->values[0] = il2cpp_functions::value_box(classof(float), red);
            arr->values[1] = il2cpp_functions::value_box(classof(float), green);
            arr->values[2] = il2cpp_functions::value_box(classof(float), blue);

            return arr;
        }
    }

    namespace Name
    {
        std::string get_name()
        {
            if (!playerName) playerName = il2cpp_utils::createcsstr("playerName", il2cpp_utils::StringType::Manual);
            Il2CppString* csMonke = PlayerPrefs::GetString(playerName, il2cpp_utils::createcsstr(string_format("gorilla%d", rand() % 10000)));
            std::string monke = to_utf8(csstrtostr(csMonke));
            return monke;
        }
    }

    namespace Room
    {
        bool get_isConnectedToMaster()
        {
            return GorillaComputer::_get_instance()->isConnectedToMaster;
        }

        std::string get_roomID()
        {
            std::string currentRoomName = "";

            Photon::Realtime::Room* currentRoom = PhotonNetwork::get_CurrentRoom();

            if (currentRoom)
            {
                Il2CppString* currentRoomCsName = currentRoom->get_Name();
                currentRoomName = to_utf8(csstrtostr(currentRoomCsName));
            }

            return currentRoomName;
        }

        int get_currentPlayers()
        {
            return PhotonNetworkController::_get_instance()->TotalUsers();
        }

        char get_playerCount()
        {
            Photon::Realtime::Room* currentRoom = Photon::Pun::PhotonNetwork::get_CurrentRoom();
            if (!currentRoom) return 0;
            return currentRoom->get_PlayerCount();
        }

        int get_collidingPlayers()
        {
            GorillaFriendCollider* friendJoinCollider = GorillaComputer::_get_instance()->friendJoinCollider;
            return friendJoinCollider->playerIDsCurrentlyTouching->size;
        }

        int get_bannedPlayers()
        {
            GorillaComputer* gorillaComputer = GorillaComputer::_get_instance();
            if (!gorillaComputer) return -1;
            return gorillaComputer->usersBanned;
        }

        bool get_roomFull()
        {
            GorillaComputer* gorillaComputer = GorillaComputer::_get_instance();
            return (gorillaComputer ? gorillaComputer->roomFull : false) && !PhotonNetwork::get_InRoom();
        }
    }

    namespace Mic
    {
        std::string get_pttType()
        {
            if (!BaseGameInterface::pttType) BaseGameInterface::pttType = il2cpp_utils::createcsstr("pttType", il2cpp_utils::StringType::Manual);
            Il2CppString* pttType = PlayerPrefs::GetString(BaseGameInterface::pttType, il2cpp_utils::createcsstr("ALL CHAT"));
            return to_utf8(csstrtostr(pttType));
        }

        int pttTypeToIndex(std::string pttType)
        {
            if (pttType == "PUSH TO MUTE") return 2;
            else if (pttType == "PUSH TO TALK") return 1;
            else return 0;
        }

        std::string indexToPttType(int index)
        {
            switch(index)
            {
                case 0:
                    return "ALL CHAT";
                    break;
                case 1:
                    return "PUSH TO TALK";
                    break;
                case 2:
                    return "PUSH TO MUTE";
                    break;
                default:
                    return "ALL CHAT";
                    break;
            }
        }

    }

    namespace Queue
    {
        std::string get_Queue()
        {
            if (!currentQueue) currentQueue = il2cpp_utils::createcsstr("currentQueue", il2cpp_utils::StringType::Manual);
            Il2CppString* currentQueueCS = PlayerPrefs::GetString(currentQueue, il2cpp_utils::createcsstr("DEFAULT"));
            return to_utf8(csstrtostr(currentQueueCS));
        }

        int queueToIndex(std::string queue)
        {
            if (queue == "CASUAL") return 2;
            else if (queue == "COMPETITIVE") return 1;
            else return 0;
        }
    }

    namespace Group
    {
        std::string get_group()
        {
            if (!groupMapJoin) groupMapJoin = il2cpp_utils::createcsstr("groupMapJoin", il2cpp_utils::StringType::Manual);
            Il2CppString* currentGroup = PlayerPrefs::GetString(groupMapJoin, il2cpp_utils::createcsstr("FOREST"));
            return to_utf8(csstrtostr(currentGroup));
        }

        int groupToIndex(std::string group)
        {
            if (group == "CAVE") return 1;
            else if (group == "CANYON") return 2;
            else return 0;
        }
    }

    namespace Voice
    {
        bool get_voiceChat()
        {
            if (!voiceChatOn) voiceChatOn = il2cpp_utils::createcsstr("voiceChatOn", il2cpp_utils::StringType::Manual);
            static Il2CppString* defaultVal = il2cpp_utils::createcsstr("TRUE", il2cpp_utils::StringType::Manual);

            Il2CppString* value = PlayerPrefs::GetString(voiceChatOn, defaultVal);
            return value ? value->Contains(defaultVal) : true;
        }
    }

    namespace Player
    {
        GlobalNamespace::VRRig* get_VRRig(std::string userID)
        {
            Array<GlobalNamespace::VRRig*>* rigs = GlobalNamespace::GorillaParent::_get_instance()->GetComponentsInChildren<GlobalNamespace::VRRig*>();

            for (int i = 0; i < rigs->Length(); i++)
            {
                GlobalNamespace::VRRig* rig = rigs->values[i];
                Photon::Realtime::Player* player = rig->get_photonView() ? rig->get_photonView()->get_Owner() : nullptr;

                if (player && userID.find(to_utf8(csstrtostr(player->get_UserId()))) != std::string::npos) return rig;
            }

            return nullptr;
        }

        bool get_isInfected()
        {
            return get_isInfected(PhotonNetwork::get_LocalPlayer());
        }

        bool get_isInfected(std::string userID)
        {
            return get_isInfected(get_VRRig(userID));
        }

        bool get_isInfected(GlobalNamespace::VRRig* rig)
        {
            if (!rig) return false;
            Photon::Realtime::Player* player = rig->get_photonView()->get_Owner();

            return rig->setMatIndex > 0;
        }

        bool get_isInfected(Photon::Realtime::Player* player)
        {
            if (!player) return false;
            using namespace GlobalNamespace;
            if (!PhotonNetwork::get_InRoom()) return false;
            GorillaTagManager* gorillaTagManager = GorillaTagManager::_get_instance();

            gorillaTagManager->CopyRoomDataToLocalData();

            if (gorillaTagManager->isCurrentlyTag)
            {
                return gorillaTagManager->currentIt->Equals(player);
            }
            else
            {
                return gorillaTagManager->currentInfected->Contains(player);
            }
        }   
    }
}