#include <iostream>
#include "algorithm.h"
#include "sensors.h"
#include "config.h"
#include "expanding_map.h"
#include "robot.h"
#include "utils.h"
#include <filesystem>

int main()
{
    ConfigInfo config = ConfigInfo("../../../input/input.txt"); // TODO(Ohad).
    Robot robot = Robot(config);
    robot.start();
}