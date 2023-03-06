#include <sstream>

#include "CmdHandler.h"

CmdHandler::CmdHandler() {
    create_table("A");
    create_table("B");
}

std::string CmdHandler::execute(const std::string& command) {
    std::string query;
    std::vector<std::string> words {split(command)};
    if (words.empty()) {
        return "ERR empty command";
    }
    if (words[0] == "INSERT") {
        if (words.size() >= 4) {
            query = "INSERT INTO " + words[1] + " VALUES(" + words[2] + ", '" + words[3] + "');";
        } else {
            return "ERR incomplete command";
        }
    } else if (words[0] == "TRUNCATE") {
        if (words.size() >= 2) {
            query = "DELETE FROM " + words[1] + ";";
        } else {
            return "ERR incomplete command";
        }
    } else if (words[0] == "INTERSECTION") {
        query = "SELECT a.id, a.name, b.name FROM A AS a, B AS b WHERE a.id = b.id;";
    } else if (words[0] == "SYMMETRIC_DIFFERENCE") {
        query = "SELECT id, name, '' AS name FROM A WHERE id NOT IN (SELECT id FROM B) UNION "
                "SELECT id, '' AS name, name FROM B WHERE id NOT IN (SELECT id FROM A);";
    } else {
        return "ERR unrecognized command";
    }

    query_result result {_sql.execute_query(query.c_str())};
    std::string status {check_status(result)};
    if (status == "ERR UNIQUE constraint failed: A.id" ||
        status == "ERR UNIQUE constraint failed: B.id") {
        status = "ERR duplicate " + words[2]; //for INSERT only
    }

    return result_to_string(result) + status;
}

std::vector<std::string> CmdHandler::split(const std::string &str) {
    std::vector<std::string> result;
    std::string word;
    std::stringstream ss(str);

    while (ss >> word) {
        result.push_back(word);
    }

    return result;
}

void CmdHandler::create_table(const std::string& name) {
    std::string query {"CREATE TABLE " + name + "(id INTEGER PRIMARY KEY, name TEXT)"};
    _sql.execute_query(query.c_str());
}

std::string CmdHandler::check_status(const query_result& result) {
    if (!result.empty() && result[0][0] == "ERR") {
        return "ERR " + result[0][1];
    } else {
        return "OK";
    }
}

std::string CmdHandler::result_to_string(const query_result& result) {
    std::string str;
    for (auto it_row = result.cbegin() + 1; it_row < result.cend(); ++it_row) {
        for (auto it_value = it_row->cbegin(); it_value < it_row->cend(); ++it_value) {
            if (it_value != it_row->cbegin()) {
                str += ',';
            }
            str += *it_value;
        }
        str += NEW_LINE;
    }
    return str;
}
