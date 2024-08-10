#include "../common/AlgorithmRegistrar.h"
#include "my_simulator.h"
#include "robot.h"
#include <dlfcn.h>
#include <filesystem>
#include <iostream>

int main() {
    void *library_handle =
        dlopen("../../../build/src/algorithm/libalgorithm.so", RTLD_LAZY);
    if (!library_handle) {
        std::cerr << "Cannot load library: " << dlerror() << '\n';
        return 1;
    }
    std::cout << AlgorithmRegistrar::getAlgorithmRegistrar().count()
              << std::endl;
    AlgorithmRegistrar::getAlgorithmRegistrar().clear();
    std::cout << AlgorithmRegistrar::getAlgorithmRegistrar().count()
              << std::endl;
    // if (argc != 2) {
    //     std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
    //     return 1;
    // }

    // // Extract input and output file paths from command line arguments
    // std::string inputFile = argv[1];
    // std::string outputFile = addPrefixToFileName(inputFile);
    // std::string inputPath = "../../../input/" + inputFile;
    // std::string outputPath = "../../../output/" + outputFile;

    // // Run.
    // Logger::getInstance().setLogFile("../../../output/logs/");
    // MySimulator simulator = MySimulator();
    // try {
    //     simulator.readHouseFile(inputPath); // In case of invalid input file,
    //                                         // this might throw an exception.
    // } catch (const std::exception &e) {
    //     std::cerr << e.what() << '\n' << std::endl;
    //     return 1;
    // }
    // SmartAlgorithm algorithm;
    // simulator.setAlgorithm(algorithm);
    // simulator.run();

    // // Output the assignment required  info to the output file.
    // simulator.dumpStepsInfo(outputPath);

    // // Output the steps to the visualizer script.
    // simulator.serializeAndDumpSteps("../../../output/moves.txt");
}