cmake_minimum_required(VERSION 3.18)
project(An-Algorithm-Library C CXX)

add_subdirectory(src)
# see https://cliutils.gitlab.io/modern-cmake/chapters/testing.html
if (CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif ()
if ((CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME) AND BUILD_TESTING)
add_subdirectory(tests)
endif ()