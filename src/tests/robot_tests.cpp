#include <gtest/gtest.h>
#include <robot.h>

class RobotTest : public ::testing::Test {
protected:
    
    Robot* r;
    void SetUp() override {
        r = new Robot("../src/tests/input.txt");
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