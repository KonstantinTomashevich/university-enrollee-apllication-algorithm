#pragma once
#include <UEADB/Core/TypeDefs.hpp>

namespace UEADB
{
namespace LoadUniversity
{
unsigned LoadUniversity (const std::vector <std::string> &arguments, SharedPointersMap &sharedContext);
void PrintHelp ();
}
}
