cmake_minimum_required(VERSION 3.10)

project(
    robot_control
    VERSION 0.1
    DESCRIPTION "project_template"
    LANGUAGES CXX)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

add_subdirectory(src)
add_subdirectory(apps)