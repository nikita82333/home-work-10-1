#include "SQL.h"

SQL::SQL() {
    if (sqlite3_threadsafe() != 0) {
        sqlite3_config(SQLITE_CONFIG_SERIALIZED);//should be by default
    } else {
        std::cerr << "The database engine operates in single-threaded mode. "
                     "Only one connection is supported." << std::endl;
    }
    if (sqlite3_open(_db_file_name, &_handle)) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(_handle) << std::endl;
        sqlite3_close(_handle);
    }
}

SQL::~SQL() {
    sqlite3_close(_handle);
}

query_result SQL::execute_query(const char* query) {

    auto get_results = [](void* ptr_result, int columns, char** data, char** names) -> int {
        auto* results = reinterpret_cast<query_result*>(ptr_result);
        if (results->empty()) {
            results->emplace_back();
            for (int i = 0; i < columns; ++i) {
                results->back().emplace_back(names[i]);
            }
        }
        results->emplace_back();
        for (int i = 0; i < columns; ++i) {
            results->back().emplace_back(data[i]);
        }
        return 0;
    };

    char* errmsg;
    query_result result;
    auto ptr_result = reinterpret_cast<void*>(&result);
    int status = sqlite3_exec(_handle, query, get_results, ptr_result, &errmsg);
    if (status != SQLITE_OK) {
        std::string err_message {errmsg};
        sqlite3_free(errmsg);
        return {{"ERR", err_message}};
    }
    return result;
}
