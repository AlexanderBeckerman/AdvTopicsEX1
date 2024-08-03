file(GLOB SENSORS_SOURCES "*.cpp")
file(GLOB SENSORS_HEADERS "*.h")

add_library(sensors_lib ${SENSORS_SOURCES} ${SENSORS_HEADERS})

target_include_directories(sensors_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(sensors_lib PUBLIC  utils_lib config_lib)