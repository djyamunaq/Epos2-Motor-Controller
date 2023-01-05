find_package                (Eigen3 REQUIRED)

add_library                ( solver solver.cpp )
target_include_directories ( solver PUBLIC ${CMAKE_SOURCE_DIR}/include )
