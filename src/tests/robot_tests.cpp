#include <gtest/gtest.h>
#include "robot.h"
#include "config.h"


class RobotTest : public ::testing::Test {
protected:
    
    ConfigInfo* cfg;

    void SetUp() override {
        cfg = new ConfigInfo("../../../src/tests/input.txt");
    }

    void TearDown() override {
        delete cfg;
    }

    bool canContinue(Robot& r) {
        return r.canContinue();
    }
};

TEST_F(RobotTest, usedBatteryTest) {
    Robot r = Robot(*cfg);
    int battery_before_step;
    int battery_after_step;
    bool used_battery = true;

    while (canContinue(r)){
        Location last_loc = r.getLocation();
        battery_before_step = r.getBatterySensor().batteryLevel();
        r.step();
        battery_after_step = r.getBatterySensor().batteryLevel();
        if (!last_loc.isChargingStation() && battery_before_step == battery_after_step){
            used_battery = false;
            break;
        }
    }
    ASSERT_TRUE(used_battery);
}