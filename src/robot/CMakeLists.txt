# Add source files in this directory
file(GLOB ROBOT_SOURCES "*.cpp")
file(GLOB ROBOT_HEADERS "*.h")

# Create a library from the source files
add_library(robot_lib ${ROBOT_SOURCES} ${ROBOT_HEADERS})

target_link_libraries(robot_lib PUBLIC map_lib utils_lib)
# Include this directory's headers
target_include_directories(robot_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})