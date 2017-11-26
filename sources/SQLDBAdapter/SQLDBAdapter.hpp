#include <Dependencies/TinyXML2/tinyxml2.h>
#include <UEAA/Core/Enrollee/Enrollee.hpp>

#include <cstdlib>
#include <vector>
#include <string>

namespace SQLDBAdapter
{
void PrintHelp ();
int XmlToDbMode (const std::vector <std::string> &arguments);
int DbToXmlMode (const std::vector <std::string> &arguments);
int ProcessDbMode (const std::vector <std::string> &arguments);
}
