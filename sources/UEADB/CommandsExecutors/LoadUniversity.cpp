#include "LoadUniversity.hpp"
#include <Dependencies/TinyXML2/tinyxml2.h>
#include <UEAA/Core/University/University.hpp>

#include <UEADB/Core/ErrorCodes.hpp>
#include <UEADB/Core/CommandsUtils.hpp>
#include <UEADB/Core/SharedContextGlobalKeys.hpp>
#include <iostream>

namespace UEADB
{
namespace LoadUniversity
{
unsigned LoadUniversity (const std::vector <std::string> &arguments, SharedPointersMap &sharedContext)
{
    if (arguments.size () != 1)
    {
        std::cout << "    Expected one argument!" << std::endl;
        return ErrorCodes::INCORRECT_ARGUMENTS;
    }

    tinyxml2::XMLDocument document;
    std::cout << "    Loading XML..." << std::endl;
    tinyxml2::XMLError loadingResult = document.LoadFile (arguments.at (0).c_str ());

    if (loadingResult != tinyxml2::XML_SUCCESS)
    {
        std::cout << "    Can't load XML file, tinyxml2 error: " << document.ErrorIDToName (loadingResult) << "." << std::endl;
        return ErrorCodes::INCORRECT_INPUT_FILE_OR_FILES;
    }

    tinyxml2::XMLElement *universityXML = document.FirstChildElement ("university");
    if (universityXML == nullptr)
    {
        std::cout << "    <university> tag in XML file root isn't found!" << std::endl;
        return ErrorCodes::INCORRECT_INPUT_FILE_OR_FILES;
    }

    std::cout << "    Loading university from XML..." << std::endl;
    UEAA::University *university = new UEAA::University ();
    UEAA::DeHashTable *deHashTable = Application::GetOrCreateDeHashTable (sharedContext);
    university->LoadFromXML (universityXML, deHashTable);

    UEAA::SharedPointer <UEAA::ReferenceCounted> sharedPointer (university);
    sharedContext.emplace (SharedContextGlobalKeys::UNIVERSITY, sharedPointer);
    return 0;
}

void PrintHelp ()
{
    std::cout << "LoadUniversity brief:" << std::endl <<
        "    Loads university from given XML file." << std::endl <<
        "    Loaded university will be stored to commands shared context." << std::endl << std::endl <<
        "LoadUniversity arguments:" << std::endl <<
        "    1. Path to XML file with <university> core tag." << std::endl;
}
}
}
