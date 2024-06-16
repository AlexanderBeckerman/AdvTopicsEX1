#include <gtest/gtest.h>
#include <robot.h>
#include <config.h>


class RobotTest : public ::testing::Test {
protected:
    
    ConfigInfo* cfg;
    Robot* r;
    void SetUp() override {
        cfg = new ConfigInfo("../src/tests/input.txt");
        r = new Robot(*cfg);
    }

    void TearDown() override {
        delete r;
    }
};

TEST_F(RobotTest, setValueTest) {
    cfg->setValueAt({0, 0}, 55);
}