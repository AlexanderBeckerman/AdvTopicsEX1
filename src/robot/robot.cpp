#include "robot.h"
#include "config.h"
#include <algorithm>
#include <cmath>
#include <iostream>

void Robot::move(const Direction direction) {
    // This function will move the robot in the given direction
    auto curr_location = this->location;
    if (curr_location.isChargingStation())
        this->battery_sensor.stopCharging();

    LOG(INFO) << "Moving: " << direction << "" << std::endl;
    LOG(INFO) << "Current location in grid: " << curr_location << ""
              << std::endl;
    this->location = curr_location + direction;
    this->wall_sensor.step(direction);
    this->dirt_sensor.step(direction);

    LOG(INFO) << "Battery level: " << this->battery_sensor.getBatteryState()
              << "" << std::endl;
    LOG(INFO) << "New location: " << this->location << "" << std::endl;
    this->battery_sensor.decreaseCharge();
}

void Robot::step(const Step next_step) {
    if (next_step == Step::Stay) {
        if (this->location.isChargingStation())
            this->battery_sensor.chargeBattery();
        else {
            LOG(INFO) << " Staying at location:" << this->location << ""
                      << std::endl;
            this->clean();
        }
    } else {
        Direction d = stepToDirection(next_step);
        this->move(d);
    }

    this->curr_steps++;
    logStep(next_step);
}

void Robot::clean() {
    Tile &t = this->dirt_sensor.getDirtyTile();
    LOG(INFO) << "Dirt level before clean: " << t.getDirtLevel() << " | ";
    LOG(INFO) << "Cleaning: " << this->location << " | ";
    t.Clean();
    this->config.setAmountToClean(this->config.getAmountToClean() - 1);
    LOG(INFO) << "Dirt level after clean: " << t.getDirtLevel() << " | ";
    // TODO(Ohad): log + output. printing for now
    LOG(INFO) << "Battery level: " << this->battery_sensor.getBatteryState()
              << "" << std::endl;
    this->battery_sensor.decreaseCharge();
}

void Robot::start(AbstractAlgorithm &algorithm) {
    // This function will start the robot and make it clean the map
    while (this->curr_steps < this->config.getMaxSteps()) {
        Step next_step = algorithm.nextStep();
        if (next_step == Step::Finish) {
            LOG(INFO) << "Algorithm returned step finished, exiting..."
                      << std::endl;
            logStep(next_step);
            this->exit_cond =
                (this->location.isChargingStation())
                    ? 0
                    : 1; // 0 for finished, 1 for dead, 2 for working.
            return;
        }
        this->step(next_step);
    }
    this->exit_cond = 2; // If we reached here it means we reached max steps and
                         // algorithm didnt return finish.
    LOG(INFO) << "Robot finished job" << std::endl;
}

bool Robot::canContinue() {
    bool cleaned_all = this->location.isChargingStation() &&
                       this->config.getAmountToClean() == 0;
    bool stuck = !this->location.isChargingStation() &&
                 this->battery_sensor.getBatteryState() <= 0;
    bool still_have_steps = this->curr_steps < this->config.getMaxSteps();
    if (cleaned_all) {
        LOG(INFO) << "Cleaned all and at charging station, exiting..."
                  << std::endl;
        this->exit_cond = 0;
    } else if (stuck) {
        LOG(INFO) << "Stuck and battery is empty, exiting..." << std::endl;
        this->exit_cond = 1;
    } else if (!still_have_steps) {
        LOG(INFO) << "Reached max steps allowed, exiting..." << std::endl;
        this->exit_cond = 2;
    }
    return still_have_steps && !cleaned_all && !stuck;
}

void Robot::logStep(const Step step) {
    auto point = this->wall_sensor.location;
    StepInfo s = {point, this->battery_sensor.getBatteryState(), step,
                  this->config.getMaxSteps() - this->curr_steps};
    this->steps_info.push_back(s);
}

void Robot::dumpStepsInfo(const std::string &output_file) const {
    std::ofstream output;
    output.open(output_file);

    output << "NumSteps = " << this->curr_steps << "\n";
    output << "DirtLeft = " << this->config.getAmountToClean() << "\n";
    output << "Status = " << getStatus(this->exit_cond) << "\n";
    output << "Steps: \n";

    for (auto &step : this->steps_info) {
        output << step.toOutputStep();
    }
    output.close();
}

void Robot::serializeAndDumpSteps(const std::string &output_file) const {
    std::ofstream output;
    output.open(output_file);
    output << serializeVecSteps(this->steps_info);
    output.close();
}
