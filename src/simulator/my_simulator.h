#include "../common/abstract_algorithm.h"
#include "../common/utils/utils.h"
#include "config/config.h"
#include "robot.h"

class MySimulator {
  private:
    std::unique_ptr<ConfigInfo> start_config;
    std::shared_ptr<ConfigInfo> config;
    AbstractAlgorithm *algorithm;

  protected:
    std::unique_ptr<Robot> robot;

  public:
    void readHouseFile(std::string input_path) {
        start_config = std::make_unique<ConfigInfo>(input_path);
        config = std::make_shared<ConfigInfo>(*start_config);
        robot = std::make_unique<Robot>(config);
    }
    void setAlgorithm(AbstractAlgorithm &algorithm) {
        this->algorithm = &algorithm;
        this->algorithm->setDirtSensor(robot->getDirtSensor());
        this->algorithm->setWallsSensor(robot->getWallSensor());
        this->algorithm->setBatteryMeter(robot->getBatterySensor());
        this->algorithm->setMaxSteps(config->getMaxSteps());
    }
    void run() { robot->start(*algorithm); }

    void reset() {
        config = std::make_shared<ConfigInfo>(*start_config);
        robot = std::make_unique<Robot>(config);
    }

    // Output the assignment required  info to the output file.
    void dumpStepsInfo(const std::string &output_file) const {
        robot->dumpStepsInfo(output_file);
    }
    void serializeAndDumpSteps(const std::string &output_file) const {
        robot->serializeAndDumpSteps(output_file);
    }

    size_t score() const { return robot->getScore(); }
    size_t dirtLeft() const { return config->getAmountToClean(); }
    RelativePoint location() const { return robot->getLocation(); }

    friend class RobotTest;
    friend class SensorTest;
    friend class AlgorithmTest;
};