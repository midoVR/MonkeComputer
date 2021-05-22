#include "PortableScoreboard/ReportView.hpp"
#include "PortableScoreboard/PlayerView.hpp"
#include "PortableScoreboard/ReportUser.hpp"
#include "GorillaUI.hpp"
#include "Register.hpp"
#include "ViewLib/MonkeWatch.hpp"
#include "Helpers/SelectionHelper.hpp"
#include "Helpers/PageHelper.hpp"
#include "GorillaUI/BaseGameInterface.hpp"

#include "Photon/Pun/PhotonNetwork.hpp"
#include "Photon/Realtime/Player.hpp"

#include "GlobalNamespace/GorillaParent.hpp"
#include "GlobalNamespace/VRRig.hpp"
#include "GlobalNamespace/GorillaPlayerLineButton.hpp"

#include "UnityEngine/Material.hpp"

DEFINE_TYPE(GorillaUI::ReportView);

extern Logger& getLogger();

#define MENU_PAGE_SIZE 5

using namespace UnityEngine;
using namespace Photon::Pun;
using namespace Photon::Realtime;

namespace GorillaUI
{
    void ReportView::Awake()
    {
        if (!selectionHandler) selectionHandler = new UISelectionHandler(EKeyboardKey::Up, EKeyboardKey::Down, EKeyboardKey::Enter, true, true);
        selectionHandler->max = 3;
    }

    void ReportView::DidActivate(bool firstActivation)
    {
        std::function<void(int)> fun = std::bind(&ReportView::Report, this, std::placeholders::_1);
        selectionHandler->selectionCallback = fun;
        
        Redraw();
    }

    void ReportView::Report(int index)
    {
        if (!PhotonNetwork::get_InRoom()) return;
        ReportUser::ReportUser(playerInfo.playerID, index);
    }

    void ReportView::Redraw()
    {
        text = "";

        DrawHeader();
        DrawReportOptions();

        watch->Redraw();
    }
    
    void ReportView::DrawHeader()
    {
        text += "<color=#ffff00>== Report User ==</color>\n";
    }
    
    void ReportView::DrawReportOptions()
    {
        text += string_format("\n Report %s ?\n\n", playerInfo.nickName.c_str());
        
        if (ReportUser::get_isReported(playerInfo.playerID))
        {
            text += "\n This user has already been reported\n";
        }
        else
        {
            text += " <size=40>\n  Select a reason and press enter to confirm\n  Or press back to stop the report\n</size>";
            std::vector<std::string> options = {
                "Hate Speech",
                "Cheating",
                "Toxicity"
            };

            SelectionHelper::DrawSelection(options, selectionHandler->currentSelectionIndex, text);
        }
    }    
    void ReportView::OnKeyPressed(int key)
    {
        selectionHandler->HandleKey((EKeyboardKey)key);
        Redraw();
    }
}