# Add source files in this directory
file(GLOB ALGORITHM_SOURCES "*.cpp")
file(GLOB ALGORITHM_HEADERS "*.h")

# Create a library from the source files
add_library(algorithm_lib SHARED ${ALGORITHM_SOURCES} ${ALGORITHM_HEADERS})

# Include this directory's headers
target_include_directories(algorithm_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(algorithm_lib PUBLIC  sensors_lib utils_lib)
