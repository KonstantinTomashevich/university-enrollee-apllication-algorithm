#pragma once
#include <UEADB/Core/TypeDefs.hpp>

namespace UEADB
{
namespace RemoveFaculty
{
unsigned RemoveFaculty (const std::vector <std::string> &arguments, SharedPointersMap &sharedContext);
void PrintHelp ();
}
}
