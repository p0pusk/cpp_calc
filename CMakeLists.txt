cmake_minimum_required(VERSION 3.24)
set(CXX_STANDARD 20)

project(hw1)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)
add_subdirectory(plugins)
add_subdirectory(src)
