#include <Dependencies/TinyXML2/tinyxml2.h>
#include <UEAA/Core/Enrollee/Enrollee.hpp>

#include <cstdlib>
#include <vector>
#include <string>

namespace EnrolleeRandomizer
{
void PrintHelp ();
bool GenerateNextEnrollee (tinyxml2::XMLElement *enrolleeElement, const std::string &outputDirectory,
                           unsigned passportNumber, unsigned examsDelta, unsigned marksDelta,
                           UEAA::DeHashTable *deHashTable);
}
