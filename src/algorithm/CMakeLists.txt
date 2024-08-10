cmake_minimum_required(VERSION 3.25)

# Set the project name
project(algorithm)


# Add the common library as a dependency
add_subdirectory(../common common_build)

# Collect all source files for the algorithm shared library
set(ALGORITHM_SOURCES
    expanding_map.cpp
    smart_algorithm.cpp
)

# Collect all header files for the algorithm (optional, useful for IDEs)
set(ALGORITHM_HEADERS
    AlgorithmRegistration.h
    expanding_map.h
    smart_algorithm.h
)

# Create the shared library from the source files
add_library(algorithm SHARED ${ALGORITHM_SOURCES} ${ALGORITHM_HEADERS})

# Include directories (if you have any dependencies or include paths needed)
target_include_directories(algorithm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/../common)

# Optionally link against other libraries if needed
# target_link_libraries(algorithm PRIVATE some_other_library)

# Link the common library to the algorithm library
target_link_libraries(algorithm PRIVATE common)
