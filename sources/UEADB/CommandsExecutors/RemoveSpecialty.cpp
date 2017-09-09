#include "RemoveSpecialty.hpp"
#include <Dependencies/TinyXML2/tinyxml2.h>
#include <UEAA/Core/University/University.hpp>
#include <UEAA/Core/University/Faculty.hpp>
#include <UEAA/Core/University/Specialty.hpp>

#include <UEADB/Core/ErrorCodes.hpp>
#include <UEADB/Utils/CommandsUtils.hpp>
#include <UEADB/Core/SharedContextGlobalKeys.hpp>
#include <iostream>

namespace UEADB
{
namespace RemoveSpecialty
{
unsigned RemoveSpecialty (const std::vector <std::string> &arguments, SharedPointersMap &sharedContext)
{
    if (arguments.size () != 2)
    {
        std::cout << "    Expected two arguments!" << std::endl;
        return ErrorCodes::INCORRECT_ARGUMENTS;
    }

    UEAA::University *university = GetOrCreateUniversity (sharedContext);
    UEAA::DeHashTable *deHashTable = GetOrCreateDeHashTable (sharedContext);

    unsigned facultyHash = UEAA::CStringToHash (arguments.at (0).c_str (), deHashTable);
    unsigned specialtyHash = UEAA::CStringToHash (arguments.at (1).c_str (), deHashTable);

    UEAA::Faculty *faculty = university->GetFaculty (facultyHash);
    if (faculty == nullptr)
    {
        std::cout << "    Can't find faculty \"" << arguments.at (0) << "\"!" << std::endl;
        return ErrorCodes::INCORRECT_CONTEXT;
    }

    if (!faculty->RemoveSpecialty (specialtyHash))
    {
        std::cout << "    Error, specialty \"" << arguments.at (1) <<
                  "\" is not exists in faculty \"" << arguments.at (0) << "\"!" << std::endl;
        return ErrorCodes::INCORRECT_CONTEXT;
    }
    else
    {
        return 0;
    }
}

void PrintHelp ()
{
    std::cout << "RemoveSpecialty brief:" << std::endl <<
        "    Removes specialty with given name from given faculty." << std::endl << std::endl <<
        "RemoveSpecialty arguments:" << std::endl <<
        "    1. Faculty name." << std::endl <<
        "    2. Specialty name." << std::endl;
}
}
}
