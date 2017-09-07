#include <vector>
#include <string>
int CrossPlatformMain (const std::vector <std::string> &arguments);

#ifdef _WIN32
#include <windows.h>
#ifdef _MSC_VER
#include <crtdbg.h>
#endif
#endif

namespace CrossPlatformMainFunctions
{
int UnixStyleExecute (int argumentsCount, char **arguments)
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
    return CrossPlatformMain (argumentsVector);
}

int WindowsStyleExecute (const wchar_t *arguments)
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

    delete[] cArguments;
    if (executablePathSkipped)
    {
        argumentsVector.push_back (read);
    }

    return CrossPlatformMain (argumentsVector);
}
}

#if defined(_MSC_VER) && defined(_DEBUG)
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    return CrossPlatformMainFunctions::WindowsStyleExecute (GetCommandLineW ());
}

#elif defined(_WIN32)
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    return CrossPlatformMainFunctions::WindowsStyleExecute (GetCommandLineW ());
}

#else
int main(int argc, char** argv)
{
    return CrossPlatformMainFunctions::UnixStyleExecute (argc, argv);
}
#endif
