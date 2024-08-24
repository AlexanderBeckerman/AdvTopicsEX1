#pragma once
#include "../common/abstract_algorithm.h"
#include "../common/utils/utils.h"
#include "config/config.h"
#include "robot.h"
#include <filesystem>

class MySimulator {
  private:
    std::shared_ptr<ConfigInfo> config;
    AbstractAlgorithm *algorithm;
    size_t init_dirt;
    size_t max_steps;

  protected:
    std::unique_ptr<Robot> robot;

  public:
    MySimulator(const MySimulator &other) {
        this->config = std::make_shared<ConfigInfo>(*other.config);
        this->robot = std::make_unique<Robot>(this->config);
        this->algorithm = nullptr;
        this->init_dirt = other.init_dirt;
        this->max_steps = other.max_steps;
    };

    MySimulator() : config(nullptr), algorithm(nullptr) {}
    void readHouseFile(std::string input_path) {
        config = std::make_unique<ConfigInfo>(input_path);
        robot = std::make_unique<Robot>(config);
        init_dirt = config->getAmountToClean();
        max_steps = config->getMaxSteps();
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
        std::filesystem::path filePath(output_file);
        // Safety check to make sure the output folder exists, if not write to
        // current directory.
        if (!std::filesystem::exists(filePath.parent_path())) {
            filePath = filePath.filename();
        }
        robot->dumpStepsInfo(filePath.string());
    }
    void serializeAndDumpSteps(const std::string &output_file,
                               const size_t score) const {
        std::filesystem::path filePath(output_file);
        // Safety check to make sure the output folder exists, if not write to
        // current directory.
        if (!std::filesystem::exists(filePath.parent_path())) {
            filePath = filePath.filename();
        }
        robot->serializeAndDumpSteps(filePath.string(), score);
    }

    size_t score() const { return robot->getScore(); }
    size_t dirtLeft() const { return config->getAmountToClean(); }
    RelativePoint location() const { return robot->getLocation(); }
    size_t getMaxSteps() const { return max_steps; }
    size_t getInitDirt() const { return init_dirt; }

    friend class RobotTest;
    friend class SensorTest;
    friend class AlgorithmTest;
};