#include "config.h"
#include "my_simulator.h"
#include "robot.h"
#include "sensors.h"
#include "smart_algorithm.h"
#include <gtest/gtest.h>

class AlgorithmTest : public ::testing::Test {
  protected:
    MySimulator *sim;

    void SetUp() override {
        sim = new MySimulator();
        sim->readHouseFile("../../../input/test_input_a.txt");
    }

    std::shared_ptr<ConfigInfo> getConfig(MySimulator &sim) {
        return sim.config;
    }
    bool canContinue(Robot &r) { return r.canContinue(); }
    LayoutPoint getLocation(const ConcreteDirtSensor &ds) {
        return ds.location;
    }
    void TearDown() override { delete sim; }
};

TEST_F(AlgorithmTest, didntChargeFullBatteryTest) {
    Robot r = Robot(getConfig(*sim));
    SmartAlgorithm algo;
    sim->setAlgorithm(algo);
    size_t battery_before_step = r.getBatterySensor().getCapacity();

    while (canContinue(r)) {
        battery_before_step = r.getBatterySensor().getBatteryState();
        auto last_loc = r.getLocation();
        Step next_step = algo.nextStep();
        r.step(next_step);
        auto curr_loc = r.getLocation();
        ASSERT_FALSE(curr_loc.isChargingStation() && last_loc == curr_loc &&
                     battery_before_step == r.getBatterySensor().getCapacity());
    }
}

TEST_F(AlgorithmTest, didntStayOnCleanTileTest) {
    Robot r = Robot(getConfig(*sim));
    SmartAlgorithm algo;
    algo.setDirtSensor(r.getDirtSensor());
    algo.setBatteryMeter(r.getBatterySensor());
    algo.setWallsSensor(r.getWallSensor());
    int dirt_before_step = r.getDirtSensor().dirtLevel();

    while (canContinue(r)) {
        Step next_step = algo.nextStep();
        if (next_step == Step::Finish) {
            break;
        }
        if (r.getLocation().isChargingStation()) {
            r.step(next_step);
            continue;
        }
        dirt_before_step = r.getDirtSensor().dirtLevel();
        auto last_loc = r.getLocation();
        r.step(next_step);
        auto curr_loc = r.getLocation();
        ASSERT_FALSE(dirt_before_step == 0 && last_loc == curr_loc);
    }
}

TEST_F(AlgorithmTest, didntMoveIntoWallTest) {
    auto cfg = getConfig(*sim);
    Robot r = Robot(cfg);
    SmartAlgorithm algo;
    algo.setWallsSensor(r.getWallSensor());
    algo.setDirtSensor(r.getDirtSensor());
    algo.setBatteryMeter(r.getBatterySensor());
    while (canContinue(r)) {
        Step next_step = algo.nextStep();
        if (next_step == Step::Finish) {
            break;
        }
        r.step(next_step);

        auto t = r.getDirtSensor().getCurrentTile().getType();
        ASSERT_FALSE(t == TileType::WALL);
    }
}

TEST_F(AlgorithmTest, didntGetStuckTest) {
    Robot r = Robot(getConfig(*sim));
    SmartAlgorithm algo;
    algo.setWallsSensor(r.getWallSensor());
    algo.setDirtSensor(r.getDirtSensor());
    algo.setBatteryMeter(r.getBatterySensor());

    while (canContinue(r)) {
        Step next_step = algo.nextStep();
        if (next_step == Step::Finish) {
            break;
        }
        r.step(next_step);
        ASSERT_FALSE(!r.getLocation().isChargingStation() &&
                     r.getBatterySensor().getBatteryState() == 0);
    }
}

TEST_F(AlgorithmTest, testInputSuccess) {
    SmartAlgorithm algo;
    sim->setAlgorithm(algo);
    sim->run();

    ASSERT_EQ(sim->dirtLeft(), 0);
}

TEST_F(AlgorithmTest, impossibleCleanFinishedAtDock) {
    SmartAlgorithm algo;
    MySimulator sim;
    sim.readHouseFile("../../../input/test_input_b.txt");
    sim.setAlgorithm(algo);
    sim.run();

    ASSERT_FALSE(sim.dirtLeft() == 0);
    ASSERT_TRUE(sim.location().isChargingStation());
}