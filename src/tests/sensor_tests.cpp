#include <gtest/gtest.h>
#include "robot.h"
#include "config.h"
#include "sensors.h"

class SensorTest : public ::testing::Test
{
protected:
    ConfigInfo *cfg;
    void SetUp() override
    {
        cfg = new ConfigInfo("../../../src/tests/input.txt");
    }

    void TearDown() override
    {
        delete cfg;
    }
};

TEST_F(SensorTest, isDirtyTest)
{
    cfg->setValueAt({1, 0}, 4);
    Robot r = Robot(*cfg);
    DirtSensor &dirt_sensor = r.getDirtSensor();

    bool is_dirty = dirt_sensor.isDirty();

    ASSERT_TRUE(is_dirty);
}

TEST_F(SensorTest, DirtLevelTest)
{
    cfg->setValueAt({1, 0}, 4);
    Robot r = Robot(*cfg);
    DirtSensor &dirt_sensor = r.getDirtSensor();

    size_t dirt_level = dirt_sensor.DirtLevel();

    ASSERT_EQ(dirt_level, 4);
}

TEST_F(SensorTest, isWallTest) {
    Robot r = Robot(*cfg);
    WallSensor& wall_sensor = r.getWallSensor();
    ASSERT_TRUE(wall_sensor.isWall(Direction::UP));
    ASSERT_TRUE(wall_sensor.isWall(Direction::DOWN));
    ASSERT_TRUE(wall_sensor.isWall(Direction::LEFT));
    ASSERT_FALSE(wall_sensor.isWall(Direction::RIGHT));
}

TEST_F(SensorTest, BatteryLevelTest) {
    Robot r = Robot(*cfg);
    BatterySensor& battery_sensor = r.getBatterySensor();
    ASSERT_EQ(battery_sensor.batteryLevel(),cfg->getMaxBatterySteps());
}