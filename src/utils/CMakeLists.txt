# Add source files in this directory
file(GLOB UTILS_SOURCES "*.cpp")
file(GLOB UTILS_HEADERS "*.h")

# Create a library from the source files
add_library(utils_lib ${UTILS_SOURCES} ${UTILS_HEADERS})

# Include this directory's headers
target_include_directories(utils_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})