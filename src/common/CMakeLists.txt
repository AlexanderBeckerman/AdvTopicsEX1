cmake_minimum_required(VERSION 3.25)

# # Add subdirectories for utils and sensors
add_subdirectory(utils)
add_subdirectory(sensors)

# Collect all source files from utils and sensors
set(COMMON_SOURCES 
    utils/utils.cpp
)

set(COMMON_HEADERS 
    sensors/sensors.h
    utils/utils.h
    utils/logger.h
    utils/tile.h
    utils/relative_point.h
    utils/layout_point.h
    abstract_algorithm.h
    AlgorithmRegistration.h
    AlgorithmRegistrar.h)

# Create a static library from all collected sources
if (NOT TARGET common)
    add_library(common STATIC ${COMMON_SOURCES} ${COMMON_HEADERS})
endif()
# Optionally include directories
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/utils)
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/sensors)