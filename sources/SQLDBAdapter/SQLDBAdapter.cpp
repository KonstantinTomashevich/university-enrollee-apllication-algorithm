#include "SQLDBAdapter.hpp"
#include <UEAA/Core/University/University.hpp>
#include <Dependencies/CrossPlatformMain/CrossPlatformMain.hpp>
#include <Dependencies/TinyXML2/tinyxml2.h>

#include <SQLDBAdapter/XMLUtils.hpp>
#include <SQLDBAdapter/ErrorCodes.hpp>
#include <SQLDBAdapter/SQLDBSaver.hpp>
#include <SQLDBAdapter/SQLDBLoader.hpp>
#include <iostream>

int CrossPlatformMain (const std::vector <std::string> &arguments)
{
    if (arguments.empty ())
    {
        SQLDBAdapter::PrintHelp ();
        return 0;
    }
    else
    {
        std::string mode = arguments [0];
        if (mode == "--Help")
        {
            SQLDBAdapter::PrintHelp ();
            return 0;
        }
        else if (mode == "--XmlToDb")
        {
            return SQLDBAdapter::XmlToDbMode (arguments);
        }
        else if (mode == "--DbToXml")
        {
            return SQLDBAdapter::DbToXmlMode (arguments);
        }
        else if (mode == "--ProcessDb")
        {
            return SQLDBAdapter::ProcessDbMode (arguments);
        }
        else
        {
            std::cout << "Unknown mode! Terminating..." << std::endl;
            return SQLDBAdapter::ErrorCodes::UNKNOWN_MODE;
        }
    }
}

namespace SQLDBAdapter
{
const char *HELP_ARTICLE =
        "UEAA SQL DB Adapter help.\n"
                "First argument is mode. List of modes:\n"
                "    --Help\n"
                "        Prints this message.\n\n"
                "    --XmlToDb\n"
                "        Converts university from XML storage to database.\n"
                "        Arg1: xml file name.\n"
                "        Arg2: output database file name.\n\n"
                "    --DbToXml\n"
                "        Converts university from database to xml storage.\n"
                "        Arg1: database file name.\n"
                "        Arg2: output xml file name.\n\n"
                "    --ProcessDb\n"
                "        Processes given database.\n"
                "        Arg1: database filename.\n\n";

void PrintHelp ()
{
    std::cout << HELP_ARTICLE;
}

int XmlToDbMode (const std::vector <std::string> &arguments)
{
    if (arguments.size () != 3)
    {
        std::cout << "Expected 2 arguments after mode name, execute --Help mode for more info." << std::endl;
        return ErrorCodes::INCORRECT_ARGUMENTS;
    }

    UEAA::SharedPointer <UEAA::DeHashTable> deHashTable (new UEAA::DeHashTable ());
    UEAA::SharedPointer <UEAA::University> university (XMLUtils::LoadUniversity (arguments.at (1).c_str (), deHashTable));

    if (university.GetTrackingObject () == nullptr)
    {
        return ErrorCodes::INCORRECT_INPUT_FILE;
    }

    if (SaveUniversityToSQLDataBase (arguments.at (2).c_str (), university, deHashTable))
    {
        return 0;
    }
    else
    {
        return ErrorCodes::INCORRECT_OUTPUT_FILE;
    }
}

int DbToXmlMode (const std::vector <std::string> &arguments)
{
    if (arguments.size () != 3)
    {
        std::cout << "Expected 2 arguments after mode name, execute --Help mode for more info." << std::endl;
        return ErrorCodes::INCORRECT_ARGUMENTS;
    }

    UEAA::SharedPointer <UEAA::DeHashTable> deHashTable (new UEAA::DeHashTable ());
    UEAA::SharedPointer <UEAA::University> university (LoadUniversityFromSQLDataBase (arguments.at (1).c_str (), deHashTable));

    if (university.GetTrackingObject () == nullptr)
    {
        return ErrorCodes::INCORRECT_INPUT_FILE;
    }

    if (XMLUtils::SaveUniversity (arguments.at (2).c_str (), university, deHashTable))
    {
        return 0;
    }
    else
    {
        return ErrorCodes::INCORRECT_OUTPUT_FILE;
    }
}

int ProcessDbMode (const std::vector <std::string> &arguments)
{
    if (arguments.size () != 2)
    {
        std::cout << "Expected 1 argument after mode name, execute --Help mode for more info." << std::endl;
        return ErrorCodes::INCORRECT_ARGUMENTS;
    }

    UEAA::SharedPointer <UEAA::University> university (LoadUniversityFromSQLDataBase (arguments.at (1).c_str ()));
    if (university.GetTrackingObject () == nullptr)
    {
        return ErrorCodes::INCORRECT_INPUT_FILE;
    }

    university->ProcessEnrolleesApplication ();
    if (SaveUniversityToSQLDataBase (arguments.at (1).c_str (), university))
    {
        return 0;
    }
    else
    {
        return ErrorCodes::INCORRECT_OUTPUT_FILE;
    }
}
}
