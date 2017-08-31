#pragma once
#include <UEAA/Utils/SharedPointer.hpp>
#include <UEAA/Utils/ReferenceCounted.hpp>

#include <cstdlib>
#include <string>
#include <vector>
#include <map>

namespace UEADB
{
typedef std::pair <std::string, std::vector <std::string> > CommandInfo;
typedef std::vector <CommandInfo> CommandsList;
typedef std::map <unsigned, UEAA::SharedPointer <UEAA::ReferenceCounted> > SharedPointersMap;
typedef unsigned (*CommandExecutor) (const std::vector <std::string> &, const SharedPointersMap &);

namespace Application
{
namespace Errors
{
const unsigned COMMAND_EXECUTOR_NOT_EXISTS = 1;
}

unsigned Execute (const wchar_t *arguments);
unsigned Execute (int argumentsCount, char **arguments);
unsigned Execute (const std::vector <std::string> &arguments);
std::vector <std::string> ParseArgumentsString (const wchar_t *arguments);

CommandsList ReadCommands (const std::vector <std::string> &cmdArguments);
void PrintCommands (const CommandsList &commandsList);
void PrintCommand (const CommandInfo &command);

std::map <unsigned, CommandExecutor> SetupCommandExecutors ();
unsigned ExecuteCommands (const CommandsList &commandsList, const std::map <unsigned, CommandExecutor> &commandExecutors);
unsigned ExecuteCommand (const CommandInfo &command, const SharedPointersMap &sharedPointersMap,
                         const std::map <unsigned, CommandExecutor> &commandExecutors);
}
}
