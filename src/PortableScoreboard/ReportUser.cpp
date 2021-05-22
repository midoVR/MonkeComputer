#include "PortableScoreboard/ReportUser.hpp"
#include "PortableScoreboard/PlayerView.hpp"

#include "GlobalNamespace/GorillaScoreBoard.hpp"
#include "GlobalNamespace/GorillaPlayerScoreboardLine.hpp"
#include "GlobalNamespace/GorillaPlayerLineButton.hpp"

#include "UnityEngine/Object.hpp"
#include "UnityEngine/Resources.hpp"

#include "Photon/Realtime/Player.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

using namespace GlobalNamespace;
using namespace UnityEngine;

extern Logger& getLogger();
namespace GorillaUI::ReportUser
{
    bool ReportUser(std::string userID, int reason)
    {
        if (get_isReported(userID)) return false;

        std::vector<GorillaPlayerScoreboardLine*> lines = PlayerView::FindScoreboardLineForUserID(userID);
        if (lines.size() > 0)
        {
            GlobalNamespace::GorillaPlayerLineButton::ButtonType buttonType = reason;
            lines[0]->ReportPlayer(il2cpp_utils::createcsstr(userID), buttonType);

            for (auto l : lines)
            {
                l->reportButton->isOn = true;
                l->reportButton->UpdateColor();
            }
            getLogger().info("Found lines to set for reporting user %s with reason %d", userID.c_str(), reason);
            return true;
        }

        return false;
    }

    bool get_isReported(std::string userID)
    {
        std::vector<std::string>::iterator it = std::find(reportedUserIDs.begin(), reportedUserIDs.end(), userID);
        if (it != reportedUserIDs.end()) 
        {
            return true;
        }

        std::vector<GorillaPlayerScoreboardLine*> lines = PlayerView::FindScoreboardLineForUserID(userID);

        if (lines.size() > 0)
        {
            bool reported = false;

            for (auto l : lines)
            {
                if (l->reportButton->isOn) reported = true;
            }

            // if this player is already reported, we need to add it to the watch
            if (reported)
            {
                reportedUserIDs.push_back(userID);
            }
            return reported;
        }
        return false;
    }
}