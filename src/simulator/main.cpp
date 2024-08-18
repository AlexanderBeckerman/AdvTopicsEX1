#include "main_utils.h"
#include <algorithm>
#include <chrono>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

std::mutex summaryMutex; // Mutex to protect access to the summaries vector

void runSimulation(SimInfo sim, std::unique_ptr<AbstractAlgorithm> algo,
                   const std::string &name, bool summary_only,
                   std::vector<SummaryInfo> &summaries, int timeout) {
    std::cout << name << " House: " << sim.house_file_name << std::endl;
    sim.simulator.setAlgorithm(*algo);
    std::optional<std::size_t> score;

    auto future = std::async(std::launch::async, [&]() {
        sim.simulator.run();
        return sim.simulator.score();
    });

    if (future.wait_for(std::chrono::milliseconds(timeout)) ==
        std::future_status::timeout) {
        //  TODO(Sasha): add penalty here
        score = sim.simulator.getMaxSteps() * 2 +
                sim.simulator.getInitDirt() * 300 + 2000;
        std::cout << "Timeout! penalty: " << score.value() << std::endl;
        future.get(); // Retrieve the result but ignore it
    } else {
        score = future.get(); // Get the actual score if completed in time
    }

    {
        // Protect access to the summaries vector
        std::lock_guard<std::mutex> guard(summaryMutex);
        summaries.push_back({sim.house_file_name, name, score.value()});
    }

    if (!summary_only) {
        sim.simulator.dumpStepsInfo(
            generateOutputPath(sim.house_file_name, name, false));
        sim.simulator.serializeAndDumpSteps(
            generateOutputPath(sim.house_file_name, name, true), score.value());
    }
}

int main(int argc, char **argv) {
    // CLI.
    std::string houseArg = "";
    std::string algoArg = "";
    fs::path housePath;
    fs::path algoPath;
    bool summary_only = false;
    long unsigned int numThreads = 10;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.find("-house_path=") == 0) {
            houseArg = arg.substr(12);
        } else if (arg.find("-algo_path=") == 0) {
            algoArg = arg.substr(11);
        } else if (arg.find("-summary_only") == 0) {
            summary_only = true;
        } else if (arg.find("-num_threads=") == 0) {
            numThreads = std::stoul(arg.substr(13));
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

    int timeout = 1000;

    // Load.
    std::vector<Handle> algorithm_handles =
        createVectorFromIterator(fs::directory_iterator(algoPath),
                                 fs::directory_iterator(), LoadAlgorithm);
    std::vector<SimInfo> simulators =
        createVectorFromIterator(fs::directory_iterator(housePath),
                                 fs::directory_iterator(), processHouses);

    // Run.
    std::vector<std::thread> threads;
    std::vector<SummaryInfo> summaries;
    summaries.reserve(simulators.size() * algorithm_handles.size());

    for (const auto &sim : simulators) {
        for (const auto &algo : AlgorithmRegistrar::getAlgorithmRegistrar()) {
            if (threads.size() >= numThreads) {
                // Wait for some threads to finish before starting new ones
                for (auto &t : threads) {
                    t.join();
                }
                threads.clear();
            }

            // Create a new algorithm & simulator instances.
            SimInfo sim_cpy = {MySimulator(sim.simulator), sim.house_file_name,
                               sim.house_output_path};
            timeout = sim_cpy.simulator.getMaxSteps() * 1;
            auto algorithm = algo.create();
            auto &algorithm_name = algo.name();
            threads.emplace_back(runSimulation, std::move(sim_cpy),
                                 std::move(algorithm),
                                 std::cref(algorithm_name), summary_only,
                                 std::ref(summaries), timeout);
        }
    }

    // Join any remaining threads
    for (auto &t : threads) {
        std::cout << "Joining thread" << std::endl;
        t.join();
    }

    std::cout << "Done! summarizing..." << std::endl;
    generateCSV(summaries);
    closeAlgos(algorithm_handles);
}