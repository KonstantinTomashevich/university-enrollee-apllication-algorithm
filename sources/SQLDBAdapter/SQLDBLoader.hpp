#pragma once
#include <UEAA/Core/University/University.hpp>
#include <UEAA/Utils/DeHashTable.hpp>
#include <Dependencies/SQLite3pp/sqlite3pp.h>

namespace SQLDBAdapter
{
UEAA::University *LoadUniversityFromSQLDataBase (const char *dbFilePath, UEAA::DeHashTable *deHashTable = nullptr);
}
