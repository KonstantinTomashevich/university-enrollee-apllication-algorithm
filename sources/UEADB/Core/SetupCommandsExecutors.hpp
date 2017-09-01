#pragma once
#include <UEADB/Core/TypeDefs.hpp>

namespace UEADB
{
namespace Application
{
std::map <unsigned, CommandExecutor> SetupCommandExecutors ();
}
}