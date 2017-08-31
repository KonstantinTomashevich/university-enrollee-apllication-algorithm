#pragma once
#include <UEAA/Utils/SharedPointer.hpp>
#include <UEAA/Utils/ReferenceCounted.hpp>
#include <UEADB/Core/TypeDefs.hpp>
#include <cstdlib>

namespace UEADB
{
namespace Application
{
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
