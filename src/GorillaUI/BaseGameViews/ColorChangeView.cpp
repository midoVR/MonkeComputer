#include "GorillaUI/BaseGameViews/ColorChangeView.hpp"
#include "GorillaUI.hpp"
#include "Register.hpp"
#include "ViewLib/CustomComputer.hpp"
#include "GorillaUI/BaseGameInterface.hpp"
#include "Helpers/SelectionHelper.hpp"


DEFINE_TYPE(GorillaUI, ColorChangeView);

extern Logger& getLogger();

using namespace UnityEngine;

namespace GorillaUI
{
    void ColorChangeView::Awake()
    {
        if (!selectionHandler) selectionHandler = new UISelectionHandler(EKeyboardKey::Up, EKeyboardKey::Down, EKeyboardKey::Enter, true);
        selectionHandler->max = 3;

        Color col = BaseGameInterface::PlayerColor::get_color();
        float* start = &col.r;
        for (int i = 0; i < 3; i++)
        {
            numberInputHandlers[i] = new UINumberInputHandler(EKeyboardKey::Enter, false);
            numberInputHandlers[i]->number = (int)(start[i] * 255);
            numberInputHandlers[i]->max = 255;
        }
    }

    void ColorChangeView::DidActivate(bool firstActivation)
    {
        std::function<void(int)> fun = std::bind(&ColorChangeView::UpdateColor, this, std::placeholders::_1);
        selectionHandler->selectionCallback = fun;
        
        if (firstActivation)
        {
            Redraw();
        }
    }

    void ColorChangeView::UpdateColor(int num)
    {
        float r = (float)numberInputHandlers[0]->number / 255.0f;
        float g = (float)numberInputHandlers[1]->number / 255.0f;
        float b = (float)numberInputHandlers[2]->number / 255.0f;

        Color col = BaseGameInterface::PlayerColor::get_color();
        if (col.r == r && col.g == g && col.b == b)
        {
            return;
        }
        BaseGameInterface::SetColor(r, g, b);
    }

    void ColorChangeView::Redraw()
    {
        text = "";

        DrawHeader();
        DrawColors();

        CustomComputer::Redraw();
    }
    
    void ColorChangeView::DrawHeader()
    {
        text += "<color=#ffff00>== <color=#fdfdfd>Settings/Color</color> ==</color>\n";
    }
    
    void ColorChangeView::DrawColors()
    {
        
        text += "\n Gorilla Color:\n  <size=40>select values between 0 - 255</size>\n";
        std::vector<std::string> colors = {
            string_format("Red:   <color=#fdfdfd>%d</color>", numberInputHandlers[0]->number),
            string_format("Green: <color=#fdfdfd>%d</color>", numberInputHandlers[1]->number),
            string_format("Blue:  <color=#fdfdfd>%d</color>", numberInputHandlers[2]->number)
        };

        SelectionHelper::DrawSelection(colors, selectionHandler->currentSelectionIndex, text);
    }
    
    void ColorChangeView::OnKeyPressed(int key)
    {
        selectionHandler->HandleKey((EKeyboardKey)key);
        UINumberInputHandler* currentNumberHandler = numberInputHandlers[selectionHandler->currentSelectionIndex];
        currentNumberHandler->HandleKey((EKeyboardKey)key);
        Redraw();
    }
}