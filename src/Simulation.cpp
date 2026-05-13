#include "Simulation.h"

#include <iostream>
#include <random>
#include <ctime>

// Constructor
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

    
    // initialize starting events
    initializeEvents();

    // Main event loop
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
    int truckId = event.getTruckId();
    EventType ET = event.getType();
    int eventTime = event.getTime();
    int stationId = event.getStationId();

    switch(ET) {
        case EventType::MiningComplete: {
            trucks_[truckId].addTravelTime();
            Event nextEvent = Event(eventTime + 30, EventType::ArriveAtStation, truckId, -1);
            eventQueue_.push(nextEvent);
            break;
        }

        case EventType::ArriveAtStation: {
            int bestStation = chooseBestStation(eventTime);
            int wait = stations_[bestStation].estimateWaitTime(eventTime);
            trucks_[truckId].addWaitTime(wait);
            if (!stations_[bestStation].isBusy(eventTime)) {
                stations_[bestStation].startUnload(truckId, eventTime);
                trucks_[truckId].addUnloadTime();
                Event nextEvent = Event(eventTime + 5, EventType::UnloadComplete, truckId, bestStation);
                eventQueue_.push(nextEvent);
            } else {
                stations_[bestStation].addTruck(truckId, eventTime);
            }
            break;
        }

        case EventType::UnloadComplete: {
            stations_[stationId].finishUnload();
            trucks_[truckId].addCycle();
            if (stations_[stationId].getQueueLength() > 0) {
                int nextTruck = stations_[stationId].popNextTruck();
                stations_[stationId].startUnload(nextTruck, eventTime);
                trucks_[nextTruck].addUnloadTime();
                Event nextEvent = Event(eventTime + 5, EventType::UnloadComplete, nextTruck, stationId);
                eventQueue_.push(nextEvent);
            }
            trucks_[truckId].addTravelTime();
            int randomMiningTime = generateMiningTime();
            trucks_[truckId].addMiningTime(randomMiningTime);
            Event nextEvent = Event(eventTime + 30 + randomMiningTime, EventType::MiningComplete, truckId, -1);
            eventQueue_.push(nextEvent);
            break;
        }

        default: {
            std::cout << "Invalid Event Type Detected";
            break;
        }
    }
}


// Station selection
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


// Generates a random miningTime from 60-300 minutes;
int Simulation::generateMiningTime() {
    return (rand() % 241) +60;
}


// Final statistics
void Simulation::printStatistics() const {

    for (const Truck& curTruck: trucks_) {
        std::cout<< "Truck ID: " << curTruck.getId() << std::endl;
        std::cout<< "Total Mining Time: " << curTruck.getMiningTime() << " Minutes" << std::endl;
        std::cout<< "Total Travel Time: " << curTruck.getTravelTime() << " Minutes" << std::endl;
        std::cout<< "Total Wait Time: " << curTruck.getWaitTime() << " Minutes" << std::endl;
        std::cout<< "Total Unload Time: " << curTruck.getUnloadTime() << " Minutes" << std::endl;
        std::cout<< "Total Cycles Completed: " << curTruck.getCycles() << " Cycles" << std::endl;
    }

    
    for (const UnloadStation& curStation: stations_) {
        std::cout<< "Station ID: " << curStation.getId() << std::endl;
        std::cout<< "Total Trucks Served: " << curStation.getTotalTrucksServed() << " Trucks" << std::endl;
        std::cout<< "Total Minutes Busy: " << curStation.getTotalBusyTime() << " Minutes" << std::endl;
    }
    
}