cmake_minimum_required(VERSION 3.7.2 FATAL_ERROR)

project(DeviceTreeComparer)
add_executable(${PROJECT_NAME}
    main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE
    LibDeviceTreeComparer)
