
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

class RobotTest : public ::testing::Test {
protected:
    
    Robot* r;
    void SetUp() override {
        r = new Robot("./src/tests/input.txt");
    }

    void TearDown() override {
        delete r;
    }
};
TEST_F(RobotTest, setValueTest) {
    Map& m = r->getMap();
    m.setValueAt({0, 0}, 55);
    ASSERT_EQ(r->getMap().getData()[0][0], 55);
}
int main(int argc, char **argv) {
    Robot r("./src/tests/input.txt");
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}