#pragma once
#include "../common/AlgorithmRegistrar.h"
#include "my_simulator.h"
#include "robot.h"
#include <dlfcn.h>
#include <filesystem>
#include <fstream>
#include <iostream>

struct SimInfo {
    MySimulator simulator;
    std::string house_file_name;
    std::string house_output_path;
};

struct AlgoInfo {
    std::unique_ptr<AbstractAlgorithm> algorithm;
    void *handle; // Save the handle so we can dlclose it later.
    std::string name;
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

std::optional<AlgoInfo>
processAlgorithms(const std::filesystem::directory_entry &entry) {

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

    AlgoInfo algoInfo;
    algoInfo.algorithm = std::move(
        (*(AlgorithmRegistrar::getAlgorithmRegistrar().end() - 1)).create());
    algoInfo.handle = handle;
    algoInfo.name =
        (*(AlgorithmRegistrar::getAlgorithmRegistrar().end() - 1)).name();
    return algoInfo;
}

void generateCSV(const std::vector<SummaryInfo> &summary,
                 const std::vector<SimInfo> &simulators,
                 const std::vector<AlgoInfo> &algorithms) {
    std::ofstream csvFile("../../../output/summary.csv");
    csvFile << "Algorithm";
    for (const auto &sim : simulators) {
        csvFile << "," << sim.house_file_name;
    }
    csvFile << "\n";
    for (const auto &algo : algorithms) {
        csvFile << algo.name;
        for (const auto &sim : simulators) {
            auto it = std::find_if(summary.begin(), summary.end(),
                                   [&algo, &sim](const SummaryInfo &info) {
                                       return info.house_file_name ==
                                                  sim.house_file_name &&
                                              info.algo_name == algo.name;
                                   });
            if (it != summary.end()) {
                csvFile << "," << it->score;
            } else {
                csvFile << ",-1";
            }
        }
        csvFile << "\n";
    }
}

void closeAlgos(std::vector<AlgoInfo> &algorithms) {
    AlgorithmRegistrar::getAlgorithmRegistrar().clear();
    for (auto &algo : algorithms) {
        algo.algorithm.reset();
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
                       "moves.txt"
                 : "../../../output/" + house_file_name + "_" + algo_name +
                       ".txt";
}