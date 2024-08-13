#include "main_utils.h"

int main(int argc, char **argv) {

    std::string housePath = "";
    std::string algoPath = "";
    bool summary_only = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.find("-house_path=") == 0) {
            housePath = arg.substr(12);
        } else if (arg.find("-algo_path=") == 0) {
            algoPath = arg.substr(11);
        } else if (arg.find("-summary_only") == 0) {
            summary_only = true;
        }
    }

    std::vector<SimInfo> simulators;
    std::vector<AlgoInfo> algorithms;

    if (!housePath.empty()) {
        processFilesInDirectory(housePath, simulators, algorithms);
    } else {
        processFilesInDirectory(fs::current_path(), simulators, algorithms);
    }
    if (!algoPath.empty()) {
        processFilesInDirectory(algoPath, simulators, algorithms);
    } else {
        processFilesInDirectory(fs::current_path(), simulators, algorithms);
    }

    std::vector<SummaryInfo> summary;
    summary.reserve(simulators.size() * algorithms.size());

    for (auto &algo : algorithms) {
        for (auto &sim : simulators) {
            sim.simulator.setAlgorithm(*algo.algorithm);
            sim.simulator.run();
            summary.push_back(
                {sim.house_file_name, algo.name, sim.simulator.score()});

            if (!summary_only) {
                sim.simulator.dumpStepsInfo(sim.house_output_path + "_" +
                                            algo.name + ".txt");
                auto output_path = "../../../output/" + sim.house_file_name +
                                   "_" + algo.name + ".moves.txt";
                sim.simulator.serializeAndDumpSteps(output_path);
            }
            sim.simulator.reset();
        }
    }

    generateCSV(summary, simulators, algorithms);
    closeAlgos(algorithms);
    std::cout << "done" << std::endl;
}