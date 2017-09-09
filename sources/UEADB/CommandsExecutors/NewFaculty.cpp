#include "NewFaculty.hpp"
#include <Dependencies/TinyXML2/tinyxml2.h>
#include <UEAA/Core/University/University.hpp>

#include <UEADB/Core/ErrorCodes.hpp>
#include <UEADB/Utils/CommandsUtils.hpp>
#include <UEADB/Core/SharedContextGlobalKeys.hpp>
#include <iostream>

namespace UEADB
{
namespace NewFaculty
{
unsigned NewFaculty (const std::vector <std::string> &arguments, SharedPointersMap &sharedContext)
{
    if (arguments.size () != 1)
    {
        std::cout << "    Expected one argument!" << std::endl;
        return ErrorCodes::INCORRECT_ARGUMENTS;
    }

    UEAA::University *university = GetOrCreateUniversity (sharedContext);
    UEAA::DeHashTable *deHashTable = GetOrCreateDeHashTable (sharedContext);
    unsigned hash = UEAA::CStringToHash (arguments.at (0).c_str (), deHashTable);

    UEAA::SharedPointer <UEAA::Faculty> faculty (new UEAA::Faculty (university, hash));
    if (!university->AddFaculty (faculty))
    {
        std::cout << "    Error, faculty \"" << arguments.at (0) << "\" is already exists!" << std::endl;
        return ErrorCodes::INCORRECT_CONTEXT;
    }
    else
    {
        return 0;
    }
}

void PrintHelp ()
{
    std::cout << "NewFaculty brief:" << std::endl <<
        "    Creates new empty faculty with given name." << std::endl << std::endl <<
        "NewFaculty arguments:" << std::endl <<
        "    1. New faculty name." << std::endl;
}
}
}
