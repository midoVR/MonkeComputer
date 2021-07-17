#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include "beatsaber-hook/shared/utils/utils.h"
#include "beatsaber-hook/shared/utils/il2cpp-type-check.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
#include "beatsaber-hook/shared/utils/hooking.hpp"

#include "custom-types/shared/register.hpp"
#include "GorillaUI.hpp"
#include "GorillaUI/CommandLineView.hpp"
#include "GorillaUI/DetailView.hpp"
#include "GorillaUI/WatchDetailView.hpp"
#include "GorillaUI/MainView.hpp"
#include "GorillaUI/MainWatchView.hpp"
#include "GorillaUI/MainViewManager.hpp"
#include "GorillaUI/ModSettingsView/ModSettingsView.hpp"
#include "GorillaUI/ModSettingsView/ModSettingsViewManager.hpp"

#include "GorillaUI/BaseGameViews/BaseGameViewManager.hpp"
#include "GorillaUI/BaseGameViews/BaseGameView.hpp"
#include "GorillaUI/BaseGameViews/ColorChangeView.hpp"
#include "GorillaUI/BaseGameViews/CustomRoomView.hpp"
#include "GorillaUI/BaseGameViews/TurnChangeView.hpp"
#include "GorillaUI/BaseGameViews/NameChangeView.hpp"
#include "GorillaUI/BaseGameViews/QueueChangeView.hpp"
#include "GorillaUI/BaseGameViews/MicChangeView.hpp"
#include "GorillaUI/BaseGameViews/GroupChangeView.hpp"
#include "GorillaUI/BaseGameViews/VoiceChatView.hpp"

#include "PortableScoreboard/PlayerView.hpp"
#include "PortableScoreboard/ReportView.hpp"
#include "PortableScoreboard/PlayerViewManager.hpp"
#include "PortableScoreboard/ScoreboardView.hpp"
#include "PortableScoreboard/ScoreboardViewManager.hpp"

#include "MonkeComputerConfigView.hpp"

#include "ViewLib/CustomComputer.hpp"
#include "ViewLib/MonkeWatch.hpp"
#include "ViewLib/MonkeWatchButton.hpp"
#include "ViewLib/WatchActivatorTrigger.hpp"

#include "Register.hpp"
#include "CommandRegister.hpp"

#include "BillboardedWatch.hpp"
#include "CustomQueues.hpp"
#include "GorillaUI/CustomQueueView.hpp"
#include "GorillaUI/BaseGameInterface.hpp"

#include <vector>
#include <stdio.h>
#include <string.h>

#include "config.hpp"

#include "UnityEngine/Application.hpp"
#include "UnityEngine/GameObject.hpp"
#include "GlobalNamespace/GorillaComputer.hpp"
#include "GorillaLocomotion/Player.hpp"
#include "PlayFab/PlayFabError.hpp"
#include "System/Collections/Generic/Dictionary_2.hpp"
#include "System/Collections/Generic/KeyValuePair_2.hpp"
#include "System/DateTime.hpp"
#include "System/TimeSpan.hpp"

#include "GlobalNamespace/PlayFabAuthenticator.hpp"
#include "GlobalNamespace/TransformFollow.hpp"

#include "CustomBackgrounds/BackgroundsList.hpp"
#include "CustomBackgrounds/BackgroundsView.hpp"

#include "BanManager.hpp"
#include "OutdatedManager.hpp"

#include <stdlib.h>

#include "Photon/Pun/PhotonNetwork.hpp"

#include "gorilla-utils/shared/Callbacks/MatchMakingCallbacks.hpp"
#include "gorilla-utils/shared/Callbacks/InRoomCallbacks.hpp"
#include "gorilla-utils/shared/CustomProperties/Room.hpp"
#include "gorilla-utils/shared/CustomProperties/Player.hpp"
#include "gorilla-utils/shared/Utils/Player.hpp"
#include "gorilla-utils/shared/GorillaUtils.hpp"

#include "Utils/TextUtils.hpp"

using namespace GorillaUI;
using namespace GorillaUI::Components;

ModInfo modInfo = {ID, VERSION};

Logger& getLogger()
{
    static Logger *logger = new Logger(modInfo, LoggerOptions(false, true));
    return *logger;
}

#define INFO(value...) getLogger().info(value)
#define ERROR(value...) getLogger().error(value)

using namespace UnityEngine;
using namespace GlobalNamespace;
using namespace GorillaLocomotion;
using Hashtable_Base = System::Collections::Generic::Dictionary_2<Il2CppObject*, Il2CppObject*>;

MAKE_HOOK_MATCH(GorillaComputer_Start, &GorillaComputer::Start, void, GorillaComputer* self)
{
    GorillaComputer_Start(self);
    GameObject* computerGO = self->get_gameObject();
    CustomComputer* computer = computerGO->AddComponent<CustomComputer*>();
    computer->Init(CreateView<MainView*>());
}

MAKE_HOOK_MATCH(GorillaComputer_CheckAutoBanList, &GorillaComputer::CheckAutoBanList, bool, GorillaComputer* self, Il2CppString* nameToCheck)
{   
    INFO("Anywhere Two Week Ban List: ");
    for (int i = 0; i < self->anywhereTwoWeek->Length(); i++)
    {
        std::string name = to_utf8(csstrtostr(self->anywhereTwoWeek->values[i]));
        INFO("\t%s", name.c_str());
    }

    INFO("Anywhere One Week Ban List: ");
    for (int i = 0; i < self->anywhereOneWeek->Length(); i++)
    {
        std::string name = to_utf8(csstrtostr(self->anywhereOneWeek->values[i]));
        INFO("\t%s", name.c_str());
    }

    INFO("Exact One Week Ban List: ");
    for (int i = 0; i < self->exactOneWeek->Length(); i++)
    {
        std::string name = to_utf8(csstrtostr(self->exactOneWeek->values[i]));
        INFO("\t%s", name.c_str());
    }

    return GorillaComputer_CheckAutoBanList(self, nameToCheck);
}

MAKE_HOOK_MATCH(GorillaComputer_BanMe, &GorillaComputer::BanMe, void, GorillaComputer* self, int hours, Il2CppString* nameToCheck)
{
    std::string name = to_utf8(csstrtostr(nameToCheck));
    INFO("Player Tried setting name %s, but a ban of %d hours was prevented", name.c_str(), hours);
}

MAKE_HOOK_MATCH(PlayFabAuthenticator_OnPlayFabError, &GlobalNamespace::PlayFabAuthenticator::OnPlayFabError, void, GlobalNamespace::PlayFabAuthenticator* self, PlayFab::PlayFabError* obj)
{
    if (!obj || !obj->ErrorMessage) 
    {
        PlayFabAuthenticator_OnPlayFabError(self, obj);
        return;
    }
    std::string error = to_utf8(csstrtostr(obj->ErrorMessage));

    using DetailsEnumerator = System::Collections::Generic::Dictionary_2<Il2CppString*, System::Collections::Generic::List_1<Il2CppString*>*>::Enumerator;
    using Details = System::Collections::Generic::KeyValuePair_2<Il2CppString*, List<Il2CppString*>*>;
    using namespace System;
    
    bool isBanned = false;
    std::string DisplayedMessage = "";
    
    // normal ban
    if (error == "The account making this request is currently banned")
    {
        isBanned = true;

        // C# enumeration, kinda weird innit
        DetailsEnumerator enumerator = obj->ErrorDetails->GetEnumerator();
        if (enumerator.MoveNext())
        {
            Details pair = enumerator.get_Current();
            Il2CppString* firstCS = pair.get_Value()->items->values[0];
            std::string first = firstCS ? to_utf8(csstrtostr(firstCS)) : "";
            std::string reason = to_utf8(csstrtostr(pair.get_Key()));

            // if not indefinite, so it will expirre within about 2 weeks
            if (first != "Indefinite")
            {   

                DateTime end = DateTime::Parse(firstCS); 
                TimeSpan time = end - DateTime::get_UtcNow();

                int count = time.get_TotalHours() + 1;

                DisplayedMessage += "You have been banned. You will not be able to play until the ban expires.\nReason: ";
                DisplayedMessage += reason;
                DisplayedMessage += string_format("\nHours Left: %d", count);
            }
            // infinte ban, this is so sad, alexa play despacito
            else
            {
                DisplayedMessage += "You have been banned Indefinitely.\nReason: ";
                DisplayedMessage += reason;
            }
        }
    }
    // ip ban
    else if (error == "The IP making this request is currently banned")
    {
        isBanned = true;
        DisplayedMessage = "<size=40>\nYour IP has been banned,\n due to the possibility of ban evasion nobody else\n from this IP will be able to connect until the ban expires.\n Only the banned user can be shown the reason and ban duration.\n</size>";
    }

    if (isBanned)
    {
        BanManager::set_displayMessage(DisplayedMessage);
        
        std::vector<ModEntry>& entries = Register::get_entries();
        
        for (auto& entry : entries)
        {
            if (entry.get_info().id == "Details")
            {
                CustomComputer* pc = CustomComputer::get_instance();
                if (pc)
                    pc->activeViewManager->ReplaceTopView(entry.get_view());
                break;
            }
        }
    }
    PlayFabAuthenticator_OnPlayFabError(self, obj);
}

MAKE_HOOK_MATCH(GorillaComputer_GeneralFailureMessage, &GlobalNamespace::GorillaComputer::GeneralFailureMessage, void, GlobalNamespace::GorillaComputer* self, Il2CppString* failMessage)
{
    GorillaComputer_GeneralFailureMessage(self, failMessage);
    if (failMessage && failMessage->Equals(self->versionMismatch)) // if the fail message is the version mismatch text
    {
        std::string fail = to_utf8(csstrtostr(failMessage));
        OutdatedManager::set_displayMessage("Sadly, your game is outdated\nIf you wish to update your game then delete it\nusing the tools in questpatcher, and reinstall.\nAfter reinstalling you can then patch again\nAfter updates mods can break,\nplease be patient while they are being updated!");

        std::vector<ModEntry>& entries = Register::get_entries();

        for (auto& entry : entries)
        {
            if (entry.get_info().id == "Details")
            {
                CustomComputer* pc = CustomComputer::get_instance();
                if (pc)
                    pc->activeViewManager->ReplaceTopView(entry.get_view());
                break;
            }
        }
    }
}

MAKE_HOOK_MATCH(VRRig_NormalizeName, &GlobalNamespace::VRRig::NormalizeName, Il2CppString*, GlobalNamespace::VRRig* self, bool doIt, Il2CppString* text)
{
    return VRRig_NormalizeName(self, false, text);
}

extern "C" void setup(ModInfo& info)
{
    info.id = ID;
    info.version = VERSION;
}

void RegisterCommands();
void AddQueues();

void loadlib()
{
    INFO("Loading mod...");
    if (!LoadConfig())
            SaveConfig();
    BackgroundsList::Load();

    GorillaUtils::Init();

    Logger& logger = getLogger();
    INSTALL_HOOK(logger, GorillaComputer_Start);
    INSTALL_HOOK(logger, GorillaComputer_CheckAutoBanList);
    INSTALL_HOOK(logger, GorillaComputer_BanMe);
    INSTALL_HOOK(logger, PlayFabAuthenticator_OnPlayFabError);
    INSTALL_HOOK(logger, GorillaComputer_GeneralFailureMessage);
    INSTALL_HOOK(logger, VRRig_NormalizeName);
    
    using namespace GorillaUI::Components;
    custom_types::Register::AutoRegister();

    GorillaUI::Register::RegisterViewManager<BaseGameViewManager*>("Game Settings", "1.0.8");
    GorillaUI::Register::RegisterViewManager<ModSettingsViewManager*>("Mod Settings", VERSION);
    GorillaUI::Register::RegisterView<DetailView*>("Details", VERSION);
    GorillaUI::Register::RegisterView<CustomQueueView*>("Custom Queues", VERSION);
    GorillaUI::Register::RegisterView<CommandLineView*>("Monkey Command Line", VERSION);
    
    GorillaUI::Register::RegisterSettingsView<MonkeComputerConfigView*>("Monke Computer", VERSION);
    GorillaUI::Register::RegisterSettingsView<BackgroundsView*>("Custom Backgrounds", VERSION);
    
    GorillaUI::Register::RegisterWatchViewManager<ScoreboardViewManager*>("Scoreboard", VERSION);
    GorillaUI::Register::RegisterWatchView<WatchDetailView*>("Details", VERSION);
    
    GorillaUI::Register::RegisterWatchCallback("Disconnect", VERSION, []{
        BaseGameInterface::Disconnect();
    });

    RegisterCommands();
    AddQueues();

    INFO("Mod Loaded!");
}

enum STR2INT_ERROR { SUCCESS, OVERFLOW, UNDERFLOW, INCONVERTIBLE };

STR2INT_ERROR str2int (int &i, char const *s, int base = 0)
{
    char *end;
    long  l;
    errno = 0;
    l = strtol(s, &end, base);
    if ((errno == ERANGE && l == LONG_MAX) || l > INT_MAX) {
        return OVERFLOW;
    }
    if ((errno == ERANGE && l == LONG_MIN) || l < INT_MIN) {
        return UNDERFLOW;
    }
    if (*s == '\0' || *end != '\0') {
        return INCONVERTIBLE;
    }
    i = l;
    return SUCCESS;
}

#define WRITEPATH "/sdcard/ModData/com.AnotherAxiom.GorillaTag/Mods/MonkeComputer/CommandFiles/"

void mkdir(std::string directory)
{
    if (!direxists(directory.c_str()))
    {
        int makePath = mkpath(directory.data());
        if (makePath == -1)
        {
            ERROR("Failed to make path %s", directory.c_str());
        }
    }
}
struct FakeColor {
    float r; float g; float b;
};

std::map <std::string, FakeColor> textToColor = {
    {"dark red", {0.5f, 0.0f, 0.0f}},
    {"red", {1.0f, 0.0f, 0.0f}},
    {"pink", {1.0f, 0.5f, 0.5f}},
    {"green", {0.0f, 1.0f, 0.0f}},
    {"dark green", {0.0f, 0.5f, 0.0f}},
    {"light green", {0.5f, 1.0f, 0.5f}},
    {"blue", {0.0f, 0.0f, 1.0f}},
    {"dark blue", {0.0f, 0.0f, 0.5f}},
    {"light blue", {0.5f, 0.5f, 1.0f}},
    {"cyan", {0.0f, 1.0f, 1.0f}},
    {"teal", {0.0f, 0.5f, 0.5f}},
    {"turqoise", {0.25f, 1.0f, 0.75f}},
    {"magenta", {1.0f, 0.0f, 1.0f}},
    {"purple", {0.5f, 0.0f, 1.0f}},
    {"yellow", {1.0f, 1.0f, 0.0f}},
    {"orange", {1.0f, 0.5f, 0.0f}},
    {"brown", {0.5f, 0.25f, 0.0f}},
    {"white", {1.0f, 1.0f, 1.0f}},
    {"black", {0.0f, 0.0f, 0.0f}},
    {"gray", {0.5f, 0.5f, 0.5f}},
    {"grey", {0.5f, 0.5f, 0.5f}},
    {"dark grey", {0.25f, 0.25f, 0.25f}},
    {"dark gray", {0.25f, 0.25f, 0.25f}},
    {"light grey", {0.75f, 0.75f, 0.75f}},
    {"light gray", {0.75f, 0.75f, 0.75f}}
};

void RegisterCommands()
{   
    // disconnect from current lobby
    GorillaUI::CommandRegister::RegisterCommand(std::vector<std::string>{"dc", "disconnect"}, [](std::vector<std::string>) -> std::string { 
        BaseGameInterface::Disconnect();
        return "  Disconnected!\n";
    }, "  Disconnect from your current room\n\n  DC\n  DISCONNECT");

    // disconnects and rejoins the same lobby, hopefully
    GorillaUI::CommandRegister::RegisterCommand("rejoin", [](std::vector<std::string>) -> std::string { 
        std::string roomID = BaseGameInterface::Room::get_roomID();
        BaseGameInterface::Disconnect();
        BaseGameInterface::JoinRoom(roomID);
        return "  Reconnected!";
    }, "  Disconnect and rejoin room\n  May or may not work properly\n\n  REJOIN");

    // read or set current color
    GorillaUI::CommandRegister::RegisterCommand(std::vector<std::string>{"color", "colour"}, [](std::vector<std::string> args) -> std::string {
        int r, g, b;
                
        UnityEngine::Color color = BaseGameInterface::PlayerColor::get_color();
        switch (args.size())
        {
            case 0: 
                return string_format("  Current Color: %d, %d, %d\n", (int)(color.r * 255), (int)(color.g * 255), (int)(color.b * 255));
            case 1:
                if (!str2int(r, args[0].c_str()))
                {
                    g = b = r;
                    BaseGameInterface::SetColor((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f);
                    return string_format("  Color Set to %d, %d, %d\n", r, g, b);
                }
                else 
                {
                    std::map <std::string, FakeColor>::iterator it = textToColor.find(TextUtils::toLower(args[0]));

                    if (it != textToColor.end())
                    {
                        BaseGameInterface::SetColor(it->second.r, it->second.g, it->second.b);
                        return string_format("  Color Set to %d, %d, %d\n", (int)(it->second.r * 255.0f), (int)(it->second.g * 255.0f), (int)(it->second.b * 255.0f));
                    }
                    else return "  Invalid Argument!";
                }
                break;
            case 2:
                if (TextUtils::toLower(args[0]) == "neon")
                {
                    std::map <std::string, FakeColor>::iterator it = textToColor.find(TextUtils::toLower(args[1]));
                    if (it != textToColor.end())
                    {
                        BaseGameInterface::SetColor(it->second.r * 2.0f, it->second.g * 2.0f, it->second.b * 2.0f);
                        return string_format("  Color Set to %d, %d, %d\n", (int)(it->second.r * 2.0f * 255.0f), (int)(it->second.g * 2.0f * 255.0f), (int)(it->second.b * 2.0f * 255.0f));
                    }
                    else return "  Invalid Argument!";
                }
                else // if not neon modifier
                {
                    std::string text = string_format("%s %s", args[0].c_str(), args[1].c_str());
                    std::map <std::string, FakeColor>::iterator it = textToColor.find(TextUtils::toLower(text));

                    if (it != textToColor.end())
                    {
                        BaseGameInterface::SetColor(it->second.r, it->second.g, it->second.b);
                        return string_format("  Color Set to %d, %d, %d\n", (int)(it->second.r * 255.0f), (int)(it->second.g * 255.0f), (int)(it->second.b * 255.0f));
                    }
                    else return "  Invalid Argument!";
                }
                break;
            case 3:
                if (!str2int(r, args[0].c_str()) && !str2int(g, args[1].c_str()) && !str2int(b, args[2].c_str()))
                {
                    BaseGameInterface::SetColor((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f);
                    return string_format("  Color Set to %d, %d, %d\n", r, g, b);
                }
                else if (TextUtils::toLower(args[0]) == "neon")
                {
                    std::string text = string_format("%s %s", args[1].c_str(), args[2].c_str());
                    std::map <std::string, FakeColor>::iterator it = textToColor.find(TextUtils::toLower(text));

                    if (it != textToColor.end())
                    {
                        BaseGameInterface::SetColor(it->second.r * 2.0f, it->second.g * 2.0f, it->second.b * 2.0f);
                        return string_format("  Color Set to %d, %d, %d\n", (int)(it->second.r * 2.0f * 255.0f), (int)(it->second.g * 2.0f * 255.0f), (int)(it->second.b * 2.0f * 255.0f));
                    }
                    else return "  Invalid Argument!";
                } 
                else return "  Invalid Argument(s)!";
                
                break;
            default:
                break;
        }
        return "  Incorrect amount of arguments!\n  Please Use the command as follows:\n  COLOR\n  COLOR [n]\n  COLOR [r] [g] [b]";
    }, "  Change your current color, or show your current color values.\n\n  COLOR\n  COLOR [n]\n  COLOR [r] [g] [b]");

    // read or set current name
    GorillaUI::CommandRegister::RegisterCommand("name", [](std::vector<std::string> args) -> std::string {
        if (args.size() == 0)
        {
            return string_format("  Current Player Name: %s", BaseGameInterface::Name::get_name().c_str());
        }
        else
        {
            std::string newName = args[0];
            newName = newName.substr(0, 30);
            if (newName == BaseGameInterface::Name::get_name()) return "  Name was already this!";

            bool nameFailed = !BaseGameInterface::SetName(newName);
            if (nameFailed)
            {
                Application::Quit();
            }

            return string_format("  Player Name Set to: %s", newName.c_str());
        }
    }, "  Change your name, or show your current name\n\n  NAME\n  NAME [newname]");

    // read or join a specific room code
    GorillaUI::CommandRegister::RegisterCommand("room", [](std::vector<std::string> args) -> std::string {
        if (args.size() == 0)
        {
            if (Photon::Pun::PhotonNetwork::get_InRoom())
            {
                return string_format("  Current Room: %s", BaseGameInterface::Room::get_roomID().c_str());
            }
            else return "  Not in a room";
        }
        else
        {
            std::string roomID = args[0];
            roomID = roomID.substr(0, 10);
            if (roomID == BaseGameInterface::Room::get_roomID()) return "  Already In This Room!";
            
            BaseGameInterface::JoinRoom(roomID);
            return string_format("  Attempting to join Room: %s", roomID.c_str());
        }
    }, "  Join a specific room, or list the current room code.\n\n  ROOM\n  ROOM [code]");

    // read or join a specific queue
    GorillaUI::CommandRegister::RegisterCommand("queue", [](std::vector<std::string> args) -> std::string {
        if (args.size() == 0)
        {
            std::string queue = BaseGameInterface::Queue::get_Queue();

            return string_format("  Current Queue: %s", queue.c_str());
        }
        else
        {
            std::string desiredQueue = args[0];

            if (desiredQueue.find("DEFAULT") != std::string::npos || desiredQueue.find("COMPETITIVE") != std::string::npos  || desiredQueue.find("CASUAL") != std::string::npos)
            {
                BaseGameInterface::SetQueue(desiredQueue);
                return string_format("  Queue set to %s!", desiredQueue.c_str());
            }
            else return "  Invalid Queue Name!\n  Please use the command as follows:\n  QUEUE [DEFAULT/COMPETITIVE/CASUAL]\n\n  Custom Queue's are not supported.";
        }
        return "";
    }, "  Change the queue you are playing in\n  Only base game queues are supproted\n\n  QUEUE\n  QUEUE [DEFAULT/COMPETITIVE/CASUAL]");

    // configure voice chat enabled
    GorillaUI::CommandRegister::RegisterCommand(std::vector<std::string>{"vc", "voice", "voicechat"}, [](std::vector<std::string> args) -> std::string {
        if (args.size() == 0)
        {
            return string_format("  voice chat is currently %s", BaseGameInterface::Voice::get_voiceChat() ? "On" : "Off");
        }
        else 
        {
            std::string arg = args[0];
            bool shouldSet = false, toSet;
            if (arg.find("ON") != std::string::npos || arg.find("1") != std::string::npos)
            {
                shouldSet = true;
                toSet = true;
            }
            else if (arg.find("OFF") != std::string::npos || arg.find("0") != std::string::npos)
            {
                shouldSet = true;
                toSet = false;
            }

            if (shouldSet)
            {
                BaseGameInterface::SetVoiceChat(toSet);
                return toSet ? "  Turned Voice Chat On!" : "  Turned Voice Chat Off!";
            }
            else return "  Invalid Argument!\n  Please use the command as follows:\n  VC [ON/OFF]\n  VC [0/1]\n  VOICE [ON/OFF]\n  VOICE [0/1]";
        }
    }, "  Turn the entire voice chat on/off, giving you some peace and quiet\n\n  VC\n  VOICE\n  VOICECHAT\n  VC [ON/OFF]\n  VC [0/1]\n  VOICE [ON/OFF]\n  VOICE [0/1]\n  VOICECHAT [ON/OFF]\n  VOICECHAT [0/1]");

    // configure mic setting (all/ptt/ptm)
    GorillaUI::CommandRegister::RegisterCommand(std::vector<std::string>{"mic", "microphone"}, [](std::vector<std::string> args) -> std::string {
        if (args.size() == 0)
        {
            return string_format("  Current Mic Setting is: %s", BaseGameInterface::Mic::get_pttType().c_str());
        }
        else
        {
            char desiredSetting = args[0][0];

            switch (desiredSetting)
            {
                case 'A':
                    BaseGameInterface::SetpttType(0);
                    return "  Set Push To Talk to ALL CHAT";
                case 'M':
                    BaseGameInterface::SetpttType(2);
                    return "  Set Push To Talk to PUSH TO MUTE";
                case 'T':
                    BaseGameInterface::SetpttType(1);
                    return "  Set Push To Talk to PUSH TO TALK";
                default:
                    return "  Invalid Argument!\n  Please use the command as follows:\n  MIC\n  MICROPHONE\n  MIC [A/M/T]\n  MICROPHONE [A/M/T]";
            }
        }
    }, "  Change your mic settings, so you can be muted if you want to\n\n  MIC\n  MICROPHONE\n  MIC [A/M/T]\n  MICROPHONE [A/M/T]");

    // configure turn settings
    GorillaUI::CommandRegister::RegisterCommand("turn", [](std::vector<std::string> args) -> std::string {
        int speed = BaseGameInterface::SnapTurn::get_turnValue();
        std::string type = BaseGameInterface::SnapTurn::get_turnType();
        bool failed = false;
        if (args.size() == 0)
        {
            std::string type = BaseGameInterface::SnapTurn::get_turnType();
            return string_format("  Current Turn type is %s\n  Current Turn speed is %d", type.c_str(), speed);
        }
        else
        {
            if (args.size() == 1)
            {
                if (str2int(speed, args[0].c_str())) // if the get value fails, it's text and not a number
                {
                    // so it's text
                    type = args[0];

                    if (type.find("SMOOTH") != std::string::npos)
                    {
                        type = "SMOOTH";
                    }
                    else if (type.find("SNAP") != std::string::npos)
                    {
                        type = "SNAP";
                    }
                    else if (type.find("NONE") != std::string::npos)
                    {
                        type = "NONE";
                    }
                    else
                    {
                        type = BaseGameInterface::SnapTurn::get_turnType();
                        failed = true;
                    }
                }
            } 
            else // if there are more than 1 arg
            {
                std::string arg1 = args[0];
                std::string arg2 = args[1];

                if (str2int(speed, arg1.c_str())) // if the get value fails, it's text and not a number
                {
                    // first arg was not a number
                    if (str2int(speed, arg2.c_str()))
                    {
                        // second arg was not a number
                        failed = true;
                    }
                    else // second arg indeed a number 
                    {
                        // first arg must be the text we are looking for
                        if (arg1.find("SMOOTH") != std::string::npos)
                        {
                            type = "SMOOTH";
                        }
                        else if (arg1.find("SNAP") != std::string::npos)
                        {
                            type = "SNAP";
                        }
                        else if (arg1.find("NONE") != std::string::npos)
                        {
                            type = "NONE";
                        }
                        else
                        {
                            type = BaseGameInterface::SnapTurn::get_turnType();
                            failed = true;
                        }
                    }
                }
                else // first arg was indeed a number
                {
                    // second arg must be the text we are looking for
                    if (arg2.find("SMOOTH") != std::string::npos)
                    {
                        type = "SMOOTH";
                    }
                    else if (arg2.find("SNAP") != std::string::npos)
                    {
                        type = "SNAP";
                    }
                    else if (arg2.find("NONE") != std::string::npos)
                    {
                        type = "NONE";
                    }
                    else
                    {
                        type = BaseGameInterface::SnapTurn::get_turnType();
                        failed = true;
                    }
                }
            }

            if (!failed) BaseGameInterface::ChangeTurnMode(type, speed);
            else return "  Invalid Argument(s)!\n  Please use the command as follows:\n  TURN\n  TURN [speed]\n  TURN [SNAP/SMOOTH/NONE]\n  TURN [speed] [SNAP/SMOOTH/NONE]\n  TURN [SNAP/SMOOTH/NONE] [speed]";
        }

        return string_format("  Set Turn type to %s\n  Set Turn speed to %d", type.c_str(), speed);
    }, "  Change the turn settings to better suit your needs\n  Choose between NONE, SNAP and SMOOTH, and type a turn speed value\n\n  TURN [SNAP/SMOOTH/NONE]\n  TURN [speed] [SNAP/SMOOTH/NONE]\n  TURN [SNAP/SMOOTH/NONE] [speed]");
    
    // quit the game
    GorillaUI::CommandRegister::RegisterCommand(std::vector<std::string>{"quit", "exit", "shutdown", "crash"}, [](std::vector<std::string> args) -> std::string {
        Application::Quit();
        return "";
    }, "  Quits the game\n\n  QUIT\n  EXIT\n  SHUTDOWN\n  CRASH");
    
    // echo whatever was typed to the output
    GorillaUI::CommandRegister::RegisterCommand("echo", [](std::vector<std::string> args) -> std::string {
        std::string msg = "  ";
        for (auto arg : args)
        {
            msg += arg;
            msg += " ";
        }
        return msg;
    }, "  Repeats whatever is given as arguments back to the user with reduced whitespace\n\n  ECHO <content>");

    // print whatever is typed to a file
    GorillaUI::CommandRegister::RegisterCommand("write", [](std::vector<std::string> args) -> std::string {
        if (args.size() < 2) return "  Invalid Arguments\n  Please use the command as follows:\n  write [filename] <content>";
        if (!direxists(WRITEPATH)) mkdir(WRITEPATH);
        std::string fileName = args[0];
        std::string filePath = WRITEPATH + fileName + ".txt";

        std::string txt = "";
        bool first = true;
        for (auto arg : args)
        {
            // skip the first argument
            if (first)
            {
                first = false;
                continue;
            }
            int index = arg.find('\n');
            if (index != std::string::npos)
            {
                arg.insert(index + 1, 1, ' ');
                arg.insert(index, 1, ' ');
            }
            
            txt += arg;
            txt += " ";
        }

        if (fileexists(filePath)) deletefile(filePath);
        writefile(filePath, txt);

        return string_format("  Wrote to file %s.txt", fileName.c_str());
    }, "  Writes content to a text file, allows you to put multiple commands\n back to back for easy execution with the RUN command\n  Use Option1 to add a new line to signify next command while writing\n\n WRITE [filename] <content>");

    // read a file from the files dir
    GorillaUI::CommandRegister::RegisterCommand(std::vector<std::string>{"read", "cat"}, [](std::vector<std::string> args) -> std::string {
        if (args.size() < 1) return "  Invalid Argument!\n  Please use the command as follows:\n  read [filename]\n  cat [filename]";
        std::string fileName = args[0];
        std::string filePath = WRITEPATH + fileName + ".txt";
        if (!fileexists(filePath)) return "  Requested file did not exist!\n  Write it to disk first before trying to read it!";
        return "  " + readfile(filePath);
    }, "  Reads a file to output from the command files folder\n  This folder can be found at <color=#fdfdfd>/sdcard/ModData/com.AnotherAxiom.GorillaTag/Mods/MonkeComputer/CommandFiles</color>\n  READ [filename]\n\n  CAT [filename]");

    // delete a file from the files dir
    GorillaUI::CommandRegister::RegisterCommand(std::vector<std::string>{"rm", "delete"}, [](std::vector<std::string> args) -> std::string {
        if (args.size() < 1) return "  Invalid Argument!\n  Please use the command as follows:</color=#fdfdfd>\n  RM [filename]\n  DELETE [filename]</color>";

        std::string fileName = args[0];
        std::string filePath = WRITEPATH + fileName + ".txt";

        if (fileexists(filePath)) 
        {
            deletefile(filePath);
            return string_format("  Deleted %s.txt", fileName.c_str());
        }
        else return string_format("  File %s.txt did not exist!", fileName.c_str());
    }, "  Deletes a .txt file from the Command files folder\n  This folder can be found at <color=#fdfdfd>/sdcard/ModData/com.AnotherAxiom.GorillaTag/Mods/MonkeComputer/CommandFiles</color>\n\n  RM [filename]\n  DELETE [filename]");

    // runs a file as list of commands
    GorillaUI::CommandRegister::RegisterCommand("run", [](std::vector<std::string> args) -> std::string {
        if (args.size() < 1) return "  Invalid Argument!\n  Please use the command as follows:\n  run [filename]";
        std::string fileName = args[0];
        std::string filePath = WRITEPATH + fileName + ".txt";
        if (!fileexists(filePath)) return "  Requested file did not exist!\n  Write it to disk first before trying to read it!";

        std::string content = readfile(filePath);
        std::vector<std::string> commands = {};

        int index;
        while((index = content.find('\n')) != std::string::npos)
        {
            std::string cmd = content.substr(0, index);
            content.erase(0, index + 1);
            cmd.erase(cmd.size() - 1);
            if (cmd != "") commands.push_back(cmd);
        }
        if (content != "") commands.push_back(content);

        for (auto c : commands)
        {
            INFO("Command: '%s'", c.c_str());
            INFO("Output:\n%s", GorillaUI::CommandRegister::HandleCommand(c).c_str());
        }

        return string_format("  Ran %s!", fileName.c_str());
    }, "Runs a made .txt file as a list of commands\n  These txt files exist in <color=#fdfdfd>/sdcard/ModData/com.AnotherAxiom.GorillaTag/Mods/MonkeComputer/CommandFiles</color>\n  Every line is a new command, this means you can automate certain lists of tasks\n\n  RUN [filename]");

    // read or set current screen color
    GorillaUI::CommandRegister::RegisterCommand("screencolor", [](std::vector<std::string> args) -> std::string {
        int r, g, b;
        switch (args.size())
        {
            case 0: 
                return string_format("  Current Screen Color: %d, %d, %d\n", (int)(config.screenColor.r * 255), (int)(config.screenColor.g * 255), (int)(config.screenColor.b * 255));
            case 1:
                if (!str2int(r, args[0].c_str()))
                {
                    g = b = r;
                    GorillaUI::CustomComputer::get_instance()->SetBG((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f);
                    return string_format("  Screen Color Set to %d, %d, %d\n", r, g, b);
                }
                else return "  Invalid Argument!";
            case 3:
                if (!str2int(r, args[0].c_str()) && !str2int(g, args[1].c_str()) && !str2int(b, args[2].c_str()))
                {
                    GorillaUI::CustomComputer::get_instance()->SetBG((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f);
                    return string_format("  Screen Color Set to %d, %d, %d\n", r, g, b);
                }
                else return "  Invalid Argument(s)!";
            default:
                break;
        }
        return "  Incorrect amount of arguments!\n  Please Use the command as follows:\n  SCREENCOLOR\n  SCREENCOLOR [n]\n  SCREENCOLOR [r] [g] [b]";
    }, "  Configure the screen color for the computer.\n\n  SCREENCOLOR\n  SCREENCOLOR [n]\n  SCREENCOLOR [r] [g] [b]");

    // set the command history length
    GorillaUI::CommandRegister::RegisterCommand("history", [](std::vector<std::string> args) -> std::string {
        if (args.size() == 0)
        {
            return string_format("  The current command history length is %d", config.commandHistoryLength);
        }
        if (!str2int(config.commandHistoryLength, args[0].c_str()))
        {
            return string_format("  Set the command history length to %d", config.commandHistoryLength);
        }
        return "  Invalid Argument!\n  Please use the command as follows:\n  HISTORY\n  HISTORY [n]";
    }, "  Change the length of recorded command history\n  This history can be scrolled through by using the arrows\n  And allows for repeating of previously inputted commando's\n\n  HISTORY\n  HISTORY[n]");

    // offers help with commands
    GorillaUI::CommandRegister::RegisterCommand("help", [](std::vector<std::string> args) -> std::string {
        if (args.size() == 0)
        {
            std::string result = "";
            result += "  Available commands in the command line:\n";
            result += "  dc          color    name\n";
            result += "  room        queue    vc\n";
            result += "  mic         turn     quit\n";
            result += "  echo        write    read\n";
            result += "  rm          run      screencolor\n";
            result += "  history     help     donate\n";
            result += "  rejoin\n";
            result += "\n  Use HELP [command] for more info";
            return result;
        }
        else return GorillaUI::CommandRegister::get_help(args[0]);
    }, "  Run without any arguments to get a list of possible commands\n  This won't be every command available,\n  but definitely some of the options.\n\n  Run with an argument to find help for a specific command\n  HELP\n  HELP [command]");

    // plug donation
    GorillaUI::CommandRegister::RegisterCommand(std::vector<std::string>{"donate", "paypal"}, [](std::vector<std::string> args) -> std::string {
        return "\n  Do you like what I do with these mods?\n  consider donating on paypal!\n  This money would help me justify working on mods, and overall just help me out :)\n\n  Paypal:\n  <color=#fdfdfd>https://paypal.me/RedBrumblerOfficial</color>";
    }, "  Information about donating, as making mods takes time\n\n  DONATE\n  PAYPAL");

    // pink cute
    GorillaUI::CommandRegister::RegisterCommand("pink", [](std::vector<std::string> args) -> std::string {
        return "  cute";
    }, "  the result is the truth");

    // william gay
    GorillaUI::CommandRegister::RegisterCommand("william", [](std::vector<std::string> args) -> std::string {
        return "  gay";
    }, "  the result is the truth");

    // cyan furry
    GorillaUI::CommandRegister::RegisterCommand("cyan", [](std::vector<std::string> args) -> std::string {
        return "  furry";
    }, "  the result is the truth");
}

void AddQueues()
{
    CustomQueues::add_queue("MODDED", "Modded", "  <size=40>A Queue for modded players, outside of the normal public lobbies.\n    You can't really report anyone for having a mod here, but griefing\n    is still prohibited.\n</size>");
    CustomQueues::add_queue("DEFAULT", "Default", "  <size=40>Most players will choose this queue,\n    expect new players all the way up to pros\n</size>");
    CustomQueues::add_queue("COMPETITIVE", "Competitive", "  <size=40>For players looking for a challenge,\n    expect pros and people looking to improve their monke skills\n</size>");
    CustomQueues::add_queue("CASUAL", "Casual", "  <size=40>For people just looking to hang out,\n    nobody will be it or be infected\n</size>");
}