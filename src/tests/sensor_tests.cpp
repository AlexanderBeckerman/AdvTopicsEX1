#include <gtest/gtest.h>
#include "robot.h"
#include "config.h"
#include "sensors.h"

class SensorTest : public ::testing::Test
{
protected:
    ConfigInfo *cfg;
    Robot *r;
    void SetUp() override
    {
        cfg = new ConfigInfo("../src/tests/input.txt");
        r = new Robot(*cfg);
        r->move(Direction::LEFT); // Move to index (0,0)
    }

    void TearDown() override
    {
        delete r;
    }
};

TEST_F(SensorTest, isDirtyTest)
{
    cfg->setValueAt({0, 0}, 4);
    DirtSensor &dirt_sensor = r->getDirtSensor();
    ASSERT_TRUE(dirt_sensor.isDirty());
    cfg->setValueAt({0, 0}, 0);
    ASSERT_FALSE(dirt_sensor.isDirty());
}

TEST_F(SensorTest, DirtLevelTest)
{
    cfg->setValueAt({0, 0}, 4);
    std::cout << "value at 0,0 is:" << cfg->getValueAt({0, 0}) << std::endl;
    DirtSensor &dirt_sensor = r->getDirtSensor();
    ASSERT_EQ(dirt_sensor.DirtLevel(), 4);
    cfg->setValueAt({0, 0}, 0);
    ASSERT_EQ(dirt_sensor.DirtLevel(), 0);
}

TEST_F(SensorTest, isWallTest) {
    WallSensor& wall_sensor = r->getWallSensor();
    ASSERT_TRUE(wall_sensor.isWall(Direction::UP));
    ASSERT_TRUE(wall_sensor.isWall(Direction::DOWN));
    ASSERT_TRUE(wall_sensor.isWall(Direction::LEFT));
    ASSERT_FALSE(wall_sensor.isWall(Direction::RIGHT));
}

TEST_F(SensorTest, BatteryLevelTest) {
    BatterySensor& battery_sensor = r->getBatterySensor();
    ASSERT_EQ(battery_sensor.BatteryLevel(), 5);
}