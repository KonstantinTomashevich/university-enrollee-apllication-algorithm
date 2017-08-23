#pragma once
#include <UEAA/BuildConfiguration.hpp>

#ifdef UEAA_XML_IO
#include <UEAA/Core/University/University.hpp>
#include <UEAA/Utils/DeHashTable.hpp>

namespace tinyxml2
{
class XMLElement;
class XMLDocument;
}

namespace UEAA
{
namespace XMLIO
{
University *LoadFromFile (const char *fileName, DeHashTable *deHashTable);
bool SaveToFile (const char *fileName, University *university, DeHashTable *deHashTable);

void SaveUniversityToXML (tinyxml2::XMLDocument &document, University *university, DeHashTable *deHashTable);
void SaveFacultyToXML (tinyxml2::XMLDocument &document, tinyxml2::XMLElement *output, Faculty *faculty, DeHashTable *deHashTable);
void SaveSpecialtyToXML (tinyxml2::XMLDocument &document, tinyxml2::XMLElement *output, Specialty *specialty, DeHashTable *deHashTable);
void SaveEnrolleeToXML (tinyxml2::XMLDocument &document, tinyxml2::XMLElement *output, Enrollee *enrollee, DeHashTable *deHashTable);
}
}
#endif
