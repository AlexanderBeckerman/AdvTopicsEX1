#include "config.h"
#include "robot.h"
#include "smart_algorithm.h"
#include "utils.h"

class MySimulator {
  private:
    std::unique_ptr<ConfigInfo> config;
    std::unique_ptr<Robot> robot;
    AbstractAlgorithm *algorithm;

  public:
    void readHouseFile(std::string input_path) {
        config = std::make_unique<ConfigInfo>(input_path);
        robot = std::make_unique<Robot>(*config);
    }
    void setAlgorithm(AbstractAlgorithm &algorithm) {
        this->algorithm = &algorithm;
        this->algorithm->setDirtSensor(robot->getDirtSensor());
        this->algorithm->setWallsSensor(robot->getWallSensor());
        this->algorithm->setBatteryMeter(robot->getBatterySensor());
        this->algorithm->setMaxSteps(config->getMaxSteps());
    }
    void run() { robot->start(*algorithm); }

    // Output the assignment required  info to the output file.
    void dumpStepsInfo(const std::string &output_file) const {
        robot->dumpStepsInfo(output_file);
    }
    void serializeAndDumpSteps(const std::string &output_file) const {
        robot->serializeAndDumpSteps(output_file);
    }

    friend class RobotTest;
    friend class SensorTest;
    friend class AlgorithmTest;
};