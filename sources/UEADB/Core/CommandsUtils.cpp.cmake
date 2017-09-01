#include "CommandsUtils.hpp"
#include <UEAA/Utils/CStringToHash.hpp>
#include <cstring>
#include <iostream>
${cuIncludes}

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
}
}
