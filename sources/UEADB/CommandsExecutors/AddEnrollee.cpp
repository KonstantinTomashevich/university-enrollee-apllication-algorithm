#include "AddEnrollee.hpp"
#include <Dependencies/TinyXML2/tinyxml2.h>
#include <UEAA/Core/University/University.hpp>
#include <UEAA/Core/Enrollee/Enrollee.hpp>
#include <UEAA/Core/Enrollee/EnrolleeHelpers.hpp>

#include <UEADB/Core/ErrorCodes.hpp>
#include <UEADB/Utils/CommandsUtils.hpp>
#include <UEADB/Core/SharedContextGlobalKeys.hpp>
#include <iostream>

namespace UEADB
{
namespace AddEnrollee
{
unsigned AddEnrollee (const std::vector <std::string> &arguments, SharedPointersMap &sharedContext)
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

    tinyxml2::XMLElement *enrolleeXML = document.FirstChildElement ("enrollee");
    if (enrolleeXML == nullptr)
    {
        std::cout << "    <enrollee> tag in XML file root isn't found!" << std::endl;
        return ErrorCodes::INCORRECT_INPUT_FILE_OR_FILES;
    }

    UEAA::University *university = GetOrCreateUniversity (sharedContext);
    UEAA::DeHashTable *deHashTable = GetOrCreateDeHashTable (sharedContext);

    std::cout << "    Loading enrollee from XML..." << std::endl;
    std::string passport = enrolleeXML->Attribute ("passport");
    UEAA::SharedPointer <UEAA::Enrollee> enrollee (new UEAA::Enrollee (UEAA::CStringToHash (passport.c_str (), deHashTable)));
    enrollee->LoadFromXML (enrolleeXML, deHashTable);

    if (!university->AddEnrollee (enrollee))
    {
        std::cout << "    Error, enrollee \"" << passport << "\" is already exists!" << std::endl;
        return ErrorCodes::INCORRECT_CONTEXT;
    }
    else
    {
        return 0;
    }
}

void PrintHelp ()
{
    std::cout << "AddEnrollee brief:" << std::endl <<
        "    Loads enrollee from given XML file and" << std::endl <<
        "    adds it to university." << std::endl << std::endl <<
        "AddEnrollee arguments:" << std::endl <<
        "    1. Path to XML file with <enrollee> core tag." << std::endl;
}
}
}
