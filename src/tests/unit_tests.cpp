
#include <iostream>
#include <gtest/gtest.h>
#include "config.h"
#include "robot.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}