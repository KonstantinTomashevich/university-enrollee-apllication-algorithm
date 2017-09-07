#include "Application.hpp"
#include <Dependencies/CrossPlatformMain/CrossPlatformMain.hpp>
#include <UEAA/Utils/CStringToHash.hpp>
#include <UEADB/Core/ErrorCodes.hpp>
#include <UEADB/Utils/CommandsUtils.hpp>

#include <cstring>
#include <iostream>

int CrossPlatformMain (const std::vector <std::string> &arguments)
{
    if (arguments.size () == 0)
    {
        UEADB::PrintCommandsList ();
        return 0;
    }
    else if (arguments.at (0) == "Help")
    {
        if (arguments.size () == 1)
        {
            UEADB::PrintCommandsList ();
            return 0;
        }
        else
        {
            UEADB::PrintCommandHelp (arguments.at (1).c_str ());
            return 0;
        }
    }
    else
    {
        UEADB::CommandsList commandsList = UEADB::ReadCommands (arguments);
        UEADB::PrintCommands (commandsList);
        std::map <unsigned, UEADB::CommandExecutor> commandExecutors = UEADB::SetupCommandExecutors ();
        return UEADB::ExecuteCommands (commandsList, commandExecutors);
    }
}

namespace UEADB
{
CommandsList ReadCommands (const std::vector <std::string> &cmdArguments)
{
    CommandsList commandsList;
    std::string currentCommand;
    std::vector <std::string> currentArguments;

    for (auto iterator = cmdArguments.begin (); iterator != cmdArguments.end (); ++iterator)
    {
        if (*iterator == ",")
        {
            if (!currentCommand.empty ())
            {
                commandsList.emplace_back (CommandInfo (currentCommand, currentArguments));
                currentCommand = "";
                currentArguments.clear ();
            }
        }
        else if (currentCommand.empty ())
        {
            currentCommand = *iterator;
        }
        else
        {
            currentArguments.emplace_back (*iterator);
        }
    }

    if (!currentCommand.empty ())
    {
        commandsList.emplace_back (CommandInfo (currentCommand, currentArguments));
    }
    return commandsList;
}

void PrintCommands (const CommandsList &commandsList)
{
    std::cout << "Parsed commands: " << std::endl;
    for (auto iterator = commandsList.cbegin (); iterator != commandsList.cend (); ++iterator)
    {
        std::cout << "    ";
        PrintCommand (*iterator);
    }
    std::cout << std::endl;
}

void PrintCommand (const CommandInfo &command)
{
    std::cout << command.first << " (";
    bool isFirst = true;

    for (auto argIterator = command.second.begin (); argIterator != command.second.end (); argIterator++)
    {
        if (!isFirst)
        {
            std::cout << ", ";
        }
        else
        {
            isFirst = false;
        }

        std::cout << "\"" << *argIterator << "\"";
    }
    std::cout << ");" << std::endl;
}

unsigned ExecuteCommands (const CommandsList &commandsList, const std::map <unsigned, CommandExecutor> &commandExecutors)
{
    std::cout << "Executing commands..." << std::endl;
    SharedPointersMap sharedContext;

    for (auto iterator = commandsList.cbegin (); iterator != commandsList.cend (); ++iterator)
    {
        const CommandInfo &command = *iterator;
        PrintCommand (command);
        unsigned result = ExecuteCommand (command, sharedContext, commandExecutors);
        if (result != 0)
        {
            return result;
        }
        else
        {
            std::cout << "    Finished!" << std::endl << std::endl;
        }
    }
    return 0;
}

unsigned ExecuteCommand (const CommandInfo &command, SharedPointersMap &sharedContext,
                         const std::map <unsigned, CommandExecutor> &commandExecutors)
{
    unsigned commandNameHash = UEAA::CStringToHash (command.first.c_str ());
    try
    {
        CommandExecutor executor = commandExecutors.at (commandNameHash);
        return (*executor) (command.second, sharedContext);
    }
    catch (std::out_of_range &exception)
    {
        std::cout << "    Executor for command " << commandNameHash << " not exists. Execution cancelled." << std::endl;
        return ErrorCodes::COMMAND_EXECUTOR_NOT_EXISTS;
    }
}
}
