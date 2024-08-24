#include "robot.h"
#include "config/config.h"
#include <algorithm>
#include <cmath>
#include <iostream>

void Robot::move(const Direction direction) {
    // This function will move the robot in the given direction
    auto curr_location = this->location;
    if (curr_location.isChargingStation())
        this->battery_sensor.stopCharging();
    this->location = curr_location + direction;
    this->wall_sensor.step(direction);
    this->dirt_sensor.step(direction);
    this->battery_sensor.decreaseCharge();
    if (!isValidMove()) { // Check if algorithm tried to move into
                          // a wall.
        throw std::runtime_error("Algorithm tried to move into a wall.");
    }
}

void Robot::step(const Step next_step) {
    if (next_step == Step::Stay) {
        if (this->location.isChargingStation())
            this->battery_sensor.chargeBattery();
        else {
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
    t.Clean();
    this->config->setAmountToClean(this->config->getAmountToClean() - 1);
    this->battery_sensor.decreaseCharge();
}

void Robot::start(AbstractAlgorithm &algorithm) {
    // This function will start the robot and make it clean the map
    while (this->curr_steps <= this->config->getMaxSteps()) {
        Step next_step = algorithm.nextStep();
        if (next_step == Step::Finish) {
            logStep(next_step);
            finished = true;
            this->exit_cond =
                (this->location.isChargingStation())
                    ? 0
                    : 1; // 0 for finished, 1 for dead, 2 for working.
            calcScore();
            return;
        }

        if (this->getBatterySensor().getBatteryState() <= 0 &&
            !this->location
                 .isChargingStation()) { // Tried moving with no battery.
            this->exit_cond = 1;
            calcScore();
            return;
        }
        this->step(next_step);
    }
    this->exit_cond = 2; // If we reached here it means we reached max steps and
                         // algorithm didnt return finish.
    calcScore();
}

bool Robot::canContinue() {
    bool cleaned_all = this->location.isChargingStation() &&
                       this->config->getAmountToClean() == 0;
    bool stuck = !this->location.isChargingStation() &&
                 this->battery_sensor.getBatteryState() <= 0;
    bool still_have_steps = this->curr_steps < this->config->getMaxSteps();
    if (cleaned_all) {
        this->exit_cond = 0;
    } else if (stuck) {
        this->exit_cond = 1;
    } else if (!still_have_steps) {
        this->exit_cond = 2;
    }
    return still_have_steps && !cleaned_all && !stuck;
}

void Robot::calcScore() {

    size_t dirt_left = this->config->getAmountToClean();
    if (this->exit_cond == 1) { // If we are dead.
        this->score = this->config->getMaxSteps() + (dirt_left * 300) + 2000;
    } else if (finished &&
               !this->location
                    .isChargingStation()) { // If reported finished and robot is
                                            // NOT in dock.
        this->score = this->config->getMaxSteps() + (dirt_left * 300) + 3000;
    } else { // Otherwise
        this->score = this->curr_steps + (dirt_left * 300) +
                      (this->location.isChargingStation() ? 0 : 1000);
    }
}

void Robot::logStep(const Step step) {
    auto point = this->wall_sensor.location;
    StepInfo s = {point, this->battery_sensor.getBatteryState(), step,
                  this->config->getMaxSteps() - this->curr_steps};
    this->steps_info.push_back(s);
}

void Robot::dumpStepsInfo(const std::string &output_file) const {
    std::ofstream output;
    output.open(output_file);
    std::string in_dock =
        (this->location.isChargingStation()) ? "TRUE" : "FALSE";
    size_t dirt_left = this->config->getAmountToClean();

    output << "NumSteps = " << this->curr_steps << "\n";
    output << "DirtLeft = " << dirt_left << "\n";
    output << "Status = " << getStatus(this->exit_cond) << "\n";
    output << "InDock = " << in_dock << "\n";
    output << "Score = " << this->score << "\n";
    output << "Steps: \n";

    for (auto &step : this->steps_info) {
        output << step.toOutputStep();
    }
    output.close();
}

void Robot::serializeAndDumpSteps(const std::string &output_file,
                                  const size_t score) const {
    std::ofstream output;
    output.open(output_file);
    output << score << "\n";
    output << serializeVecSteps(this->steps_info);
    output.close();
}

bool Robot::isValidMove() const {

    auto tile = this->dirt_sensor.getCurrentTile();
    if (tile.getType() == TileType::WALL) {
        return false;
    }

    return true;
}

Robot::~Robot() {}