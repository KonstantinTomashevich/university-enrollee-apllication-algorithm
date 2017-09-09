#include "AddSpecialty.hpp"
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
namespace AddSpecialty
{
unsigned AddSpecialty (const std::vector <std::string> &arguments, SharedPointersMap &sharedContext)
{
    if (arguments.size () != 2)
    {
        std::cout << "    Expected two arguments!" << std::endl;
        return ErrorCodes::INCORRECT_ARGUMENTS;
    }

    UEAA::University *university = GetOrCreateUniversity (sharedContext);
    UEAA::DeHashTable *deHashTable = GetOrCreateDeHashTable (sharedContext);

    unsigned facultyHash = UEAA::CStringToHash (arguments.at (0).c_str (), deHashTable);
    UEAA::Faculty *faculty = university->GetFaculty (facultyHash);
    if (faculty == nullptr)
    {
        std::cout << "    Can't find faculty \"" << arguments.at (0) << "\"!" << std::endl;
        return ErrorCodes::INCORRECT_CONTEXT;
    }

    tinyxml2::XMLDocument document;
    std::cout << "    Loading XML..." << std::endl;
    tinyxml2::XMLError loadingResult = document.LoadFile (arguments.at (1).c_str ());

    if (loadingResult != tinyxml2::XML_SUCCESS)
    {
        std::cout << "    Can't load XML file, tinyxml2 error: " << document.ErrorIDToName (loadingResult) << "." << std::endl;
        return ErrorCodes::INCORRECT_INPUT_FILE_OR_FILES;
    }

    tinyxml2::XMLElement *specialtyXML = document.FirstChildElement ("specialty");
    if (specialtyXML == nullptr)
    {
        std::cout << "    <specialty> tag in XML file root isn't found!" << std::endl;
        return ErrorCodes::INCORRECT_INPUT_FILE_OR_FILES;
    }

    std::cout << "    Loading specialty from XML..." << std::endl;
    UEAA::SharedPointer <UEAA::Specialty> specialty (new UEAA::Specialty (
        faculty, UEAA::CStringToHash (specialtyXML->Attribute ("name"), deHashTable)));
    specialty->LoadFromXML (specialtyXML, deHashTable);

    if (!faculty->AddSpecialty (specialty))
    {
        std::cout << "    Error, specialty \"" << specialtyXML->Attribute ("name") <<
            "\" is already exists in faculty \"" << arguments.at (0) << "\"!" << std::endl;
        return ErrorCodes::INCORRECT_CONTEXT;
    }
    else
    {
        return 0;
    }
}

void PrintHelp ()
{
    std::cout << "AddSpecialty brief:" << std::endl <<
        "    Loads specialty from given XML file and" << std::endl <<
        "    adds it to specified faculty." << std::endl << std::endl <<
        "AddSpecialty arguments:" << std::endl <<
        "    1. Faculty name." << std::endl <<
        "    2. Path to XML file with <specialty> core tag." << std::endl;
}
}
}
