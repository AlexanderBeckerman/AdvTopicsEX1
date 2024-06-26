#include <gtest/gtest.h>
#include "robot.h"
#include "config.h"
#include "algorithm.h"


class ExpandingMapTest : public ::testing::Test {
protected:
    
    ConfigInfo* cfg;
    Robot* robot;

    void SetUp() override {
        cfg = new ConfigInfo("../../../src/tests/input.txt");
        robot = new Robot(*cfg);
    }

    void TearDown() override {
        delete cfg;
    }

    ExpandingMap& getMap() {
        return robot->algorithm.map;
    }

};

TEST_F(ExpandingMapTest, updatedMapTest) {
    ExpandingMap& map = getMap();
    robot->step();
    auto loc = robot->getLocation();
    robot->step();
    auto tile = map.getTile(loc);
    ASSERT_TRUE(tile.has_value());
}

// TEST_F(ExpandingMapTest, cleanedTileTest) {
//     // ExpandingMap& map = getMap();
    
// }