#pragma once
#include <cstdlib>
#include <UEAA/Utils/SharedPointer.hpp>
#include <UEAA/Utils/ReferenceCounted.hpp>
#include <UEADB/Core/TypeDefs.hpp>

namespace UEADB
{
CommandsList ReadCommands (const std::vector <std::string> &cmdArguments);
void PrintCommands (const CommandsList &commandsList);
void PrintCommand (const CommandInfo &command);

unsigned ExecuteCommands (const CommandsList &commandsList, const std::map <unsigned, CommandExecutor> &commandExecutors);
unsigned ExecuteCommand (const CommandInfo &command, SharedPointersMap &sharedPointersMap,
                         const std::map <unsigned, CommandExecutor> &commandExecutors);
}
