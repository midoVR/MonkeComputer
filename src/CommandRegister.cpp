#include "CommandRegister.hpp"
#include "beatsaber-hook/shared/utils/utils.h"

namespace GorillaUI
{
    std::string toUpperCase(std::string in)
    {
        std::string out = "";

        for (auto c : in)
        {
            out += toupper(c);
        }

        return out;
    }

    bool CommandRegister::RegisterCommand(std::vector<std::string> commands, CommandRegister::CommandCallback callback, std::string help)
    {
        bool noFail = true;

        for (auto c : commands)
        {
            if (!RegisterCommand(c, callback, help)) noFail = false;
        }

        return noFail;
    }

    bool CommandRegister::RegisterCommand(std::string command, CommandRegister::CommandCallback callback, std::string help)
    {
        command = toUpperCase(command);

        CommandList::iterator it = commandList.find(command);
        if (it != commandList.end()) // if command already exists
        {
            return false;
        }

        // hurray it was a new command, let's register it
        commandList[command] = callback;
        if (help == "") help = "  No help offered for this command :(";
        else helpList[command] = help;

        return true;
    }

    std::string CommandRegister::HandleCommand(std::string userInput)
    {
        while(userInput[0] == ' ') userInput.erase(0, 1);
        if (!userInput.ends_with(' ')) userInput += ' ';

        int firstPos = userInput.find_first_of(' ');
        std::string command = userInput.substr(0, firstPos);
        command = toUpperCase(command);
        userInput.erase(0, firstPos);

        CommandList::iterator it = commandList.find(command);
        if (it == commandList.end()) // if command not found
        {
            return string_format("  Command '%s', not found!", command.c_str());
        }

        std::vector<std::string> arguments = {};
        while (userInput.find(' ') != std::string::npos)
        {
            firstPos = userInput.find_first_of(' ');
            std::string arg = userInput.substr(0, firstPos);
            userInput.erase(0, firstPos + 1);
            while(userInput[0] == ' ') userInput.erase(0, 1);
            if (!arg.empty()) arguments.push_back(arg);
        }

        return it->second(arguments);
    }

    std::string CommandRegister::get_help(std::string command)
    {
        command = toUpperCase(command);
        // if not end, exists thus true
        HelpList::iterator it = helpList.find(command);
        if (it == helpList.end()) // if command not found
        {
            return string_format("  Command '%s', not found!", command.c_str());
        }

        return it->second;
    }
}