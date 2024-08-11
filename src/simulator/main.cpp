#include "../common/AlgorithmRegistrar.h"
#include "my_simulator.h"
#include "robot.h"
#include <dlfcn.h>
#include <filesystem>
#include <iostream>

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

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    // Extract input and output file paths from command line arguments
    std::string inputFile = argv[1];
    std::string outputFile = addPrefixToFileName(inputFile);
    std::string inputPath = "../../../input/" + inputFile;
    std::string outputPath = "../../../output/" + outputFile;

    // Run.

    MySimulator simulator = MySimulator();
    try {
        simulator.readHouseFile(inputPath); // In case of invalid input file,
                                            // this might throw an exception.
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n' << std::endl;
        return 1;
    }
    for (auto algo : AlgorithmRegistrar::getAlgorithmRegistrar()) {
        Logger::getInstance().setLogFile("../../../output/logs/");
        std::cout << "Running algorithm: " << algo.getName() << std::endl;
        auto algorithm = algo.create();
        simulator.setAlgorithm(*algorithm);
        simulator.run();
        // Output the assignment required  info to the output file.
        simulator.dumpStepsInfo(outputPath);
        // Output the steps to the visualizer script.
        auto output_path = "../../../output/" + algo.getName() + "moves.txt" ;
        simulator.serializeAndDumpSteps(output_path);
        simulator.reset();
        Logger::getInstance().closeLogFile();
    }
}