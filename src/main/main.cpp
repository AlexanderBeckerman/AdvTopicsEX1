#include "algorithm.h"
#include "config.h"
#include "expanding_map.h"
#include "robot.h"
#include "sensors.h"
#include "utils.h"
#include <filesystem>
#include <iostream>

int main(int argc, char *argv[]) {
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
    Logger::getInstance().setLogFile("../../../output/logs/");
    ConfigInfo config = ConfigInfo(inputPath);
    Robot robot = Robot(config);
    robot.start();

    // Output the steps info to the output file.
    robot.dumpStepsInfo(outputPath);

    // Output the steps to the visualizer script.
    robot.serializeAndDumpSteps("../../../output/moves.txt");
}