# Add source files in this directory
file(GLOB MAP_SOURCES "*.cpp")
file(GLOB MAP_HEADERS "*.h")

# Create a library from the source files
add_library(map_lib ${MAP_SOURCES} ${MAP_HEADERS})

# Include this directory's headers
target_include_directories(map_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(map_lib PUBLIC robot_lib utils_lib)
 