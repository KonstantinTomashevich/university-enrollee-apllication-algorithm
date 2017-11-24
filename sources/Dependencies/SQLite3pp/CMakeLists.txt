add_library (SQLite3pp sqlite3pp.h sqlite3pp.cpp sqlite3ppext.h sqlite3ppext.cpp)
target_link_libraries (SQLite3pp SQLite3)
