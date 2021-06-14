#pragma once
#include <functional>
#include <string>
#include <vector>
#include <map>

namespace GorillaUI
{
    class CommandRegister
    {
        public:
            // the parameter of the method is the list of the arguments the user gave, split up as strings
            // the return of the callback is a string, this can be the command line output given when a command is entered 
            typedef std::function<std::string(std::vector<std::string>)> CommandCallback;

            /// @brief registers a command for the Monkey Command Line
            /// @param command the first argument a user should type in to get your given callback
            /// @param callback the command that gets called when your command is typed in, with parameters
            /// @param help the message that will appear when the command is passed into the help command
            /// @return bool true for success, false for command already registered
            static bool RegisterCommand(std::string command, CommandCallback callback, std::string help = "");

            /// @brief registers multiple commands with the same callback for the Monkey Command Line
            /// @param commands the list of commands that are registered
            /// @param callback the command that gets called when your command is typed in, with parameters
            /// @return bool true for success, false for one of them already registered
            static bool RegisterCommand(std::vector<std::string> commands, CommandCallback callback, std::string help = "");

            /// @brief handles input, shouldn't really be used by anyone but the author of monkecomputer
            /// @param userInput the input the user gave
            /// @return std::string command output
            static std::string HandleCommand(std::string userInput);

            static std::string get_help(std::string command);
        private:
            typedef std::map<std::string, CommandCallback> CommandList;
            typedef std::map<std::string, std::string> HelpList;

            static inline CommandList commandList = {};
            static inline HelpList helpList = {};
    };
}