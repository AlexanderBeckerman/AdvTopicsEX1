cmake_minimum_required(VERSION 3.25)

# Set the project name
project(B_209639780_207011180)


# Add the common library as a dependency
add_subdirectory(../common common_build)

# Collect all source files for the algorithm shared library
set(SDFS_SOURCES
    SDFS.cpp
)

# Collect all header files for the algorithm (optional, useful for IDEs)
set(SDFS_HEADERS
    SDFS.h
)

# Create the shared library from the source files
add_library(B_209639780_207011180 SHARED ${SDFS_SOURCES} ${SDFS_HEADERS})

# Include directories (if you have any dependencies or include paths needed)
target_include_directories(B_209639780_207011180 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/../common)

# Optionally link against other libraries if needed
# target_link_libraries(algorithm PRIVATE some_other_library)

# Link the common library to the algorithm library
target_link_libraries(B_209639780_207011180 PRIVATE common)
