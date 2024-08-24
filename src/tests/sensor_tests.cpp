#include "../algorithm/smart_algorithm.h"
#include "../common/sensors/sensors.h"
#include "../simulator/config/config.h"
#include "../simulator/my_simulator.h"
#include "../simulator/robot.h"
#include <gtest/gtest.h>

class SensorTest : public ::testing::Test {
  protected:
    MySimulator *sim;
    void SetUp() override {
        sim = new MySimulator();
        sim->readHouseFile("../../../input/test_input_a.txt");
    }

    void TearDown() override { delete sim; }

    std::shared_ptr<ConfigInfo> getConfigInfo(MySimulator *sim) {
        return std::shared_ptr<ConfigInfo>(sim->config);
    }

    Robot &getRobot(MySimulator &sim) { return *(sim.robot); }
};

TEST_F(SensorTest, isDirtyTest) {
    std::shared_ptr<ConfigInfo> cfg = getConfigInfo(sim);
    cfg->setValueAt({1, 2}, 4);
    Robot &r = getRobot(*sim);
    const ConcreteDirtSensor &dirt_sensor = r.getDirtSensor();
    bool is_dirty = dirt_sensor.isDirty();

    ASSERT_TRUE(is_dirty);
}

TEST_F(SensorTest, dirtLevelTest) {
    auto cfg = getConfigInfo(sim);
    cfg->setValueAt({1, 2}, 4);
    Robot &r = getRobot(*sim);
    const ConcreteDirtSensor &dirt_sensor = r.getDirtSensor();

    size_t dirt_level = dirt_sensor.dirtLevel();

    ASSERT_EQ(dirt_level, 4);
}

TEST_F(SensorTest, isWallTest) {
    Robot &r = getRobot(*sim);
    const ConcreteWallSensor &wall_sensor = r.getWallSensor();
    ASSERT_TRUE(wall_sensor.isWall(Direction::North));
    ASSERT_FALSE(wall_sensor.isWall(Direction::South));
    ASSERT_FALSE(wall_sensor.isWall(Direction::West));
    ASSERT_FALSE(wall_sensor.isWall(Direction::East));
}

TEST_F(SensorTest, batteryLevelTest) {
    auto cfg = getConfigInfo(sim);
    Robot &r = getRobot(*sim);
    const ConcreteBatteryMeter &battery_sensor = r.getBatterySensor();
    ASSERT_EQ(battery_sensor.getBatteryState(), cfg->getMaxBatterySteps());
}