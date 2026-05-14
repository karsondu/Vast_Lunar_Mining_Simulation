#include "Simulation.h"
#include <iostream>


int main() {
    //Instantiate variables for numTrucks(n), numStations(m), the seed for randomness, and seedInput for user input to the seed to check if we are using a user given seed or a random seed
    int numTrucks;
    int numStations;
    unsigned int seed;
    std::string seedInput;

    //Prompt the user for n and m and verify that they are both valid inputs greater than 0
    std::cout << "Enter Number of Trucks(n): ";
    std::cin>>numTrucks;
    std::cout << "Enter Number of Stations(m): ";
    std::cin>>numStations;
    if (numTrucks < 1 || numStations < 1) {
        std::cout << "Must have at least 1 truck and at least 1 station";
        return 0;
    }

    //Clear up the input buffer since previous inputs leave \n, and prompt the user for a seed and put the user's input into seedInput
    std::cin.ignore(); 
    std::cout << "Enter seed or press Enter for random: ";
    std::getline(std::cin, seedInput);

    //Check to see if a seed was given, if not then we use a random seed based on time, if so we just convert the user's input into an int that gets used as the seed
    if (seedInput.empty()) {
        seed = time(0);
        std::cout << "Using random seed: " << seed << "\n";
    } else {
        seed = std::stoi(seedInput);
    }

    //We create a Simulation object using numTrucks, numStations, and seed, then we run the sim, and afterwards can print out the statistics from it
    Simulation sim(numTrucks, numStations,seed);
    sim.run();
    sim.printStatistics();

    return 0;
}