#include "GorillaUI/WatchDetailView.hpp"
#include "GorillaUI.hpp"
#include "ViewLib/MonkeWatch.hpp"
#include "GorillaUI/BaseGameInterface.hpp"
#include "BanManager.hpp"
#include "UnityEngine/Color.hpp"
#include "ViewLib/CustomScreenInfo.hpp"


DEFINE_TYPE(GorillaUI, WatchDetailView);

extern Logger& getLogger();

namespace GorillaUI
{
    void WatchDetailView::DidActivate(bool firstActivation)
    {
        Redraw();
    }

    void WatchDetailView::Redraw()
    {
        text = "";

        DrawHeader();
        DrawDetails();

        watch->Redraw();
    }
    
    void WatchDetailView::DrawHeader()
    {
        text += "<color=#ffff00>== <color=#fdfdfd>Details</color> ==</color>\n";
    }
    
    void WatchDetailView::DrawDetails()
    {
        // if user is banned
        if (BanManager::get_isBanned())
        {
            text += BanManager::get_displayMessage();
        }
        // if user is not banned
        else
        {
            text += "<size=60>";
            text += "<size=40>  Refresh by reopening this menu\n</size>\n";
            time_t now = time(0);
            char *date = ctime(& now);

            text += string_format(" Current Time:\n %s\n", date);
            
            text += " Current Game Version:\n <color=#fdfdfd>" + BaseGameInterface::get_gameVersion() + "</color>\n";
            text += " Current Name:\n <color=#fdfdfd>" + BaseGameInterface::Name::get_name() + "</color>\n\n";

            char playerCount = BaseGameInterface::Room::get_playerCount();
            if (playerCount > 0)
            {
                std::string room = BaseGameInterface::Room::get_roomID();
                text += " Current Room:\n <color=#fdfdfd>" + room + "</color>\n";
                text += string_format(" Players In Room:\n <color=#fdfdfd>%d</color>\n", playerCount);
            }
            else
            {
                text += " Not in Room\n";
            }

            text += string_format(" Players Online:\n <color=#fdfdfd>%d</color>\n", BaseGameInterface::Room::get_currentPlayers());
            int banned = BaseGameInterface::Room::get_bannedPlayers();
            if (banned > 0)
                text += string_format(" Users banned yesterday:\n <color=#fdfdfd>%d</color>\n", banned);
            text += "</size>";
        }
    }
}