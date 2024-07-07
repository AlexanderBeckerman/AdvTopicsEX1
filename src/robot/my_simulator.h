#include "config.h"
#include "robot.h"
#include "smart_algorithm.h"
#include "utils.h"

class MySimulator {
  private:
    std::unique_ptr<ConfigInfo> config;
    std::unique_ptr<Robot> robot;
    std::unique_ptr<SmartAlgorithm> algorithm;

  public:
    void readHouseFile(std::string input_path) {
        config = std::make_unique<ConfigInfo>(input_path);
        robot = std::make_unique<Robot>(*config);
    }
    void setAlgorithm(SmartAlgorithm &algorithm) {
        this->algorithm = std::make_unique<SmartAlgorithm>(algorithm);
        this->algorithm->setDirtSensor(robot->getDirtSensor());
        this->algorithm->setWallsSensor(robot->getWallSensor());
        this->algorithm->setBatteryMeter(robot->getBatterySensor());
        this->algorithm->setMaxSteps(config->getMaxSteps());
    }
    void run() { robot->start(*algorithm); }

    friend class RobotTest;
};