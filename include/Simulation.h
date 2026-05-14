#pragma once
#include <vector>
#include <queue>
#include <ctime>

#include "Truck.h"
#include "UnloadStation.h"
#include "Event.h"


class Simulation {
    public:
        //Constructor that takes in n for number of trucks and m for number of stations
        //Also Takes in an unsigned int to be used for seeding the random mining times, it has a default random number based on time
        Simulation(int n, int m, unsigned int seed=time(0));

        //Function call to actually run the simulation
        void run();

        //After running the simulation, this function call is used to print out the statistics of each truck and station.
        void printStatistics() const;
    
    private:
        //Current time that the simulation is at, used to advance the simulation time until we get to the end of 72 hours
        int currentTime_; 
        
        //72 hours, converted to minutes 72*60 = 4320 minutes, used to tell when we should stop our simulation
        int simulationEndTime_; 

        //All truck objects are stored in simulation's truck vector
        std::vector<Truck> trucks_;
        //All station objects are stored in simulation's station vector
        std::vector<UnloadStation> stations_;

        //A priority queue is used to store events in an order such that closer earlier events come first
        //This allows us to process events one at a time start with earlier events and updates our current time whenever we process an event
        //We will keep getting events from this priority queue as long as the event times are before 72 hours.
        std::priority_queue<Event,
                            std::vector<Event>,
                            std::greater<Event>> eventQueue_;

        //Function to start up our simulation, it initializes all trucks and stations and pushes initial mining complete events into the event queue    .                
        void initializeEvents();

        //Function for processing our event, it takes in an event object and based on the event types, the function takes care of the related
        //trucks, stations, and scheduling of future events appropriately.
        void processEvent(const Event& event);

        //Function that when given the currentTime, will loop through all the stations and see which one has the shortest wait time
        //This function returns the Id of the station with the smallest wait time.
        int chooseBestStation(int currentTime); 
        
        //Generates a random time from 1 - 5 hours converted to minutes, so
        //60 - 300 minutes, is used specifically for mining time only.
        int generateMiningTime(); 


};