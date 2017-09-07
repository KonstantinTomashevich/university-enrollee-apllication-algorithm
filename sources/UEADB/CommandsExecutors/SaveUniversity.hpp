#pragma once
#include <UEADB/Core/TypeDefs.hpp>

namespace UEADB
{
namespace SaveUniversity
{
unsigned SaveUniversity (const std::vector <std::string> &arguments, SharedPointersMap &sharedContext);
void PrintHelp ();
}
}
