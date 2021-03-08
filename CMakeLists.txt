cmake_minimum_required(VERSION 3.18)
project(An-Algorithm-Library C CXX)

set(CMAKE_CXX_STANDARD 20) ## Supported values are 98, 11, 14, 17, and 20.
set(CMAKE_CXX_STANDARD_REQUIRED ON) ## the value of the CXX_STANDARD target property is treated as a requirement
set(CMAKE_CXX_EXTENSIONS OFF) ## on g++ this ensures: -std=c++11 and not -std=gnu++11
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON) ## automatically create a module definition (.def) file with all global
                                         ## symbols found in the input .obj files for a SHARED library on Windows.

add_subdirectory(src)
enable_testing()
add_subdirectory(tests)
