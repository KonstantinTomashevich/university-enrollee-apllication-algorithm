#pragma once
#include <UEAA/Core/University/University.hpp>
#include <UEAA/Utils/DeHashTable.hpp>
#include <Dependencies/SQLite3pp/sqlite3pp.h>

namespace SQLDBAdapter
{
bool SaveUniversityToSQLDataBase (const char *dbFilePath, UEAA::University *university, UEAA::DeHashTable *deHashTable = nullptr);
}
