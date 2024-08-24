#include "config.h"

/*Each robot will have a map instance. To initialize his map, he will pass the
given input.txt path with a reference to himself. The map constructor will
initialize the robot data members (max steps, max battery steps etc.) by reading
the input.txt. */

// This function reads the input.txt file and initializes the map and robot data
// members.
ConfigInfo::ConfigInfo(const std::string path) {
    std::string line;
    TileLayout layout;
    std::ifstream file(path);
    if (!file) {
        LOG(ERROR) << "Error: file not found" << std::endl;
        throw std::runtime_error("Couldn't build ConfigInfo, File not found!");
    }
    try {
        parseConfigLines(file); // Parse the first 5 lines of the input file.
    } catch (const std::exception &e) {
        LOG(ERROR) << "Error parsing file: " << e.what() << std::endl;
        throw std::runtime_error(
            "Couldn't build ConfigInfo, Error parsing file");
    }
    layout.reserve(this->rows);
    for (size_t curr_row = 0; curr_row < this->rows; curr_row++) {
        std::getline(file, line);
        std::vector<Tile> row;
        row.reserve(this->cols);
        auto start = line.begin();
        for (size_t col = 0; col < this->cols; col++) {
            LayoutPoint loc{curr_row, col};
            if (start == line.end() || std::isspace(*start) || line == "") {
                row.push_back(TileFromCode(0));
                start++;
                continue;
            } else if (*start == 'W') {
                row.push_back(TileFromCode(-2));
            } else if (*start == 'D') {
                this->charging_station = loc;
                row.push_back(TileFromCode(-1));
            } else if (std::isdigit(*start)) {
                size_t dirt = *start - '0';
                this->amount_to_clean += dirt;
                row.push_back(TileFromCode(dirt));
            } else { // If it's non of the above, treat it like space.
                row.push_back(TileFromCode(0));
            }
            start++;
        }
        layout.push_back(row);
    }

    file.close();
    this->topograhpy_data = std::make_shared<TileLayout>(layout);
    this->draw(path);
}

ConfigInfo::ConfigInfo(ConfigInfo &&other) noexcept {
    this->topograhpy_data = std::move(other.topograhpy_data);
    this->charging_station = other.charging_station;
    this->max_battery_steps = other.max_battery_steps;
    this->max_steps = other.max_steps;
    this->amount_to_clean = other.amount_to_clean;
}

void ConfigInfo::parseConfigLines(std::ifstream &file_reader) {
    std::string line;
    for (int line_num = 1; line_num < 6; line_num++) {
        std::getline(file_reader, line);
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace),
                   line.end());
        std::string trimmedLine = line;
        std::istringstream iss(trimmedLine);
        std::string prefix;
        int value;
        size_t pos;

        switch (line_num) {
        case 1:
            // Ignore this line
            continue;
        case 2:
            pos = trimmedLine.find('=');
            if (pos == std::string::npos ||
                trimmedLine.substr(0, pos) != "MaxSteps") {
                throw std::runtime_error("Invalid format in line 2");
            }
            value = std::stoi(trimmedLine.substr(pos + 1));
            if (value < 0) {
                throw std::runtime_error("Invalid MaxSteps value");
            }
            this->max_steps = value;
            break;
        case 3:
            pos = trimmedLine.find('=');
            if (pos == std::string::npos ||
                trimmedLine.substr(0, pos) != "MaxBattery") {
                throw std::runtime_error("Invalid format in line 3");
            }
            value = std::stoi(trimmedLine.substr(pos + 1));
            if (value < 0) {
                throw std::runtime_error("Invalid MaxBattery value");
            }
            this->max_battery_steps = value;
            break;
        case 4:
            pos = trimmedLine.find('=');
            if (pos == std::string::npos ||
                trimmedLine.substr(0, pos) != "Rows") {
                throw std::runtime_error("Invalid format in line 4");
            }
            value = std::stoi(trimmedLine.substr(pos + 1));
            if (value < 0) {
                throw std::runtime_error("Invalid Rows value");
            }
            this->rows = value;
            break;
        case 5:
            pos = trimmedLine.find('=');
            if (pos == std::string::npos ||
                trimmedLine.substr(0, pos) != "Cols") {
                throw std::runtime_error("Invalid format in line 5");
            }
            value = std::stoi(trimmedLine.substr(pos + 1));
            if (value < 0) {
                throw std::runtime_error("Invalid Cols value");
            }
            this->cols = value;
            break;
        default:
            throw std::runtime_error("Unexpected line in the input file");
        }
    }
}

int ConfigInfo::getValueAt(LayoutPoint loc) const {
    if (!checkInRange(loc))
        return -1;
    return (*topograhpy_data)[loc.row][loc.col].getDirtLevel();
}

Tile ConfigInfo::getTileAt(LayoutPoint loc) const {
    if (!checkInRange(loc))
        return Tile(TileType::WALL);
    return (*topograhpy_data)[loc.row][loc.col];
}

void ConfigInfo::setValueAt(LayoutPoint loc, int value) {
    if (!checkInRange(loc))
        throw std::out_of_range("Index out of range");
    (*topograhpy_data)[loc.row][loc.col] = TileFromCode(value);
}

void ConfigInfo::clean(LayoutPoint p) {
    Tile &tile = (*topograhpy_data)[p.row][p.col];
    try {
        tile.Clean();
    } catch (std::underflow_error &e) {
        return;
    }

    this->amount_to_clean--;
}

std::shared_ptr<TileLayout> ConfigInfo::getLayout() { return topograhpy_data; }

std::string ConfigInfo::toString() const {
    std::string output = "";
    output += "Charging station at: " + charging_station.toString() + "\n";
    for (const auto &row : *topograhpy_data) {
        for (const auto &tile : row) {
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

void ConfigInfo::draw(std::string path) const {

    std::filesystem::path pathObj(path);
    std::string filename = pathObj.stem().string();
    std::ofstream outFile("../../../output/visualization/" + filename +
                          "_cleaned_input.txt");
    if (!outFile) {
        std::cerr << "Failed to open file for writing." << std::endl;
        return;
    }
    outFile << this->charging_station.row << " " << this->charging_station.col
            << " " << this->max_battery_steps << " " << this->max_steps << "\n";
    for (const auto &row : *this->topograhpy_data) {
        for (const auto &tile : row) {
            if (tile.getType() == TileType::WALL) {
                outFile << -2;
            } else if (tile.getType() == TileType::CHARGING_STATION) {
                outFile << -1;
            } else if (tile.getType() == TileType::FLOOR) {
                outFile << tile.getDirtLevel();
            }
            outFile << " "; // Separate each value with a space
        }
        outFile << "\n"; // Newline after each row
    }

    outFile.close();
    if (!outFile) {
        std::cerr << "Error occurred while closing the file " << std::endl;
    }
}