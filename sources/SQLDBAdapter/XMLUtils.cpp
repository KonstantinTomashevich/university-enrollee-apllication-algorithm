#include "XMLUtils.hpp"
#include <iostream>

namespace SQLDBAdapter
{
namespace XMLUtils
{
UEAA::University *LoadUniversity (const char *xmlFilePath, UEAA::DeHashTable *deHashTable)
{
    tinyxml2::XMLDocument document;
    std::cout << "Loading XML..." << std::endl;
    tinyxml2::XMLError loadingResult = document.LoadFile (xmlFilePath);

    if (loadingResult != tinyxml2::XML_SUCCESS)
    {
        std::cout << "Can't load XML file, tinyxml2 error: " << document.ErrorIDToName (loadingResult) << "." << std::endl;
        return nullptr;
    }

    tinyxml2::XMLElement *universityXML = document.FirstChildElement ("university");
    if (universityXML == nullptr)
    {
        std::cout << "<university> tag in XML file root isn't found!" << std::endl;
        return nullptr;
    }

    std::cout << "Loading university from XML..." << std::endl;
    UEAA::University *university = new UEAA::University ();

    bool deleteDeHashTable = false;
    if (deHashTable == nullptr)
    {
        deHashTable = new UEAA::DeHashTable ();
        deleteDeHashTable = true;
    }

    university->LoadFromXML (universityXML, deHashTable);
    if (deleteDeHashTable)
    {
        delete deHashTable;
    }
    return university;
}

bool SaveUniversity (const char *xmlFilePath, UEAA::University *university, UEAA::DeHashTable *deHashTable)
{
    tinyxml2::XMLDocument document;
    tinyxml2::XMLElement *rootElement = document.NewElement ("university");
    document.InsertFirstChild (rootElement);

    std::cout << "Saving university to XML..." << std::endl;
    university->SaveToXML (document, rootElement, deHashTable);
    std::cout << "Saving XML..." << std::endl;
    tinyxml2::XMLError savingResult = document.SaveFile (xmlFilePath);

    if (savingResult == tinyxml2::XML_SUCCESS)
    {
        return true;
    }
    else
    {
        std::cout << "Can't save XML file, tinyxml2 error: " << document.ErrorIDToName (savingResult) << "." << std::endl;
        return false;
    }
}
}
}
