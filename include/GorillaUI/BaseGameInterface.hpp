#pragma once
#include <string>

#include "UnityEngine/Color.hpp"

namespace GlobalNamespace
{
    class VRRig;
}

namespace Photon::Realtime
{
    class Player;
}

namespace GorillaUI::BaseGameInterface
{
    void SetColor(float r, float g, float b);
    bool SetName(std::string name);
    void Disconnect();
    void JoinRoom(std::string roomID);
    void SetpttType(int index);
    void SetpttType(std::string pttType);
    void SetQueue(std::string queue);
    void SetGroup(std::string group);
    void InitColorState();
    void InitNameState();
    void InitTurnState();
    void InitAll();
    void ChangeTurnMode(std::string turnType, int turnValue);
    void SetVoiceChat(bool value);
    std::string get_gameVersion();

    namespace SnapTurn
    {
        int get_turnValue();
        void set_turnValue(int turnValue);

        std::string get_turnType();
        void set_turnType(std::string turnType);

        int turnTypeToIndex(std::string turnType);
    }

    namespace PlayerColor
    {
        UnityEngine::Color get_color();
        Array<Il2CppObject*>* get_colorArray(float r, float g, float b);
    }

    namespace Name
    {
        std::string get_name();
    }

    namespace Room
    {
        bool get_isConnectedToMaster();
        std::string get_roomID();
        /// @brief current online players
        int get_currentPlayers();

        /// @brief players in lobby
        char get_playerCount();

        /// @brief players close enough to the pc for group transfer
        int get_collidingPlayers();

        /// @brief count of players banned yesterday
        int get_bannedPlayers();

        bool get_roomFull();
    }

    namespace Mic
    {
        std::string get_pttType();
        int pttTypeToIndex(std::string pttType);
        std::string indexToPttType(int index);
    }

    namespace Queue
    {
        std::string get_Queue();
        int queueToIndex(std::string queue);
    }

    namespace Group
    {
        std::string get_group();
        int groupToIndex(std::string group);
    }

    namespace Voice
    {
        bool get_voiceChat();
    }

    namespace Player
    {
        GlobalNamespace::VRRig* get_VRRig(std::string userID);
        bool get_isInfected();
        bool get_isInfected(GlobalNamespace::VRRig* rig);
        bool get_isInfected(Photon::Realtime::Player* player);
        bool get_isInfected(std::string userID);

    }
}
