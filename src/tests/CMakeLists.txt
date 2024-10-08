


include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../simulator)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../algorithm)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../smarter_algorithm)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common)


add_executable(unit_tests unit_tests.cpp robot_tests.cpp algorithm_a_tests.cpp 
                sensor_tests.cpp)
add_executable(robot_tests robot_tests.cpp)
add_executable(algorithm_a_tests algorithm_a_tests.cpp)
add_executable(sensor_tests sensor_tests.cpp)


target_link_libraries(unit_tests
    A_209639780_207011180
    simulator
    common
    GTest::gtest_main
)

target_link_libraries(robot_tests
    A_209639780_207011180
    simulator
    common
    GTest::gtest_main
)

target_link_libraries(algorithm_a_tests
    A_209639780_207011180
    simulator
    common
    GTest::gtest_main
)

target_link_libraries(sensor_tests
    A_209639780_207011180
    simulator
    common
    GTest::gtest_main
)



include(GoogleTest)
gtest_discover_tests(unit_tests robot_tests sensor_tests algorithm_a_tests)