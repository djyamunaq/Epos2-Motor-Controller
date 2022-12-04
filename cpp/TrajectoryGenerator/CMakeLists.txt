cmake_minimum_required(VERSION 2.8.12)
project (TrajectoryGeneratorModule)

find_package (Eigen3 3.3.7 REQUIRED)

add_library                (TrajectoryGenerator-lib STATIC ${CMAKE_SOURCE_DIR}/TrajectoryGenerator/src/TrajectoryGenerator.cpp)
target_link_libraries      (TrajectoryGenerator-lib Eigen3::Eigen)
target_include_directories (TrajectoryGenerator-lib PUBLIC ${CMAKE_SOURCE_DIR}/TrajectoryGenerator/include/)

