#ifndef CMDHANDLER_H
#define CMDHANDLER_H

#include "SQL.h"

#ifdef WIN32
    std::string_view constexpr NEW_LINE {"\r\n"};
#else
    std::string_view constexpr NEW_LINE {"\n"};
#endif

/// <summary>
/// Class CmdHandler - creates a database and necessary tables.
/// Converts incoming commands into SQL-queries and returns their result.
/// </summary>
class CmdHandler {
public:
    CmdHandler();

    std::string execute(const std::string& command);

private:
    static std::vector<std::string> split(const std::string &str);
    void create_table(const std::string& name);
    static std::string check_status(const query_result& result);
    static std::string result_to_string(const query_result& result);

    SQL _sql;

};


#endif //CMDHANDLER_H
