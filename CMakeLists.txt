cmake_minimum_required(VERSION 3.11)

set(PATCH_VERSION "1" CACHE INTERNAL "Patch version")
set(PROJECT_VERSION 0.0.${PATCH_VERSION})

project(join_server VERSION ${PROJECT_VERSION})

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

include(FetchContent)
FetchContent_Declare(sqlite3 URL "https://sqlite.org/2023/sqlite-amalgamation-3410000.zip")
FetchContent_MakeAvailable(sqlite3)
if (NOT sqlite3_POPULATED)
    FetchContent_Populate(sqlite3)
endif()

include_directories(${sqlite3_SOURCE_DIR})

find_package(Boost COMPONENTS system unit_test_framework REQUIRED)
find_package(Threads)
#find_package(Boost COMPONENTS REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(join_server_cli main.cpp Server.cpp Session.cpp ${sqlite3_SOURCE_DIR}/sqlite3.c SQL.cpp CmdHandler.cpp)

if (WIN32)
    target_link_libraries(join_server_cli -lws2_32 -lwsock32)
endif()

add_subdirectory(googletest)
add_executable(tests tests.cpp ${sqlite3_SOURCE_DIR}/sqlite3.c SQL.cpp CmdHandler.cpp)
target_link_libraries(tests gtest_main)

set_target_properties(join_server_cli tests PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        )

if (MSVC)
    target_compile_options(join_server_cli PRIVATE
            /W4
            )
    target_compile_options(tests PRIVATE
            /W4
            )
else ()
    target_compile_options(join_server_cli PRIVATE
            -Wall -Wextra -pedantic
            )
    target_compile_options(tests PRIVATE
            -Wall -Wextra -pedantic
            )
endif()

install(TARGETS join_server_cli RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()
add_test(tests tests)
