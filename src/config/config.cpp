#include "config.h"
#include "robot.h"

/*Each robot will have a map instance. To initialize his map, he will pass the given input.txt path with a reference to himself.
The map constructor will initialize the robot data members (max steps, max battery steps etc.) by reading the input.txt. */

// This function reads the input.txt file and initializes the map and robot data members.
ConfigInfo::ConfigInfo(const std::string path, const std::string output_path)
{
    std::string line;
    std::shared_ptr<TileLayout> layout(new TileLayout());
    this->output_path = output_path;
    std::ifstream file(path);
    if (!file)
    {
        std::cerr << "Error: file not found" << std::endl;
        throw std::runtime_error("Couldn't build ConfigInfo, File not found!");
    }

    int curr_row = 0;
    while (getline(file, line))
    {
        std::istringstream file_reader(line);

        if (curr_row == 0)
        { // Read max_battery_steps and max_steps from first line.
            file_reader >> this->max_battery_steps;
            file_reader >> this->max_steps;
            curr_row++;
            continue;
        }
        std::vector<Tile> row;
        int tile_code;
        int col = 0;
        while (file_reader >> tile_code)
        {
            LayoutPoint loc{col, curr_row - 1};
            if (tile_code == -1)
            {
                charging_station = loc;
            }
            col++;
            row.push_back(TileFromCode(loc, tile_code));
            if (tile_code >= 0)
            {
                amount_to_clean += tile_code;
            }
        }
        layout->push_back(row);
        curr_row++;
    }

    file.close();
    this->topograhpy_data = layout;
}

int ConfigInfo::getValueAt(LayoutPoint loc) const
{
    if (!checkInRange(loc))
        return -1;
    return (*topograhpy_data)[loc.y][loc.x].getDirtLevel();
}
void ConfigInfo::setValueAt(LayoutPoint loc, int value)
{
    if (!checkInRange(loc))
        return;
    (*topograhpy_data)[loc.y][loc.x] = TileFromCode(loc, value);
}

void ConfigInfo::clean(LayoutPoint p)
{   
    std::cout << "Cleaning tile at" << p.x << ","  << p.y << std::endl;
    int value = (*topograhpy_data)[p.y][p.x].getDirtLevel();
    if (value <= 0)
    {
        std::cout << "Tile is already clean" << std::endl;
        return;
    }
    setValueAt(p, value - 1);
    this->amount_to_clean--;
}

std::shared_ptr<TileLayout> ConfigInfo::getLayout() const
{
    return topograhpy_data;
}

void ConfigInfo::print() const
{
    std::cout << "Charging station location: " << charging_station.y << "," << charging_station.x << std::endl;
    for (const auto &row : *topograhpy_data)
    {
        for (Tile tile : row)
        {
            tile.print();
        }
        std::cout << std::endl;
    }
    std::cout << "-----------" << std::endl;
}

bool ConfigInfo::checkInRange(LayoutPoint p) const
{
    if (p.y < 0 || p.y >= static_cast<int>(topograhpy_data->size()) || p.x < 0 || p.x >= static_cast<int>((*topograhpy_data)[p.y].size()))
    {
        std::cout << "Index out of range" << std::endl;
        return false;
    }
    return true;
}
