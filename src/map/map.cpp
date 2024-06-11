#include "map.h"
#include "robot.h"

/*Each robot will have a map instance. To initialize his map, he will pass the given input.txt path with a reference to himself. 
The map constructor will initialize the robot data members (max steps, max battery steps etc.) by reading the input.txt. */  

Map::Map(const std::string path, Robot& robot){
    data = getMapFromFile(path, robot);
}

void Map::setValueAt(Position p, int value){
    if (!checkInRange(p)) return;
    data[p.y][p.x] = value;
}

void Map::clean(Position p){
    int value = data[p.y][p.x];
    if (value <= 0) return;
    setValueAt(p, value - 1);
}

 std::vector<std::vector<int>>& Map::getData() {
    return data;
}

void Map::print() const {
    std::cout << "Charging station location: " << charging_station.y << "," << charging_station.x << std::endl;
    for (const auto& row : data) {
        for (int num : row) {
            if (num == -1){
                std::cout << "C" << " ";
                continue;
            }
            else if (num == -2){
                std::cout << "X" << " ";
                continue;
            }
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
}

bool Map::checkInRange(Position p) const {
    if (p.y < 0 || p.y >= data.size() || p.x < 0 || p.x >= data[p.y].size()) {
        std::cerr << "Index out of range" << std::endl;
        return false;
    }
    return true;
}
// This function reads the input.txt file and initializes the map and robot data members.
std::vector<std::vector<int>> Map::getMapFromFile(const std::string path, Robot& robot){
    std::ifstream file(path);
    std::string line;
    int counter = 0;
    std::vector<std::vector<int>> data;
    size_t max_battery_steps;
    size_t max_steps;

    if (!file){
        std::cerr << "Error: file not found" << std::endl;
        return data;
    }
    while (getline(file, line)){
        std::istringstream iss(line);
        
        if (counter == 0){ // Read max_battery_steps and max_steps from first line.
            iss >> max_battery_steps;
            iss >> max_steps;
            counter++;
            continue;
        }
        std::vector<int> row;
        int number;
        int col = 0;
        while (iss >> number){
            if (number == -1){
                charging_station = Position{counter - 1, col};
            }
            col++;
            row.push_back(number);
        }
        data.push_back(row);
        counter++;
    }

    file.close();
    robot.setMaxBatterySteps(max_battery_steps);
    robot.setMaxSteps(max_steps);
    return data;

}