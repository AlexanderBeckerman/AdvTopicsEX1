#include "config.h"
#include "robot.h"

/*Each robot will have a map instance. To initialize his map, he will pass the
given input.txt path with a reference to himself. The map constructor will
initialize the robot data members (max steps, max battery steps etc.) by reading
the input.txt. */

// This function reads the input.txt file and initializes the map and robot data
// members.
ConfigInfo::ConfigInfo(const std::string path) {
    std::string line;
    TileLayout layout;
    this->output_path = output_path;
    std::ifstream file(path);
    if (!file) {
        LOG(ERROR) << "Error: file not found" << std::endl;
        throw std::runtime_error("Couldn't build ConfigInfo, File not found!");
    }

    size_t curr_row = 0;
    while (getline(file, line)) {
        std::istringstream file_reader(line);

        if (curr_row ==
            0) { // Read max_battery_steps and max_steps from first line.
            file_reader >> this->max_battery_steps;
            file_reader >> this->max_steps;
            curr_row++;
            continue;
        }
        std::vector<Tile> row;
        int tile_code;
        size_t col = 0;
        while (file_reader >> tile_code) {
            LayoutPoint loc{curr_row - 1, col};
            if (tile_code == -1) {
                charging_station = loc;
            }
            col++;
            row.push_back(TileFromCode(tile_code));
            if (tile_code >= 0) {
                amount_to_clean += tile_code;
            }
        }
        layout.push_back(row);
        curr_row++;
    }

    file.close();
    this->topograhpy_data = std::make_shared<TileLayout>(layout);
}

ConfigInfo::ConfigInfo(ConfigInfo &&other) noexcept {
    this->topograhpy_data = std::move(other.topograhpy_data);
    this->charging_station = other.charging_station;
    this->max_battery_steps = other.max_battery_steps;
    this->max_steps = other.max_steps;
    this->amount_to_clean = other.amount_to_clean;
}

int ConfigInfo::getValueAt(LayoutPoint loc) const {
    if (!checkInRange(loc))
        return -1;
    return (*topograhpy_data)[loc.row][loc.col].getDirtLevel();
}
void ConfigInfo::setValueAt(LayoutPoint loc, int value) {
    if (!checkInRange(loc))
        throw std::out_of_range("Index out of range");
    (*topograhpy_data)[loc.row][loc.col] = TileFromCode(value);
}

void ConfigInfo::clean(LayoutPoint p) {
    LOG(INFO) << "Cleaning tile at" << p.row << "," << p.col << "" << std::endl;
    Tile &tile = (*topograhpy_data)[p.row][p.col];
    try {
        tile.Clean();
    } catch (std::underflow_error &e) {
        LOG(ERROR) << "Tile at: " << p << " is already clean" << std::endl;
        return;
    }

    this->amount_to_clean--;
}

std::shared_ptr<TileLayout> ConfigInfo::getLayout() { return topograhpy_data; }

std::string ConfigInfo::toString() const {
    std::string output = "";
    output += "Charging station at: " + charging_station.toString() + "\n";
    for (const auto &row : *topograhpy_data) {
        for (Tile tile : row) {
            output += tile.toString();
        }
        output += "\n";
    }
    output += "-----------\n";
    return output;
}

bool ConfigInfo::checkInRange(LayoutPoint p) const {
    if (p.row >= topograhpy_data->size() ||
        p.col >= (*topograhpy_data)[p.row].size()) {
        LOG(ERROR) << "Index out of range" << std::endl;
        return false;
    }
    return true;
}
