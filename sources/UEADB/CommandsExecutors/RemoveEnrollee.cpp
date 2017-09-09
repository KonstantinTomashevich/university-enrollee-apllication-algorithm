#include "RemoveEnrollee.hpp"
#include <Dependencies/TinyXML2/tinyxml2.h>
#include <UEAA/Core/University/University.hpp>

#include <UEADB/Core/ErrorCodes.hpp>
#include <UEADB/Utils/CommandsUtils.hpp>
#include <UEADB/Core/SharedContextGlobalKeys.hpp>
#include <iostream>

namespace UEADB
{
namespace RemoveEnrollee
{
unsigned RemoveEnrollee (const std::vector <std::string> &arguments, SharedPointersMap &sharedContext)
{
    if (arguments.size () != 1)
    {
        std::cout << "    Expected one argument!" << std::endl;
        return ErrorCodes::INCORRECT_ARGUMENTS;
    }
    UEAA::University *university = GetOrCreateUniversity (sharedContext);
    UEAA::DeHashTable *deHashTable = GetOrCreateDeHashTable (sharedContext);
    unsigned hash = UEAA::CStringToHash (arguments.at (0).c_str (), deHashTable);

    if (!university->RemoveEnrollee (hash))
    {
        std::cout << "    Error, enrollee \"" << arguments.at (0) << "\" is not exists!" << std::endl;
        return ErrorCodes::INCORRECT_CONTEXT;
    }
    else
    {
        return 0;
    }
}

void PrintHelp ()
{
    std::cout << "RemoveEnrollee brief:" << std::endl <<
        "    Removes enrollee with given passport data." << std::endl << std::endl <<
        "RemoveEnrollee arguments:" << std::endl <<
        "    1. Enrollee passport data (for example AB1234567)." << std::endl;
}
}
}
