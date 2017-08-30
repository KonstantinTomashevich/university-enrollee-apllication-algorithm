#include "Application.hpp"
#include <iostream>

namespace UEADB
{
Application::Application ()
{

}

Application::~Application ()
{

}

unsigned Application::Execute (const wchar_t *arguments)
{
    return Execute (ParseArgumentsString (arguments));
}

unsigned Application::Execute (int argumentsCount, char **arguments)
{
    std::vector <std::string> argumentsVector;
    // Because first arguments is always executable path.
    if (argumentsCount > 1)
    {
        for (unsigned index = 1; index < argumentsCount; index++)
        {
            argumentsVector.emplace_back (std::string (arguments [index]));
        }
    }
    return Execute (argumentsVector);
}

unsigned Application::Execute (const std::vector <std::string> &arguments)
{
    CommandsList commandsList = ReadCommands (arguments);
    PrintCommands (commandsList);
    return 0;
}

std::vector <std::string> Application::ParseArgumentsString (const wchar_t *arguments)
{
    size_t length = wcslen (arguments);
    char *cArguments = new char [length];
    wcstombs (cArguments, arguments, length);

    std::vector <std::string> argumentsVector;
    std::string read = "";
    bool isQuoteOpened = false;
    bool executablePathSkipped = false;

    for (unsigned index = 0; index < length; index++)
    {
        char symbol = cArguments [index];
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

CommandsList Application::ReadCommands (const std::vector <std::string> &cmdArguments)
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

void Application::PrintCommands (const CommandsList &commandsList)
{
    std::cout << "Parsed commands:" << std::endl;
    for (auto iterator = commandsList.cbegin (); iterator != commandsList.cend (); iterator++)
    {
        CommandInfo command = *iterator;
        std::cout << "    " << command.first << " (";
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
    std::cout << std::endl;
}
}
