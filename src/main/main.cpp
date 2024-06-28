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
    Logger::getInstance().setLogFile("../../../output/");
    ConfigInfo config = ConfigInfo("../../../input/input_b.txt", "../../../output/output_b.txt"); // TODO(Ohad).
    Robot robot = Robot(config);
    robot.start();
}