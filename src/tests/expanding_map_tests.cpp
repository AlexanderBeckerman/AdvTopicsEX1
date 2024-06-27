#include <gtest/gtest.h>
#include "robot.h"
#include "config.h"
#include "algorithm.h"


class ExpandingMapTest : public ::testing::Test {
protected:
    
    ConfigInfo* cfg;

    void SetUp() override {
        cfg = new ConfigInfo("../../../input/input.txt");
    }

    void TearDown() override {
        delete cfg;
    }

    ExpandingMap& getMap(Robot& r) {
        return r.algorithm.map;
    }

    bool canContinue(Robot& r) {
        return r.canContinue();
    }
    
    void clean(Robot& r) {
        r.clean();
    }
};

TEST_F(ExpandingMapTest, updatedMapTest) {
    Robot r = Robot(*cfg);
    ExpandingMap& map = getMap(r);
    r.step();
    auto loc = r.getLocation();
    r.step();
    auto tile = map.getTile(loc);

    ASSERT_TRUE(tile.has_value());
}

TEST_F(ExpandingMapTest, cleanedTileTest) {
    Robot r = Robot(*cfg);
    ExpandingMap& map = getMap(r);
    int dirt_before_step = r.getDirtSensor().DirtLevel();

    while (canContinue(r)){
        if (r.getLocation().isChargingStation() || r.getDirtSensor().DirtLevel() == 0){
            r.step();
            continue;
        }
        dirt_before_step = r.getDirtSensor().DirtLevel();
        Location last_loc = r.getLocation();
        auto tileOpt = map.getTile(last_loc);
        if (dirt_before_step > 0){
            clean(r);
            r.step();
            Tile& tile = tileOpt.value();
            ASSERT_TRUE(tile.getDirtLevel() == dirt_before_step - 1);
            break;
        }
        // std::cout << "dirt sensor dirt level: " << r.getDirtSensor().DirtLevel() << std::endl;
        // if (dirt_before_step > 0 && last_loc == curr_loc){
        //     if (tileOpt.has_value()){
        //         std::cout << "dirt before step: " << dirt_before_step << std::endl;
        //         std::cout << "expanding map dirt level: " << tile.getDirtLevel() << std::endl;
        //     }
        // }
    }
    
}