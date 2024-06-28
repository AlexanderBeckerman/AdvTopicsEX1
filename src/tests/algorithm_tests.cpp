#include "algorithm.h"
#include "sensors.h"
#include "config.h"
#include "robot.h"
#include <gtest/gtest.h>

class AlgorithmTest : public ::testing::Test {
protected:
    
    ConfigInfo* cfg;

    void SetUp() override {
        cfg = new ConfigInfo("../../../input/input_b.txt", "../../../output/output_b.txt");

    }
    void TearDown() override {
        delete cfg;
    }

    Algorithm& getAlgorithm(Robot& r) {
        return r.algorithm;
    }
    bool canContinue(Robot& r) {
        return r.canContinue();
    }
    WallSensor& getWallSensor(Robot& r) {
        return r.wall_sensor;
    }
};

TEST_F(AlgorithmTest, didntChargeFullBatteryTest) {
    Robot r = Robot(*cfg);
    size_t battery_before_step = r.getBatterySensor().getCapacity();

    while (canContinue(r)){
        battery_before_step = r.getBatterySensor().batteryLevel();
        Location last_loc = r.getLocation();
        r.step();
        Location curr_loc = r.getLocation();
        ASSERT_FALSE(curr_loc.isChargingStation() && last_loc == curr_loc && battery_before_step == r.getBatterySensor().getCapacity());
    }
}

TEST_F(AlgorithmTest, didntStayOnCleanTileTest) {
    Robot r = Robot(*cfg);
    int dirt_before_step = r.getDirtSensor().DirtLevel();

    while (canContinue(r)){
        if (r.getLocation().isChargingStation()){
            r.step();
            continue;
        }
        dirt_before_step = r.getDirtSensor().DirtLevel();
        Location last_loc = r.getLocation();
        r.step();
        Location curr_loc = r.getLocation();
        ASSERT_FALSE(dirt_before_step == 0 && last_loc == curr_loc);
    }
}

TEST_F(AlgorithmTest, didntMoveIntoWallTest){
    Robot r = Robot(*cfg);
    Algorithm a = getAlgorithm(r);
    WallSensor& wall_sensor = getWallSensor(r);

    while (canContinue(r)){
        r.step();
        ASSERT_FALSE(wall_sensor.isWall(Direction::STAY));       
    }
}

TEST_F(AlgorithmTest, didntGetStuckTest){
    Robot r = Robot(*cfg);

    while (canContinue(r)){
        r.step();
        ASSERT_FALSE(!r.getLocation().isChargingStation() && r.getBatterySensor().batteryLevel() == 0);
    }
}