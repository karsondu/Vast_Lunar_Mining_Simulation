#include "Simulation.h"
#include <iostream>

int main() {
    int numTrucks = 10;
    int numStations = 2;
    /*std::cout << "Enter Number of Trucks(n): ";
    std::cin>>numTrucks;
    std::cout << "Enter Number of Stations(m): ";
    std::cin>>numStations;*/

    Simulation sim(numTrucks, numStations);
    sim.run();
    sim.printStatistics();

    return 0;
}