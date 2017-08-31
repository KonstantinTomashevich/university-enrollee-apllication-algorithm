#include "Application.hpp"
#include <UEAA/Utils/CStringToHash.hpp>
#include <iostream>

namespace UEADB
{
namespace Application
{
unsigned Execute (const wchar_t *arguments)
{
    return Execute (ParseArgumentsString (arguments));
}

unsigned Execute (int argumentsCount, char **arguments)
{
    std::vector <std::string> argumentsVector;
    // Because first arguments is always executable path.
    if (argumentsCount > 1)
    {
        for (unsigned index = 1; index < argumentsCount; index++)
        {
            argumentsVector.emplace_back (std::string (arguments[index]));
        }
    }
    return Execute (argumentsVector);
}

unsigned Execute (const std::vector <std::string> &arguments)
{
    CommandsList commandsList = ReadCommands (arguments);
    PrintCommands (commandsList);
    std::map <unsigned, CommandExecutor> commandExecutors = SetupCommandExecutors ();
    return ExecuteCommands (commandsList, commandExecutors);
}

std::vector <std::string> ParseArgumentsString (const wchar_t *arguments)
{
    size_t length = wcslen (arguments);
    char *cArguments = new char[length];
    wcstombs (cArguments, arguments, length);

    std::vector <std::string> argumentsVector;
    std::string read = "";
    bool isQuoteOpened = false;
    bool executablePathSkipped = false;

    for (unsigned index = 0; index < length; index++)
    {
        char symbol = cArguments[index];
        if (symbol == '\"')
        {
            isQuoteOpened = !isQuoteOpened;
        }

        else if (isspace (symbol))
        {
            if (isQuoteOpened)
            {
                read += " ";
            }
            else if (executablePathSkipped)
            {
                if (!read.empty ())
                {
                    argumentsVector.push_back (read);
                }
                read = "";
            }
            else
            {
                executablePathSkipped = true;
                read = "";
            }
        }

        else
        {
            read += symbol;
        }
    }

    delete [] cArguments;
    if (executablePathSkipped)
    {
        argumentsVector.push_back (read);
    }
    return argumentsVector;
}

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

std::map <unsigned, CommandExecutor> SetupCommandExecutors ()
{
    std::map <unsigned, CommandExecutor> commandExecutors;
    return commandExecutors;
}

unsigned ExecuteCommands (const CommandsList &commandsList, const std::map <unsigned, CommandExecutor> &commandExecutors)
{
    std::cout << "Executing commands..." << std::endl;
    for (auto iterator = commandsList.cbegin (); iterator != commandsList.cend (); ++iterator)
    {
        const CommandInfo &command = *iterator;
        PrintCommand (command);
        unsigned result = ExecuteCommand (command, commandExecutors);
        if (result != 0)
        {
            return result;
        }
    }
    return 0;
}

unsigned ExecuteCommand (const CommandInfo &command, const std::map <unsigned, CommandExecutor> &commandExecutors)
{
    unsigned commandNameHash = UEAA::CStringToHash (command.first.c_str ());
    try
    {
        CommandExecutor executor = commandExecutors.at (commandNameHash);
        return (*executor) (command.second);
    }
    catch (std::out_of_range &exception)
    {
        std::cout << "    Executor for command " << commandNameHash << " not exists. Execution cancelled." << std::endl;
        return Errors::COMMAND_EXECUTOR_NOT_EXISTS;
    }
}
}
}
