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
    std::string algo_file_path;
    void *handle; // Save the handle so we can dlclose it later.
    std::string name;
};

struct SummaryInfo {
    std::string house_file_name;
    std::string algo_name;
    std::size_t score;
};

namespace fs = std::filesystem;

void processHouseFile(const fs::path &houseFile,
                      std::vector<SimInfo> &simulators) {

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
        simulators.push_back(std::move(simInfo));

    } catch (const std::exception &e) {
        std::ofstream errorFile(errorFilePath);
        errorFile << "Error: " << e.what() << std::endl;
    }
}

void processAlgorithmFile(const fs::path &algoFile,
                          std::vector<AlgoInfo> &algorithms) {

    std::size_t curr_count =
        AlgorithmRegistrar::getAlgorithmRegistrar().count();
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
        return;
    }

    AlgoInfo algoInfo;
    algoInfo.algorithm = std::move(
        (*(AlgorithmRegistrar::getAlgorithmRegistrar().end() - 1)).create());
    algoInfo.handle = handle;
    algoInfo.algo_file_path = algoFile.string();
    algoInfo.name =
        (*(AlgorithmRegistrar::getAlgorithmRegistrar().end() - 1)).name();
    algorithms.push_back(std::move(algoInfo));
}
void processFilesInDirectory(const fs::path &dirPath,
                             std::vector<SimInfo> &simulators,
                             std::vector<AlgoInfo> &algorithms) {
    for (const auto &entry : fs::directory_iterator(dirPath)) {
        if (entry.path().extension() == ".house") {
            processHouseFile(entry.path(), simulators);
        } else if (entry.path().extension() == ".so") {
            processAlgorithmFile(entry.path(), algorithms);
        }
    }
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