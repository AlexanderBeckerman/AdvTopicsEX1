
#include <iostream>
#include <gtest/gtest.h>
#include "config.h"
#include "robot.h"

void RobotDebug(Robot& r) {
    std::cout << "Printing map:\n" << std::endl;
    r.debug();
}

int run_all_tests(Robot& r) {
    std::cout << "Running all tests" << std::endl;
    RobotDebug(r);
    return 0;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}