cmake_minimum_required(VERSION 3.15)
project(PackageTest CXX)

find_package(EXAMPLE_REST CONFIG REQUIRED)

add_executable(example example.cpp)
target_link_libraries(example EXAMPLE_REST::EXAMPLE_REST)
