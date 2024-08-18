#include "main_utils.h"

int main(int argc, char **argv) {

    std::string houseArg = "";
    std::string algoArg = "";
    fs::path housePath;
    fs::path algoPath;
    bool summary_only = false;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.find("-house_path=") == 0) {
            houseArg = arg.substr(12);
        } else if (arg.find("-algo_path=") == 0) {
            algoArg = arg.substr(11);
        } else if (arg.find("-summary_only") == 0) {
            summary_only = true;
        }
    }

    if (houseArg.empty()) {
        housePath = fs::current_path();
    } else {
        housePath = houseArg;
    }
    if (algoArg.empty()) {
        algoPath = fs::current_path();
    } else {
        algoPath = algoArg;
    }

    std::vector<AlgoInfo> algorithms =
        createVectorFromIterator(fs::directory_iterator(algoPath),
                                 fs::directory_iterator(), processAlgorithms);
    std::vector<SimInfo> simulators =
        createVectorFromIterator(fs::directory_iterator(housePath),
                                 fs::directory_iterator(), processHouses);

    std::vector<SummaryInfo> summary;
    summary.reserve(simulators.size() * algorithms.size());

    for (auto &sim : simulators) {
        for (const auto &algo : AlgorithmRegistrar::getAlgorithmRegistrar()) {
            auto algorithm = algo.create();
            std::cout << algorithm << std::endl;
            sim.simulator.setAlgorithm(*algorithm);
            sim.simulator.run();
            summary.push_back(
                {sim.house_file_name, algo.getName(), sim.simulator.score()});

            if (!summary_only) {
                sim.simulator.dumpStepsInfo(generateOutputPath(
                    sim.house_file_name, algo.getName(), false));
                sim.simulator.serializeAndDumpSteps(generateOutputPath(
                    sim.house_file_name, algo.getName(), true));
            }
            sim.simulator.reset();
        }
    }

    generateCSV(summary, simulators, algorithms);
    closeAlgos(algorithms);
    std::cout << "done" << std::endl;
}