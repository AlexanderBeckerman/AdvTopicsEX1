#include "config.h"
#include "my_simulator.h"
#include "robot.h"
#include <gtest/gtest.h>

class RobotTest : public ::testing::Test {
  protected:
    MySimulator *sim;

    void SetUp() override {
        sim = new MySimulator();
        sim->readHouseFile("../../../input/input_a.txt");
    }

    void TearDown() override { delete sim; }

    bool canContinue(Robot &r) { return r.canContinue(); }

    size_t getCurrSteps(Robot &r) { return r.curr_steps; }

    size_t getMaxSteps(Robot &r) { return r.config.getMaxSteps(); }

    size_t getAmountToClean(Robot &r) { return r.config.getAmountToClean(); }

    ConfigInfo &getConfig(MySimulator &sim) { return *sim.config; }

    SmartAlgorithm &getAlgorithm(MySimulator &sim) { return *sim.algorithm; }
};

TEST_F(RobotTest, usedBatteryTest) {
    Robot r = Robot(getConfig(*sim));
    SmartAlgorithm algo;
    sim->setAlgorithm(algo);
    int battery_before_step;
    int battery_after_step;

    while (canContinue(r)) {
        auto last_loc = r.getLocation();
        battery_before_step = r.getBatterySensor().getBatteryState();
        Step next_step = algo.nextStep();
        r.step(next_step);
        battery_after_step = r.getBatterySensor().getBatteryState();
        ASSERT_FALSE(!last_loc.isChargingStation() &&
                     battery_before_step == battery_after_step);
    }
}

TEST_F(RobotTest, didntPassMaxStepsTest) {
    Robot r = Robot(getConfig(*sim));
    SmartAlgorithm algo;
    sim->setAlgorithm(algo);
    size_t steps = 0;
    while (canContinue(r)) {
        Step next_step = algo.nextStep();
        r.step(next_step);
        steps++;
    }
    ASSERT_TRUE(steps <= getMaxSteps(r));
}

TEST_F(RobotTest, cleanedAllTest) {
    Robot r = Robot(getConfig(*sim));
    SmartAlgorithm algo;
    sim->setAlgorithm(algo);
    while (canContinue(r)) {
        Step next_step = algo.nextStep();
        r.step(next_step);
        if (getAmountToClean(r) == 0 && r.getLocation().isChargingStation()) {
            ASSERT_FALSE(canContinue(r));
        }
    }
}