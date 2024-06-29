#include "algorithm.h"
#include "config.h"
#include "robot.h"
#include <gtest/gtest.h>

class ExpandingMapTest : public ::testing::Test {
  protected:
    ConfigInfo *cfg;

    void SetUp() override {
        cfg = new ConfigInfo("../../../input/input_a.txt");
    }

    void TearDown() override { delete cfg; }

    ExpandingMap &getMap(Robot &r) { return r.algorithm.map; }

    bool canContinue(Robot &r) { return r.canContinue(); }

    void clean(Robot &r) { r.clean(); }
};

TEST_F(ExpandingMapTest, updatedMapTest) {
    Robot r = Robot(*cfg);
    ExpandingMap &map = getMap(r);
    r.step();
    auto loc = r.getLocation();
    r.step();
    auto tile = map.getTile(loc);

    ASSERT_TRUE(tile.has_value());
}
