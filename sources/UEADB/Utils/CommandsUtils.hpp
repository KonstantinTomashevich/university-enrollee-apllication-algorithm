#pragma once
#include <UEADB/Core/TypeDefs.hpp>
#include <UEAA/Utils/DeHashTable.hpp>
#include <UEAA/Core/University/University.hpp>

namespace UEADB
{
std::map <unsigned, CommandExecutor> SetupCommandExecutors ();
void PrintCommandsList ();
void PrintCommandHelp (const char *commandName);
UEAA::DeHashTable *GetOrCreateDeHashTable (SharedPointersMap &sharedContext);
UEAA::University *GetOrCreateUniversity (SharedPointersMap &sharedContext);
}
