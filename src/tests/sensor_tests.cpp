#include "config.h"
#include "robot.h"
#include "sensors.h"
#include <gtest/gtest.h>

class SensorTest : public ::testing::Test {
  protected:
    ConfigInfo *cfg;
    void SetUp() override {
        cfg = new ConfigInfo("../../../input/input_a.txt");
    }

    void TearDown() override { delete cfg; }
};

TEST_F(SensorTest, isDirtyTest) {
    cfg->setValueAt({0, 1}, 4);
    Robot r = Robot(*cfg);
    const ConcreteDirtSensor &dirt_sensor = r.getDirtSensor();

    bool is_dirty = dirt_sensor.isDirty();

    ASSERT_TRUE(is_dirty);
}

TEST_F(SensorTest, dirtLevelTest) {
    cfg->setValueAt({0, 1}, 4);
    Robot r = Robot(*cfg);
    const ConcreteDirtSensor &dirt_sensor = r.getDirtSensor();

    size_t dirt_level = dirt_sensor.dirtLevel();

    ASSERT_EQ(dirt_level, 4);
}

TEST_F(SensorTest, isWallTest) {
    Robot r = Robot(*cfg);
    const ConcreteWallSensor &wall_sensor = r.getWallSensor();
    ASSERT_TRUE(wall_sensor.isWall(Direction::UP));
    ASSERT_TRUE(wall_sensor.isWall(Direction::DOWN));
    ASSERT_TRUE(wall_sensor.isWall(Direction::LEFT));
    ASSERT_FALSE(wall_sensor.isWall(Direction::RIGHT));
}

TEST_F(SensorTest, batteryLevelTest) {
    Robot r = Robot(*cfg);
    const ConcreteBatteryMeter &battery_sensor = r.getBatterySensor();
    ASSERT_EQ(battery_sensor.getBatteryState(), cfg->getMaxBatterySteps());
}