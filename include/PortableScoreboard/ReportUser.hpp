#pragma once

#include <string>
#include <vector>

namespace GorillaUI::ReportUser
{
    bool ReportUser(std::string userID, int reason, std::string otherPlayerNickname);
    bool get_isReported(std::string userID);
    
    namespace
    {
        std::vector<std::string> reportedUserIDs = {};
    }
}