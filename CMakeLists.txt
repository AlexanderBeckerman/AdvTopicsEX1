cmake_minimum_required(VERSION 3.25)
if(POLICY CMP0079 )
  cmake_policy(SET CMP0079 NEW)
endif()
cmake_policy(SET CMP0135 NEW)

project(robot)
# Set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_compile_options(-std=c++20 -Wall -Wextra -Werror -pedantic)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add subdirectories
add_subdirectory(src/algorithm)
add_subdirectory(src/smarter_algorithm)
add_subdirectory(src/simulator)
add_subdirectory(src/tests)
