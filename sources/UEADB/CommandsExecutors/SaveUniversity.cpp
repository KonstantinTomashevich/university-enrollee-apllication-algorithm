#include "SaveUniversity.hpp"
#include <Dependencies/TinyXML2/tinyxml2.h>
#include <UEAA/Core/University/University.hpp>

#include <UEADB/Core/ErrorCodes.hpp>
#include <UEADB/Utils/CommandsUtils.hpp>
#include <UEADB/Core/SharedContextGlobalKeys.hpp>
#include <iostream>

namespace UEADB
{
namespace SaveUniversity
{
unsigned SaveUniversity (const std::vector <std::string> &arguments, SharedPointersMap &sharedContext)
{
    if (arguments.size () != 1)
    {
        std::cout << "    Expected one argument!" << std::endl;
        return ErrorCodes::INCORRECT_ARGUMENTS;
    }

    UEAA::University *university = GetOrCreateUniversity (sharedContext);
    tinyxml2::XMLDocument document;
    tinyxml2::XMLElement *rootElement = document.NewElement ("university");
    document.InsertFirstChild (rootElement);

    UEAA::DeHashTable *deHashTable = GetOrCreateDeHashTable (sharedContext);
    std::cout << "    Saving university to XML..." << std::endl;
    university->SaveToXML (document, rootElement, deHashTable);
    std::cout << "    Saving XML..." << std::endl;
    tinyxml2::XMLError savingResult = document.SaveFile (arguments.at (0).c_str ());

    if (savingResult == tinyxml2::XML_SUCCESS)
    {
        return 0;
    }
    else
    {
        std::cout << "    Can't save XML file, tinyxml2 error: " << document.ErrorIDToName (savingResult) << "." << std::endl;
        return ErrorCodes::INCORRECT_OUTPUT_FILE_OR_FILES;
    }
}

void PrintHelp ()
{
    std::cout << "SaveUniversity brief:" << std::endl <<
        "    Saves university to given XML file." << std::endl << std::endl <<
        "SaveUniversity arguments:" << std::endl <<
        "    1. Path to save university as XML file." << std::endl;
}
}
}
