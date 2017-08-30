#pragma once
#include <UEAA/Utils/ReferenceCounted.hpp>
#include <cstdlib>
#include <string>
#include <vector>

namespace UEADB
{
// List of <command, vector of arguments> pairs.
typedef std::pair <std::string, std::vector <std::string> > CommandInfo;
typedef std::vector <CommandInfo> CommandsList;

class Application : public UEAA::ReferenceCounted
{
public:
    Application ();
    virtual ~Application ();

    unsigned Execute (const wchar_t* arguments);
    unsigned Execute (int argumentsCount, char **arguments);

private:
    unsigned Execute (const std::vector <std::string> &arguments);
    std::vector <std::string> ParseArgumentsString (const wchar_t *arguments);

    CommandsList ReadCommands (const std::vector <std::string> &cmdArguments);
    void PrintCommands (const CommandsList &commandsList);
};
}
