# Add source files in this directory
file(GLOB SMARTMAP_SOURCES "*.cpp")
file(GLOB SMARTMAP_HEADERS "*.h")

# Create a library from the source files
add_library(smart_map_lib ${SMARTMAP_SOURCES} ${SMARTMAP_HEADERS})

target_link_libraries(smart_map_lib PUBLIC  algorithm_lib utils_lib)
# Include this directory's headers
target_include_directories(smart_map_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})