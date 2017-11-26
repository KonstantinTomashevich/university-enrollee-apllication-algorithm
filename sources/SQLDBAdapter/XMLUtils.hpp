#pragma once
#include <UEAA/Core/University/University.hpp>
#include <UEAA/Utils/DeHashTable.hpp>

namespace SQLDBAdapter
{
namespace XMLUtils
{
UEAA::University *LoadUniversity (const char *xmlFilePath, UEAA::DeHashTable *deHashTable = nullptr);
bool SaveUniversity (const char *xmlFilePath, UEAA::University *university, UEAA::DeHashTable *deHashTable);
}
}
