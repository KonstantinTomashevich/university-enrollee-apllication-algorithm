#include "RemoveFaculty.hpp"
#include <Dependencies/TinyXML2/tinyxml2.h>
#include <UEAA/Core/University/University.hpp>

#include <UEADB/Core/ErrorCodes.hpp>
#include <UEADB/Utils/CommandsUtils.hpp>
#include <UEADB/Core/SharedContextGlobalKeys.hpp>
#include <iostream>

namespace UEADB
{
namespace RemoveFaculty
{
unsigned RemoveFaculty (const std::vector <std::string> &arguments, SharedPointersMap &sharedContext)
{
    if (arguments.size () != 1)
    {
        std::cout << "    Expected one argument!" << std::endl;
        return ErrorCodes::INCORRECT_ARGUMENTS;
    }

    UEAA::University *university = GetOrCreateUniversity (sharedContext);
    UEAA::DeHashTable *deHashTable = GetOrCreateDeHashTable (sharedContext);
    unsigned hash = UEAA::CStringToHash (arguments.at (0).c_str (), deHashTable);

    if (!university->RemoveFaculty (hash))
    {
        std::cout << "    Error, faculty \"" << arguments.at (0) << "\" is not exists!" << std::endl;
        return ErrorCodes::INCORRECT_CONTEXT;
    }
    else
    {
        return 0;
    }
}

void PrintHelp ()
{
    std::cout << "RemoveFaculty brief:" << std::endl <<
        "    Removes faculty with given name." << std::endl <<
        "RemoveFaculty arguments:" << std::endl <<
        "    1. Faculty name to remove." << std::endl;
}
}
}
