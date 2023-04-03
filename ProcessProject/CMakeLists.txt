cmake_minimum_required(VERSION 3.14)
project(OSProject VERSION 1.1.0)

set_property(GLOBAL PROPERTY CXX_STANDARD 20)
set(CMAKE_CXX20_STANDARD_COMPILE_OPTION "-std:c++latest")
set(CMAKE_CXX20_EXTENSION_COMPILE_OPTION "-std:c++latest")
link_libraries("-static")
include_directories(".")

add_executable(${PROJECT_NAME} OSProject.cpp Process.cpp)
