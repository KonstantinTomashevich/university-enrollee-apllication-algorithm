#include "CommandsUtils.hpp"
#include <UEAA/Utils/CStringToHash.hpp>
#include <UEADB/Core/SharedContextGlobalKeys.hpp>

${cuIncludes}

#include <cstring>
#include <iostream>

namespace UEADB
{
namespace Application
{
std::map <unsigned, CommandExecutor> SetupCommandExecutors ()
{
    std::map <unsigned int, CommandExecutor> commandExecutors;
    ${sceCode}
    return commandExecutors;
}

void PrintCommandsList ()
{
    std::cout << "Supported commands list:" << std::endl;
    ${pclCode}
    std::cout << "For more info type \"UEADB Help CommandName\"." << std::endl;
}

void PrintCommandHelp (const char *commandName)
{
    ${pchCode}
}

UEAA::DeHashTable *GetOrCreateDeHashTable (SharedPointersMap &sharedContext)
{
    try
    {
        UEAA::ReferenceCounted *object = sharedContext.at (SharedContextGlobalKeys::DE_HASH_TABLE).GetTrackingObject ();
        return static_cast <UEAA::DeHashTable *> (object);
    }
    catch (std::out_of_range &exception)
    {
        UEAA::DeHashTable *deHashTable = new UEAA::DeHashTable ();
        UEAA::SharedPointer <UEAA::ReferenceCounted> sharedPointer (deHashTable);
        sharedContext.emplace (SharedContextGlobalKeys::DE_HASH_TABLE, sharedPointer);
        return deHashTable;
    }
}
}
}
