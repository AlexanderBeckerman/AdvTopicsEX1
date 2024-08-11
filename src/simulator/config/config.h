#pragma once
#include "../../common/utils/layout_point.h"
#include "../../common/utils/tile.h"
#include "../../common/utils/utils.h"

class Robot;

typedef std::vector<std::vector<Tile>> TileLayout;

class ConfigInfo {
    std::shared_ptr<TileLayout> topograhpy_data;
    LayoutPoint charging_station;
    size_t max_battery_steps;
    size_t max_steps;
    size_t amount_to_clean = 0;
    size_t rows;
    size_t cols;

    friend Robot;

  public:
    ConfigInfo(std::string input_path);
    ConfigInfo(ConfigInfo &&other) noexcept;
    ConfigInfo(ConfigInfo &other) {
        topograhpy_data = std::make_shared<TileLayout>(*other.topograhpy_data);
        charging_station = other.charging_station;
        max_battery_steps = other.max_battery_steps;
        max_steps = other.max_steps;
        amount_to_clean = other.amount_to_clean;
        rows = other.rows;
        cols = other.cols;
    }
    std::string input_path;
    std::string output_path;
    void draw() const; // Will write a parsed version of the map to a file for
                       // the visualization.
    void setValueAt(LayoutPoint position, int value);
    int getValueAt(LayoutPoint position) const;
    Tile getTileAt(LayoutPoint position) const;
    void clean(LayoutPoint position);
    std::string toString() const;
    void setAmountToClean(size_t amount) { amount_to_clean = amount; }
    std::shared_ptr<TileLayout> getLayout();
    size_t getMaxBatterySteps() const { return max_battery_steps; }
    size_t getMaxSteps() const { return max_steps; }
    LayoutPoint getChargingStation() const { return charging_station; }
    size_t getAmountToClean() const { return amount_to_clean; }

    std::string print_debug() const {
        auto output = std::string();
        for (size_t i = 0; i < rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                output += topograhpy_data->at(i).at(j).toString();
                output += " ";
            }
            output += "\n";
        }
        return output;
    }

  private:
    bool checkInRange(LayoutPoint p) const;
    void parseConfigLines(std::ifstream &file_reader);
};

inline Tile TileFromCode(int code) {
    switch (code) {
    case -2:
        return Tile(TileType::WALL);
    case -1:
        return Tile(TileType::CHARGING_STATION);
    default:
        return Tile(code);
    }
}
