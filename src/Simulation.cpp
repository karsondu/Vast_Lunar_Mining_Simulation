#include "Simulation.h"
#include <iomanip>
#include <iostream>
#include <random>
#include <ctime>

// Constructor 
//Initializes current time as 0 and sets up end time and uses seed for randomization
//Initializes and pushes numTrucks trucks into the simulations truck vector and numStations stations into the simulations station vector
Simulation::Simulation(int numTrucks, int numStations, unsigned int seed)
    : currentTime_(0),
    simulationEndTime_(72 * 60) {
        
        srand(seed);
        for (int i = 0; i < numTrucks; i++) {
            trucks_.push_back(Truck(i));
        }

        for (int i = 0; i < numStations; i++) {
            stations_.push_back(UnloadStation(i));
        }
    
}

// Main simulation loop
void Simulation::run() {

    
    // Initialize starting events so that the simulation can start correctly
    initializeEvents();

    // Main event loop
    //While there is still at least one event in the eventQueue, we retrieve it and pop it from the event Queue, then update our simulation's time
    //If we are past the simulation's end time then we immediately break, this allows all events that start prior to the simulations end time to be processed
    //If instead, the event happens before the simulations end time, we just process the event via the processEvent function.
    while (!eventQueue_.empty()) {

        Event nextEvent = eventQueue_.top();
        eventQueue_.pop();
        currentTime_ = nextEvent.getTime();
        if (currentTime_ > simulationEndTime_) {
            break;
        }
        processEvent(nextEvent);
    }
}

//Initial setup
//We loop through all trucks and for each, we generate a random mining time and add it to the truck's total
//Then we create MiningComplete events for each truck to signify when they finish mining, since we start at 0
//the time to complete mining is just random mining time, and the station id is -1 since this event doesn't require a station
//Finally we just push these MiningComplete events into our eventQueue for processing.
void Simulation::initializeEvents() {
    for (Truck& cur: trucks_) {
        int randomMiningTime = generateMiningTime(); //random miningTime from 60-300 minutes;
        cur.addMiningTime(randomMiningTime);
        Event nextInitialEvent = Event(randomMiningTime,EventType::MiningComplete,cur.getId(),-1);
        eventQueue_.push(nextInitialEvent);
    }
}


// Event processing
void Simulation::processEvent(const Event& event) {
    //Extract the information from the event object passed in
    int truckId = event.getTruckId();
    EventType ET = event.getType();
    int eventTime = event.getTime();
    int stationId = event.getStationId();

    //Switch case based on the event type (ET)
    switch(ET) {

        //If ET is MiningComplete, we proceed to travel to an unload station which takes 30 minutes of travel time which we add to the related truck
        //We then create an ArriveAtStation Event that occurs 30 minutes from this event time and push the newly created event into our eventQueue
        case EventType::MiningComplete: {
            trucks_[truckId].addTravelTime(std::min(30,simulationEndTime_ - eventTime));
            Event nextEvent = Event(eventTime + 30, EventType::ArriveAtStation, truckId, -1);
            eventQueue_.push(nextEvent);
            break;
        }

        //If ET is ArriveAtStation, we first find and select the bestStation to assign the truck to, we then add the stations current waitTime to the truck's total wait time.
        //If the bestStation is currently not busy at the time of the event, we can start the unloading process for the truck at bestStation meaning we add unloading time to the truck's total
        //We can then create and queue up the next event of UnloadComplete which takes time 5 minutes after the current event time
        //If instead the bestStation is busy, we can just simply add the truck to the bestStation's queue to queue it up for unloading.
        case EventType::ArriveAtStation: {
            int bestStation = chooseBestStation(eventTime);
            int wait = stations_[bestStation].estimateWaitTime(eventTime);
            trucks_[truckId].addWaitTime(wait);
            if (!stations_[bestStation].isBusy(eventTime)) {
                stations_[bestStation].startUnload(truckId, eventTime);
                int unloadDuration = std::min(5, simulationEndTime_ - eventTime);
                trucks_[truckId].addUnloadTime(unloadDuration);
                Event nextEvent = Event(eventTime + 5, EventType::UnloadComplete, truckId, bestStation);
                eventQueue_.push(nextEvent);
            } else {
                stations_[bestStation].addTruck(truckId, eventTime);
            }
            break;
        }

        //If ET is UnloadComplete, we can call the station's finishUnload function to update its total busy time and free it up for future trucks
        //We can also add 1 cycle to the truck's total cycle count since this is the last event stage of a cycle
        //We can then check if the station's queue has any trucks, while it does, we grab the nextTruck from the station's queue and pop it from the queue
        //We then start the unloading process for this nextTruck and add the unloading time to its total unload time, then we create and queue up another UnloadComplete event 5 minutes from now
        //This means that UnloadComplete events automatically schedule future UnloadComplete events for other trucks in the station's queue 
        case EventType::UnloadComplete: {
            stations_[stationId].finishUnload();
            trucks_[truckId].addCycle();
            if (stations_[stationId].getQueueLength() > 0) {
                int nextTruck = stations_[stationId].popNextTruck();
                stations_[stationId].startUnload(nextTruck, eventTime);
                trucks_[nextTruck].addUnloadTime(std::min(5,simulationEndTime_ - eventTime));
                Event nextEvent = Event(eventTime + 5, EventType::UnloadComplete, nextTruck, stationId);
                eventQueue_.push(nextEvent);
            }
            //Since our truck is going back to a mining site, we can add the 30 minutes of travel time to its total travel time
            //To emulate arriving at a mining and actively mining, we generate a random mining time and see if we even have time to return to a mining site before the simulation ends
            //if we do, then our actual mining time is clamped between the random mining time we generated, and the difference between the time we get to the mine and the amount of time left before the simulation ends 
            //Then we add the actual mining time to our truck's total,then we create and schedule a future mining complete event that happens at miningStartTime + the actualMining time which is when we finish mining. 
            //to emulate the time it takes from eventTime to travel to the mine and finish mining
            trucks_[truckId].addTravelTime(std::min(30,simulationEndTime_- eventTime));
            int randomMiningTime = generateMiningTime();
            int miningStartTime = eventTime + 30;
            if (miningStartTime + randomMiningTime <= simulationEndTime_) {
                trucks_[truckId].addMiningTime(randomMiningTime);
                Event nextEvent(miningStartTime + randomMiningTime,EventType::MiningComplete,truckId,-1);
                eventQueue_.push(nextEvent);
            }
            break;
        }

        //We have a default case here for any other unspecified input or behavior to just print out an error message and not schedule any other events
        default: {
            std::cout << "Invalid Event Type Detected";
            break;
        }
    }
}


// Station selection
//Since we know our simulation has to have at least 1 station, we initialize our shortestWaitTime to be the estimated wait time of our station with Id 0
//We also set out index to be 0, then we loop through all the stations, if any station has a shorter wait time, we update both our shortestWaitTime and shortestWaitTimeIndex
// to be the new best station, after looping through, we can just return the index of the station with the shortest wait time.
int Simulation::chooseBestStation(int currentTime) {
    int shortestWaitTime = stations_[0].estimateWaitTime(currentTime);
    int shortestWaitTimeIndex = 0;
    for (int i = 1; i < stations_.size(); i++) {
        int wait = stations_[i].estimateWaitTime(currentTime);
        if (wait < shortestWaitTime) {
            shortestWaitTime = wait;
            shortestWaitTimeIndex = i;
        }
    }
    return shortestWaitTimeIndex;
}


// Generates a random miningTime from minimum 60 to maximum 300 minutes;
int Simulation::generateMiningTime() {
    return (rand() % 241) +60;
}


// Final statistics
//We are just printing our the internal statistics for each truck and station that has been collected throughout the simulation
void Simulation::printStatistics() const {
    int totalWaitingMinutes = 0;
    for (const Truck& curTruck: trucks_) {
        totalWaitingMinutes += curTruck.getWaitTime();
        std::cout << "\n";
        std::cout << "Truck ID: " << curTruck.getId() << std::endl;
        std::cout << "Utilization: " << (curTruck.getMiningTime() + curTruck.getUnloadTime()) / 60 << " Hours and " <<  (curTruck.getMiningTime() + curTruck.getUnloadTime()) % 60 << " Minutes" << std::endl;
        std::cout << "Utilization Percentage: " << std::fixed << std::setprecision(2) << static_cast<double>(curTruck.getMiningTime() + curTruck.getUnloadTime()) / (72*60) * 100<< "%" << std::endl;
        int total_times = (curTruck.getWaitTime() + curTruck.getMiningTime() + curTruck.getTravelTime() + curTruck.getUnloadTime()) / curTruck.getCycles();
        std::cout << "Average Cycle Duration: " << total_times / 60 << " Hours and "<< total_times % 60 << " Minutes" << std::endl;
        std::cout << "Average Wait Time per Cycle: " << (curTruck.getWaitTime()) / curTruck.getCycles() / 60 << " Hours and " <<(curTruck.getWaitTime()) / curTruck.getCycles() % 60 <<" Minutes"<< std::endl;
        std::cout << "Waiting Percentage: " << (static_cast<double>(curTruck.getWaitTime()) / (72*60)) * 100<< "%" << std::endl;
        std::cout<< "Total Wait Time: " << curTruck.getWaitTime() / 60 << " Hours and " << curTruck.getWaitTime() % 60 << " Minutes" << std::endl;
        
        /*
        std::cout<< "Total Mining Time: " << curTruck.getMiningTime() << " Minutes" << std::endl;
        std::cout<< "Total Travel Time: " << curTruck.getTravelTime() << " Minutes" << std::endl;
        std::cout<< "Total Unload Time: " << curTruck.getUnloadTime() << " Minutes" << std::endl;
        std::cout<< "Total Cycles Completed: " << curTruck.getCycles() << " Cycles" << std::endl;
        */
    }

    
    for (const UnloadStation& curStation: stations_) {
        std::cout << "\n";
        std::cout<< "Station ID: " << curStation.getId() << std::endl;
        std::cout<< "Station Utilization: " << (static_cast<double>(curStation.getTotalBusyTime()) / (72*60)) * 100<< "%" << std::endl;
        std::cout<< "Throughput: " << static_cast<double>(curStation.getTotalTrucksServed()) / 72 << " Trucks Per Hour" << std::endl;
        std::cout<< "Total Trucks Served: " << curStation.getTotalTrucksServed() << " Trucks" << std::endl;
        //std::cout<< "Total Minutes Busy: " << curStation.getTotalBusyTime() << " Minutes" << std::endl;
        
    }
    std::cout << "\n";
    double avgWait = static_cast<double>(totalWaitingMinutes) / trucks_.size();
    int avg_hours = static_cast<int>(avgWait) / 60;
    int avg_mins = static_cast<int>(avgWait) % 60;
    std::cout << "Average Truck Wait Time: " << avg_hours <<" Hours and " << avg_mins << " Minutes" << std::endl;
    
}