#pragma once
#include <UEADB/Core/TypeDefs.hpp>
#include <UEAA/Utils/DeHashTable.hpp>

namespace UEADB
{
namespace Application
{
std::map <unsigned, CommandExecutor> SetupCommandExecutors ();
void PrintCommandsList ();
void PrintCommandHelp (const char *commandName);
UEAA::DeHashTable *GetOrCreateDeHashTable (SharedPointersMap &sharedContext);
}
}
