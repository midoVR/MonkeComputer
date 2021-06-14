#include "GorillaUI/CommandLineView.hpp"
#include "CustomQueues.hpp"
#include "GorillaUI/BaseGameInterface.hpp"
#include "Helpers/SelectionHelper.hpp"
#include "ViewLib/CustomComputer.hpp"
#include "CommandRegister.hpp"

#include "config.hpp"

DEFINE_TYPE(GorillaUI::CommandLineView);

extern Logger& getLogger();

namespace GorillaUI
{
    void CommandLineView::Awake()
    {
        textInputHandler = new UITextInputHandler(EKeyboardKey::Enter, true);
        historyHandler = new UISelectionHandler(EKeyboardKey::Down, EKeyboardKey::Up, EKeyboardKey::Enter, false, false);
        historyHandler->max = 1;
        historyHandler->min = 0;
        historyHandler->currentSelectionIndex = 0;

        commandHistory = std::vector<std::string>{};
        lastOutput = "";
    }

    void CommandLineView::DidActivate(bool firstActivation)
    {
        std::function<void(std::string)> fun = std::bind(&CommandLineView::OnCommandInput, this, std::placeholders::_1);
        textInputHandler->textCallback = fun;
        lastOutput = "";
        Redraw();
    }

    void CommandLineView::OnCommandInput(std::string value)
    {
        lastOutput = CommandRegister::HandleCommand(value);
        textInputHandler->text = "";

        AddCommandToHistory(value);
        historyHandler->max = commandHistory.size() + 1;
        historyHandler->currentSelectionIndex = 0;
    }

    void CommandLineView::Redraw()
    {
        text = "";

        DrawHeader();
        DrawCommand();

        CustomComputer::Redraw();
    }
    
    void CommandLineView::DrawHeader()
    {
        text += "<color=#ffff00>== <color=#fdfdfd>Command Line</color> ==</color>\n";
    }
    
    void CommandLineView::DrawCommand()
    {
        text += "\n";
        text += " > ";
        text += textInputHandler->text;

        if (!lastOutput.empty())
        {
            text += "\n\nOutput:\n";
            text += "<size=40>\n";
            text += lastOutput;
            text += "\n</size>";
        }
    }
    
    void CommandLineView::OnKeyPressed(int key)
    {
        textInputHandler->HandleKey((EKeyboardKey)key);
        if (historyHandler->HandleKey((EKeyboardKey)key) && key != (int)EKeyboardKey::Enter) // it was an arrow key
        {
            std::string historyCommand = GetCommand(historyHandler->currentSelectionIndex);
            if ((historyCommand != "" || historyHandler->currentSelectionIndex == 0) && historyHandler->max != 1)
            {
                textInputHandler->text = historyCommand;
            }
        }

        // some special characters that can be used with the macto creation
        switch ((EKeyboardKey)key)
        {
            case EKeyboardKey::Option1:
                textInputHandler->text += '\n';
                break;
            case EKeyboardKey::Option2:
                textInputHandler->text += ',';
                break;
            case EKeyboardKey::Option3:
                textInputHandler->text += '.';
                break;
            default:
                break;
        }

        getLogger().info("history index: %d", historyHandler->currentSelectionIndex);
        Redraw();
    }

    std::string CommandLineView::GetCommand(int index)
    {
        // if index 0, it's what was being typed, so return empty
        if (index == 0) return "";
        // get size
        int size = commandHistory.size();
        // remove the 1 that maeks it so we can know if it's current typing
        index -= 1;
        // if size is somehow smaller than asked, return empty, or if index is lower than 0
        if (size <= index || index < 0) return "";

        // return the command from the top of the list
        return commandHistory[size - index - 1];
    }

    void CommandLineView::AddCommandToHistory(std::string command)
    {
        // if empty, just dont add to history because that is a waste of space and history
        if (command == "") return;
        
        getLogger().info("Added Command %s to the list", command.c_str());
        // add to list
        commandHistory.push_back(command);

        // remove first from list if the size is getting too big
        while (commandHistory.size() > config.commandHistoryLength)
        {
            getLogger().info("Command history list was larger than max size, making it shorter...");
            commandHistory.erase(commandHistory.begin(), commandHistory.begin() + 1);
        }
    }
}