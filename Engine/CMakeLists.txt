cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

include(FetchContent)

# Fetch GoogleTest
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

# Fetch HTTP library (cpp-httplib)
FetchContent_Declare(
    httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG v0.10.8
)
FetchContent_MakeAvailable(httplib)

# Fetch SQLite3
FetchContent_Declare(
    sqlite3
    GIT_REPOSITORY https://github.com/sqlite/sqlite.git
    GIT_TAG version-3.43.2  # Use a stable version
)
FetchContent_MakeAvailable(sqlite3)


set(SRC 
src/ChessMove.cpp
src/ChessState.cpp
src/Engine.cpp
src/PositionORM.cpp
src/ChessServer.cpp
)

set(TEST_SRC
tests/ChessMoveTest.cpp
tests/ChessStateTest.cpp
)

add_library(PerchFish ${SRC})

add_executable(PerchFishTest ${TEST_SRC})

add_executable(PerchFishMain src/main.cpp ${SRC})

target_include_directories(PerchFishMain PUBLIC ${sqlite3_SOURCE_DIR}/include ${httplib_SOURCE_DIR})

target_link_libraries(PerchFishMain PerchFish httplib::httplib sqlite3)

target_include_directories(PerchFish PUBLIC include ${sqlite3_SOURCE_DIR}/include ${httplib_SOURCE_DIR})

target_link_libraries(PerchFishTest PerchFish gtest gtest_main sqlite3)

enable_testing()
