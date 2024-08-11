cmake_minimum_required(VERSION 3.25)
if(POLICY CMP0079 )
  cmake_policy(SET CMP0079 NEW)
endif()
cmake_policy(SET CMP0135 NEW)
# Set the project name
project(simulator)

# Add the common library as a dependency
add_subdirectory(../common common_build)

# Include directories for header files
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/config)

# Collect all source files for the simulator
set(SIMULATOR_SOURCES
    config/config.cpp
    robot.cpp
    AlgorithmRegistrar.cpp
    main.cpp
)

# Collect all header files for the simulator (optional, useful for IDEs)
set(SIMULATOR_HEADERS
    config/config.h
    my_simulator.h
    robot.h
    step_info.h
)

# Create the executable from the simulator source files
add_executable(simulator_main ${SIMULATOR_SOURCES} ${SIMULATOR_HEADERS})

add_library(simulator STATIC ${SIMULATOR_SOURCES} ${SIMULATOR_HEADERS})

# Link the common library
target_link_libraries(simulator PRIVATE common)


#Add any required flags for dlopen (optional, depending on platform)
target_link_libraries(simulator PRIVATE dl)

target_link_options(simulator_main PRIVATE -rdynamic)
target_link_options(simulator PRIVATE -rdynamic)
