#include "algorithm.h"

Direction Algorithm::nextMove() {

    if (notEnoughBattery()) {
        LOG(INFO) << "returning to charging station" << std::endl;
        return returnToChargingStation();
    }

    // Decide the next move.
    Direction next_direction = selectDirection(getPossibleDirections());
    RelativePoint next_location = this->robot_location + next_direction;
    if (next_location.isChargingStation()) {
        // Reset the path if we are at the charging station.
        path = std::stack<Direction>();
    } else if (next_direction != Direction::STAY) {
        // Cache the path. TODO(Sasha): Implement a pathing algorithm and
        // deperacte this.
        path.push(next_direction);
    }

    // Update the map with the current tile we discovered.
    updateMapFloor();

    // Update the current location of the robot
    this->robot_location = next_location;
    return next_direction;
}

bool Algorithm::shouldMove(const Direction d) // This checks if the robot should
                                              // move in the given direction
{

    if (this->dirt_sensor.isDirty() && d == Direction::STAY) {
        // We don't want to stay on a clean tile.
        return true;
    }
    return !this->wall_sensor.isWall(
        d); // Move if there is no wall in the given direction.
}

bool Algorithm::notEnoughBattery() {
    return (this->path.size() >= this->battery_sensor.getBatteryState() - 1) &&
           !this->robot_location.isChargingStation();
}

Direction Algorithm::returnToChargingStation() {
    // Returns the direction opposite to the one at the top of the stack so we
    // reach the charging station.
    Direction last_dir = this->path.top();
    this->path.pop();
    Direction opposite_dir = getOppositeDirection(last_dir);
    this->robot_location = this->robot_location + opposite_dir;
    return opposite_dir;
}

void Algorithm::setLocation(const Direction d) {
    this->robot_location = this->robot_location + d;
}

void Algorithm::updateMapWall(const Direction d) {
    RelativePoint p = this->robot_location + d;
    this->map.addWallTile(p);
}

void Algorithm::updateMapFloor() {
    if (this->robot_location.isChargingStation())
        return;
    this->map.addFloorTile(this->robot_location, this->dirt_sensor.dirtLevel());
}

std::vector<Direction> Algorithm::getPossibleDirections() {
    std::vector<Direction> possible_directions;

    // Clean dirty tile with probabilty of 95%
    if (this->dirt_sensor.isDirty() && trueWithProb(95)) {
        possible_directions.push_back(Direction::STAY);
        return possible_directions;
    }

    // So we won't move from charging station without enough battery to come
    // back.
    if ((this->robot_location.isChargingStation() &&
         this->battery_sensor.getBatteryState() <
             0.90 * this->battery_sensor.getCapacity())) {
        possible_directions.push_back(Direction::STAY);
        return possible_directions;
    }

    Direction directions[] = {Direction::UP, Direction::DOWN, Direction::LEFT,
                              Direction::RIGHT};
    for (auto d : directions) {
        if (this->wall_sensor.isWall(d))
            updateMapWall(d);
        else {
            possible_directions.push_back(d);
        }
    }

    return possible_directions;
}

Direction
Algorithm::selectDirection(const std::vector<Direction> &possible_directions) {
    if (possible_directions.empty()) {
        LOG(ERROR) << "No possible direction." << std::endl;
        return Direction::STAY;
    }

    // Randomly select a direction from the possible directions.
    // TODO: Implement a better selection algorithm.
    return possible_directions[rand() % possible_directions.size()];
}