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
            argumentsVector.push_back (std::string (arguments [index]));
        }
    }
    return Execute (argumentsVector);
}

unsigned Application::Execute (const std::vector <std::string> &arguments)
{
    for (auto iterator = arguments.cbegin (); iterator != arguments.cend (); iterator++)
    {
        std::cout << *iterator << std::endl;
    }
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
}
