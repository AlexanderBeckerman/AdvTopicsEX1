#include "robot.h"
#include "map.h"


void Robot::move(Direction direction) {
    // Implementation of move function
}
void Robot::setMaxBatterySteps(size_t max_battery_steps) {
    this->max_battery_steps = max_battery_steps;
}
void Robot::setMaxSteps(size_t max_steps) {
    this->max_steps = max_steps;
}
void Robot::print(){
    std::cout << "Max steps and max battery steps: " << max_steps << "," << max_battery_steps << std::endl;
    map.print();
}
void Robot::clean(Position p){
    map.clean(p);
}
Map& Robot::getMap(){
    return map;
}