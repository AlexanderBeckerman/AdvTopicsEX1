


include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(unit_tests unit_tests.cpp robot_tests.cpp algorithm_tests.cpp 
                sensor_tests.cpp graph_tests.cpp)
add_executable(robot_tests robot_tests.cpp)
add_executable(algorithm_tests algorithm_tests.cpp)
add_executable(sensor_tests sensor_tests.cpp)
add_executable(graph_tests graph_tests.cpp)


target_link_libraries(unit_tests
    algorithm_lib
    robot_lib
    GTest::gtest_main
)

target_link_libraries(robot_tests
    smart_map_lib
    algorithm_lib
    robot_lib
    GTest::gtest_main
)

target_link_libraries(algorithm_tests
    algorithm_lib
    robot_lib
    GTest::gtest_main
)

target_link_libraries(sensor_tests
    algorithm_lib
    robot_lib
    GTest::gtest_main
)

target_link_libraries(graph_tests
    smart_map_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests robot_tests sensor_tests expanding_map_tests algorithm_tests graph_tests)