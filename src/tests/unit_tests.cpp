
#include <iostream>
#include <gtest/gtest.h>
#include "map.h"
#include "robot.h"

void printMapTest(Robot& r) {
    std::cout << "Printing map:\n" << std::endl;
    r.print();
}

void changeMapTest(Robot& r){
    std::cout << "Printing map before change:\n" << std::endl;
    r.print();
    std::cout << "Printing map after change:\n" << std::endl;
    r.getMap().setValueAt({0, 0}, 55);
    r.print();

}

int run_all_tests(Robot& r) {
    std::cout << "Running all tests" << std::endl;
    changeMapTest(r);
    return 0;
}

int main(int argc, char **argv) {
    Robot r("./src/tests/input.txt");
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}