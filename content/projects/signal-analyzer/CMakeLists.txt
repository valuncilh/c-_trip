cmake_minimum_required(VERSION 3.20)
project(signal_analyzer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(statistics statistics.cpp)
target_include_directories(statistics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(signal_analyzer main.cpp)
target_link_libraries(signal_analyzer PRIVATE statistics)

include(CTest)
if(BUILD_TESTING)
    add_executable(statistics_tests tests.cpp)
    target_link_libraries(statistics_tests PRIVATE statistics)
    add_test(NAME statistics_tests COMMAND statistics_tests)
endif()
