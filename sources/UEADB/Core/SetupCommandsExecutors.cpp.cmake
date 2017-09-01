#include "SetupCommandsExecutors.hpp"
#include <UEAA/Utils/CStringToHash.hpp>
${SCE_INCLUDES}

namespace UEADB
{
namespace Application
{
std::map <unsigned, CommandExecutor> SetupCommandExecutors ()
{
    std::map <unsigned int, CommandExecutor> commandExecutors;
    ${SCE_CODE}
    return commandExecutors;
}
}
}
