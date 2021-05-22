#pragma once

#include <string>
#include <vector>

namespace GorillaUI::ReportUser
{
    bool ReportUser(std::string userID, int reason);
    bool get_isReported(std::string userID);
    
    namespace
    {
        std::vector<std::string> reportedUserIDs = {};
    }
}