project(PerchFish)
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




set(SRC 
    src/ChessMove.cpp
    src/ChessState.cpp
)

set(TEST_SRC
    tests/ChessMoveTest.cpp
    tests/ChessStateTest.cpp
)

add_library(PerchFish ${SRC})

add_executable(PerchFishTest ${TEST_SRC})

target_link_libraries(PerchFishTest PerchFish gtest gtest_main)

enable_testing()
