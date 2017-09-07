#include <Dependencies/TinyXML2/tinyxml2.h>
#include <UEAA/Core/Enrollee/Enrollee.hpp>
#include <cstdlib>
#include <vector>
#include <string>

namespace EnrolleeRandomizer
{
// TODO: Maybe move this functions and main definitions to special module? They are the same in UEADB and EnrolleeRandomizer;
unsigned Execute (const wchar_t *arguments);
unsigned Execute (int argumentsCount, char **arguments);
unsigned Execute (const std::vector <std::string> &arguments);
std::vector <std::string> ParseArgumentsString (const wchar_t *arguments);

void PrintHelp ();
void GenerateNextEnrollee (tinyxml2::XMLElement *enrolleeElement, const std::string &outputDirectory,
                           unsigned passportNumber, unsigned examsDelta, unsigned marksDelta,
                           UEAA::DeHashTable *deHashTable);
}
