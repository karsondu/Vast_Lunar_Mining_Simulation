#include "Simulation.h"
#include <iostream>


int main() {
    int numTrucks;
    int numStations;
    unsigned int seed;
    std::string seedInput;

    std::cout << "Enter Number of Trucks(n): ";
    std::cin>>numTrucks;
    std::cout << "Enter Number of Stations(m): ";
    std::cin>>numStations;

    std::cin.ignore(); // clear buffer
    std::cout << "Enter seed or press Enter for random: ";
    std::getline(std::cin, seedInput);

    if (seedInput.empty()) {
        seed = time(0);
        std::cout << "Using random seed: " << seed << "\n";
    } else {
        seed = std::stoi(seedInput);
    }


    Simulation sim(numTrucks, numStations,seed);
    sim.run();
    sim.printStatistics();

    return 0;
}