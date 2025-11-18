cmake_minimum_required(VERSION 3.10)
project(PiXELGraph)

set(CMAKE_CXX_STANDARD 17)

include_directories(Include)

file(GLOB SOURCES "Source/*.cpp")
add_executable(PiXELGraph ${SOURCES})