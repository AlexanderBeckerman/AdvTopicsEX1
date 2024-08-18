#pragma once
#include "../common/AlgorithmRegistrar.h"
#include "my_simulator.h"
#include "robot.h"
#include <dlfcn.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

struct SimInfo {
    MySimulator simulator;
    std::string house_file_name;
    std::string house_output_path;
};

struct Handle {
    void *handle; // Save the handle so we can dlclose it later.
};

struct SummaryInfo {
    std::string house_file_name;
    std::string algo_name;
    std::size_t score;
};

namespace fs = std::filesystem;

std::optional<SimInfo>
processHouses(const std::filesystem::directory_entry &entry) {

    if (entry.path().extension() != ".house") {
        return std::nullopt;
    }
    auto houseFile = entry.path();
    std::ifstream file(houseFile);
    fs::path errorFilePath =
        fs::current_path() / (houseFile.stem().string() + ".error");

    if (!file.is_open()) {
        std::ofstream errorFile(errorFilePath);
        errorFile << "Error: Could not open house file: " << houseFile
                  << std::endl;
    }
    try {
        SimInfo simInfo;
        simInfo.simulator.readHouseFile(houseFile.string());
        simInfo.house_file_name = houseFile.stem().string();
        std::string outputFile = addPrefixToFileName(houseFile.stem().string());
        simInfo.house_output_path = "../../../output/" + outputFile;
        return simInfo;

    } catch (const std::exception &e) {
        std::ofstream errorFile(errorFilePath);
        errorFile << "Error: " << e.what() << std::endl;
        return std::nullopt;
    }
}

std::optional<Handle>
LoadAlgorithm(const std::filesystem::directory_entry &entry) {
    if (entry.path().extension() != ".so") {
        return std::nullopt;
    }

    std::size_t curr_count =
        AlgorithmRegistrar::getAlgorithmRegistrar().count();
    auto algoFile = entry.path();
    void *handle = dlopen(algoFile.c_str(), RTLD_LAZY);
    if ((curr_count !=
         AlgorithmRegistrar::getAlgorithmRegistrar().count() - 1) ||
        !handle) {
        fs::path errorFilePath =
            fs::current_path() / (algoFile.stem().string() + ".error");
        std::ofstream errorFile(errorFilePath);
        errorFile << "Error: Could not load algorithm file: " << algoFile
                  << "\n"
                  << dlerror() << std::endl;
        return std::nullopt;
    }

    return Handle{handle};
}

void generateCSV(const std::vector<SummaryInfo> &summaries) {
    // Collect unique house names
    std::set<std::string> houseNames;
    for (const auto &summary : summaries) {
        houseNames.insert(summary.house_file_name);
    }

    // Organize summaries by algorithm name
    std::unordered_map<std::string,
                       std::unordered_map<std::string, std::size_t>>
        organizedData;
    for (const auto &summary : summaries) {
        organizedData[summary.algo_name][summary.house_file_name] =
            summary.score;
    }

    // Write to summary.csv
    std::ofstream summaryFile("../../../output/summary.csv");

    // Write the header row
    summaryFile << "Algorithm";
    for (const auto &house : houseNames) {
        summaryFile << "," << house;
    }
    summaryFile << "\n";

    // Write each algorithm's scores
    for (const auto &[algoName, houseScores] : organizedData) {
        summaryFile << algoName;
        for (const auto &house : houseNames) {
            auto it = houseScores.find(house);
            if (it != houseScores.end()) {
                summaryFile << "," << it->second;
            } else {
                summaryFile << ",N/A"; // Should never happen.
            }
        }
        summaryFile << "\n";
    }

    summaryFile.close();
}

void closeAlgos(std::vector<Handle> &algorithms) {
    AlgorithmRegistrar::getAlgorithmRegistrar().clear();
    for (auto &algo : algorithms) {
        dlclose(algo.handle);
    }
}

template <typename Iterator, typename Func>
auto createVectorFromIterator(Iterator begin, Iterator end, Func mapFunc) {
    using ValueType = typename std::invoke_result_t<
        Func, typename std::iterator_traits<Iterator>::value_type>::value_type;
    std::vector<ValueType> result;

    for (Iterator it = begin; it != end; ++it) {
        auto value = mapFunc(*it);
        if (value) {
            result.push_back(std::move(*value));
        }
    }
    return result;
}

std::string generateOutputPath(const std::string &house_file_name,
                               const std::string &algo_name, bool moves) {
    return moves ? "../../../output/" + house_file_name + "_" + algo_name +
                       "_moves.txt"
                 : "../../../output/output_" + house_file_name + "_" +
                       algo_name + ".txt";
}