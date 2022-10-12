cmake_minimum_required(VERSION 2.8.12)
project (Epos2TestModule)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_executable(epos_example ${CMAKE_SOURCE_DIR}/epos_example.cpp)
target_link_libraries(epos_example PUBLIC MotorController-lib)

include(${CMAKE_SOURCE_DIR}/MotorController/CMakeLists.txt)


