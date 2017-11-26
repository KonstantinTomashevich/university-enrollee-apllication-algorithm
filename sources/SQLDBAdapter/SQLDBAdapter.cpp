#include "SQLDBAdapter.hpp"
#include <UEAA/Core/University/University.hpp>
#include <Dependencies/CrossPlatformMain/CrossPlatformMain.hpp>
#include <Dependencies/TinyXML2/tinyxml2.h>

#include <SQLDBAdapter/XMLUtils.hpp>
#include <SQLDBAdapter/ErrorCodes.hpp>
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
        if (mode == "--XmlToDb")
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
    UEAA::SharedPointer <UEAA::DeHashTable> deHashTable (new UEAA::DeHashTable ());
    UEAA::SharedPointer <UEAA::University> university (XMLUtils::LoadUniversity (arguments.at (1).c_str (), deHashTable));

    if (university.GetTrackingObject () == nullptr)
    {
        return ErrorCodes::INCORRECT_INPUT_FILE;
    }


    return 0;
}

int DbToXmlMode (const std::vector <std::string> &arguments)
{
    return 0;
}

int ProcessDbMode (const std::vector <std::string> &arguments)
{
    return 0;
}
}
