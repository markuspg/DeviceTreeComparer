cmake_minimum_required(VERSION 3.7.2 FATAL_ERROR)

project(LibDeviceTreeComparer)

add_library(${PROJECT_NAME}
    device_tree_parser.cpp
    node.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC
    public_headers)
