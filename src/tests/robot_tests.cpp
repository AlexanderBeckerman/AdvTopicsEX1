#include <gtest/gtest.h>
#include "robot.h"
#include "config.h"


class RobotTest : public ::testing::Test {
protected:
    
    ConfigInfo* cfg;

    void SetUp() override {
        cfg = new ConfigInfo("../../../input/input_a.txt");
    }

    void TearDown() override {
        delete cfg;
    }

    bool canContinue(Robot& r) {
        return r.canContinue();
    }

    size_t getCurrSteps(Robot& r) {
        return r.curr_steps;
    }

    size_t getMaxSteps(Robot& r) {
        return r.config.getMaxSteps();
    }

    size_t getAmountToClean(Robot& r) {
        return r.config.getAmountToClean();
    }
};

TEST_F(RobotTest, usedBatteryTest) {
    Robot r = Robot(*cfg);
    int battery_before_step;
    int battery_after_step;

    while (canContinue(r)){
        Location last_loc = r.getLocation();
        battery_before_step = r.getBatterySensor().batteryLevel();
        r.step();
        battery_after_step = r.getBatterySensor().batteryLevel();
        ASSERT_FALSE(!last_loc.isChargingStation() && battery_before_step == battery_after_step);
    }
}

TEST_F(RobotTest, didntPassMaxStepsTest){
    Robot r = Robot(*cfg);
    size_t steps = 0;
    while (canContinue(r)){
        r.step();
        steps++;
    }
    ASSERT_TRUE(steps <= getMaxSteps(r));
}

TEST_F(RobotTest, cleanedAllTest){
    Robot r = Robot(*cfg);
    while (canContinue(r)){
        r.step();
        if (getAmountToClean(r) == 0 && r.getLocation().isChargingStation()){
            ASSERT_FALSE(canContinue(r));
        }
    }
}