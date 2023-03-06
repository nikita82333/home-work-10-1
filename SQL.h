#ifndef SQL_H
#define SQL_H

#include <string>
#include <vector>
#include <iostream>

#include "sqlite3.h"

using query_result = std::vector<std::vector<std::string>>;

/// <summary>
/// Class SQL - SQL database engine.
/// </summary>
class SQL {
public:
    SQL();
    ~SQL();

    query_result execute_query(const char* query);

private:
    const char* _db_file_name {":memory:"};
    sqlite3* _handle {nullptr};
};


#endif //SQL_H
