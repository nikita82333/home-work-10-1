#include "gtest/gtest.h"

#include "SQL.h"
#include "CmdHandler.h"

TEST(sql, test1) {
    const char* query {
        "CREATE TABLE A (id INTEGER PRiMARY KEY, value TEXT);"
        "CREATE TABLE B (id INTEGER PRiMARY KEY, value TEXT);"
        "INSERT INTO A VALUES (0, 'A_value0');"
        "INSERT INTO A VALUES (1, 'A_value1');"
        "INSERT INTO A VALUES (2, 'A_value2');"
        "INSERT INTO B VALUES (1, 'B_value1');"
        "INSERT INTO B VALUES (2, 'B_value2');"
        "INSERT INTO B VALUES (3, 'B_value3');"
        "SELECT a.id, a.value AS A, b.value AS B FROM A AS a, B as b WHERE a.id = b.id"
    };
    const query_result expected {
        {"id", "A", "B"},
        {"1", "A_value1", "B_value1"},
        {"2", "A_value2", "B_value2"}
    };
    SQL sql;
    query_result result {sql.execute_query(query)};
    ASSERT_EQ(result, expected);
}

TEST(sql, test2) {
    const char* query {
        "CREATE TABLE A (id INTEGER PRiMARY KEY, b_id INTEGER);"
        "CREATE TABLE B (id INTEGER PRiMARY KEY, value TEXT);"
        "INSERT INTO A VALUES (0, 1);"
        "INSERT INTO A VALUES (1, 2);"
        "INSERT INTO A VALUES (2, 3);"
        "INSERT INTO B VALUES (1, 'B_value1');"
        "INSERT INTO B VALUES (2, 'B_value2');"
        "INSERT INTO B VALUES (3, 'B_value3');"
        "SELECT A.id, B.value FROM A JOIN B ON A.b_id = B.id"
    };
    const query_result expected {
        {"id", "value"},
        {"0", "B_value1"},
        {"1", "B_value2"},
        {"2", "B_value3"}
    };
    SQL sql;
    query_result result {sql.execute_query(query)};
    ASSERT_EQ(result, expected);
}

TEST(cmd, test_unique_id) {
    CmdHandler handler;
    handler.execute("INSERT A 0 lean");
    std::string result {handler.execute("INSERT A 0 understand")};
    const std::string expected {"ERR duplicate 0"};
    ASSERT_EQ(result, expected);
}

TEST(cmd, test_intersection) {
    CmdHandler handler;
    handler.execute("INSERT A 0 lean");
    handler.execute("INSERT A 1 sweater");
    handler.execute("INSERT A 2 frank");
    handler.execute("INSERT A 3 violation");
    handler.execute("INSERT A 4 quality");
    handler.execute("INSERT A 5 precision");

    handler.execute("INSERT B 3 proposal");
    handler.execute("INSERT B 4 example");
    handler.execute("INSERT B 5 lake");
    handler.execute("INSERT B 6 flour");
    handler.execute("INSERT B 7 wonder");
    handler.execute("INSERT B 8 selection");

    std::string result {handler.execute("INTERSECTION")};

    const std::string expected {
        "3,violation,proposal" + std::string(NEW_LINE) +
        "4,quality,example" + std::string(NEW_LINE) +
        "5,precision,lake" + std::string(NEW_LINE) +
        "OK"
    };
    ASSERT_EQ(result, expected);
}

TEST(cmd, test_symmetric_difference) {
    CmdHandler handler;
    handler.execute("INSERT A 0 lean");
    handler.execute("INSERT A 1 sweater");
    handler.execute("INSERT A 2 frank");
    handler.execute("INSERT A 3 violation");
    handler.execute("INSERT A 4 quality");
    handler.execute("INSERT A 5 precision");

    handler.execute("INSERT B 3 proposal");
    handler.execute("INSERT B 4 example");
    handler.execute("INSERT B 5 lake");
    handler.execute("INSERT B 6 flour");
    handler.execute("INSERT B 7 wonder");
    handler.execute("INSERT B 8 selection");

    std::string result {handler.execute("SYMMETRIC_DIFFERENCE")};

    const std::string expected {
        "0,lean," + std::string(NEW_LINE) +
        "1,sweater," + std::string(NEW_LINE) +
        "2,frank," + std::string(NEW_LINE) +
        "6,,flour" + std::string(NEW_LINE) +
        "7,,wonder" + std::string(NEW_LINE) +
        "8,,selection" + std::string(NEW_LINE) +
        "OK"
    };
    ASSERT_EQ(result, expected);
}