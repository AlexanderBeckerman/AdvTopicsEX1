#include <gtest/gtest.h>
#include <robot.h>
#include <config.h>


class RobotTest : public ::testing::Test {
protected:
    
    ConfigInfo* cfg;

    void SetUp() override {
        cfg = new ConfigInfo("../src/tests/input.txt");
    }

    void TearDown() override {
        delete cfg;
    }
};

TEST_F(RobotTest, setValueTest) {
    cfg->setValueAt({0, 0}, 55);
}