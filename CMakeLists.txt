cmake_minimum_required(VERSION 3.10)
if(POLICY CMP0079 )
  cmake_policy(SET CMP0079 NEW)
endif()

# Project name
project(MyProject)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add subdirectories
add_subdirectory(src/algorithm)
add_subdirectory(src/robot)
add_subdirectory(src/tests)
add_subdirectory(src/map)

# Add test executable
# add_executable(test2 test.cpp)

# Link the test executable against gtest and the libraries
target_link_libraries(algorithm_lib robot_lib map_lib)
