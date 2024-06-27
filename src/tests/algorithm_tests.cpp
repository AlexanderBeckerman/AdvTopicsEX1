#include "algorithm.h"
#include "sensors.h"
#include "config.h"
#include "robot.h"
#include <gtest/gtest.h>

class AlgorithmTest : public ::testing::Test {
protected:
    
    ConfigInfo* cfg;

    void SetUp() override {
        cfg = new ConfigInfo("../../../src/tests/input.txt");

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
    bool charged_with_full_battery = false;
    int battery_before_step = r.getBatterySensor().getCapacity();

    while (canContinue(r)){
        battery_before_step = r.getBatterySensor().batteryLevel();
        Location last_loc = r.getLocation();
        r.step();
        Location curr_loc = r.getLocation();
        if (curr_loc.isChargingStation() && last_loc == curr_loc && battery_before_step == r.getBatterySensor().getCapacity()){
            charged_with_full_battery = true;
            break;
        }
    }
    ASSERT_FALSE(charged_with_full_battery);
}

TEST_F(AlgorithmTest, didntStayOnCleanTileTest) {
    Robot r = Robot(*cfg);
    bool stayed_on_clean_tile = false;
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
        if (dirt_before_step == 0 && last_loc == curr_loc){
            stayed_on_clean_tile = true;
            break;
        }
    }
    ASSERT_FALSE(stayed_on_clean_tile);
}

TEST_F(AlgorithmTest, didntMoveIntoWallTest){
    Robot r = Robot(*cfg);
    Algorithm a = getAlgorithm(r);
    bool moved_into_wall = false;
    WallSensor& wall_sensor = getWallSensor(r);

    while (canContinue(r)){
        r.step();
        if (wall_sensor.isWall(Direction::STAY)){
            moved_into_wall = true;
            break;
        }
    }
    ASSERT_FALSE(moved_into_wall);
}

TEST_F(AlgorithmTest, didntGetStuckTest){
    Robot r = Robot(*cfg);
    bool got_stuck = false;

    while (canContinue(r)){
        r.step();
        if (!r.getLocation().isChargingStation() && r.getBatterySensor().batteryLevel() == 0){
            got_stuck = true;
            break;
        }
    }
    ASSERT_FALSE(got_stuck);
}