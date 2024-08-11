#include "../common/AlgorithmRegistrar.h"
#include "my_simulator.h"
#include "robot.h"
#include <dlfcn.h>
#include <filesystem>
#include <fstream>
#include <iostream>

struct SimInfo {
    MySimulator simulator;
    std::string house_file_path;
    std::string house_output_path;
};

struct AlgoInfo {
    std::unique_ptr<AbstractAlgorithm> algorithm;
    std::string algo_file_path;
    void *handle; // Save the handle so we can dlclose it later.
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
        simInfo.simulator.readHouseFile(houseFile);
        simInfo.house_file_path = houseFile;
        std::string outputFile = addPrefixToFileName(houseFile);
        simInfo.house_output_path = "../../../output/" + outputFile;
        simulators.push_back(simInfo);

    } catch (const std::exception &e) {
        std::ofstream errorFile(errorFilePath);
        errorFile << "Error: " << e.what() << std::endl;
    }
}

void processAlgorithmFile(const fs::path &algoFile,
                          std::vector<AlgoInfo> &algorithms) {

    int curr_count = AlgorithmRegistrar::getAlgorithmRegistrar().count();
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
    algoInfo.algorithm =
        (*(AlgorithmRegistrar::getAlgorithmRegistrar().end() - 1)).create();
    algoInfo.handle = handle;
    algoInfo.algo_file_path = algoFile;
    algorithms.push_back(algoInfo);
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

int main(int argc, char **argv) {

    void *smart_alrogithm_lib =
        dlopen("../../../build/src/algorithm/libalgorithm.so", RTLD_LAZY);
    if (!smart_alrogithm_lib) {
        std::cerr << "Cannot load library: " << dlerror() << '\n';
        return 1;
    }
    void *sdfs_lib =
        dlopen("../../../build/src/smarter_algorithm/libSDFS.so", RTLD_LAZY);
    if (!sdfs_lib) {
        std::cerr << "Cannot load library: " << dlerror() << '\n';
        return 1;
    }

    std::string housePath = "";
    std::string algoPath = "";

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.find("-house_path=") == 0) {
            housePath = arg.substr(12);
        } else if (arg.find("-algo_path=") == 0) {
            algoPath = arg.substr(11);
        }
    }

    std::vector<SimInfo> simulators;
    std::vector<AlgoInfo> algorithms;

    // Extract input and output file paths from command line arguments
    // std::string inputFile = argv[1];
    // std::string outputFile = addPrefixToFileName(inputFile);
    // std::string inputPath = "../../../input/" + inputFile;
    // std::string outputPath = "../../../output/" + outputFile;

    // Run.


    MySimulator simulator = MySimulator();
    for (auto algo : AlgorithmRegistrar::getAlgorithmRegistrar()) {
        try {
            simulator.readHouseFile(
                inputPath); // In case of invalid input file,
                            // this might throw an exception.
        } catch (const std::exception &e) {
            std::cerr << e.what() << '\n' << std::endl;
            return 1;
        }
        Logger::getInstance().setLogFile("../../../output/logs/");
        std::cout << "Running algorithm: " << algo.getName() << std::endl;
        auto algorithm = algo.create();
        simulator.setAlgorithm(*algorithm);
        simulator.run();
        // Output the assignment required  info to the output file.
        simulator.dumpStepsInfo(outputPath);
        // Output the steps to the visualizer script.
        auto output_path = "../../../output/" + algo.getName() + "moves.txt";
        simulator.serializeAndDumpSteps(output_path);
        simulator.reset();
        Logger::getInstance().closeLogFile();
    }

}