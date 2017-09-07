#pragma once
#include <UEADB/Core/TypeDefs.hpp>

namespace UEADB
{
namespace ProcessApplication
{
unsigned ProcessApplication (const std::vector <std::string> &arguments, SharedPointersMap &sharedContext);
void PrintHelp ();
}
}
