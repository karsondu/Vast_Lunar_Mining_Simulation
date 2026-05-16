#include "Truck.h"

//travelTime and unloadTime defined to avoid magic numbers
static constexpr int travelTime = 30;
static constexpr int unloadTime = 5;

// Constructor
//Assigns id to truck and initializes all total times to 0 as well as initalizing cycles to 0
Truck::Truck(int id)
    : id_(id),
      miningTime_(0),
      travelTime_(0),
      unloadTime_(0),
      waitTime_(0),
      cyclesCompleted_(0) {
}

//Truck ID
int Truck::getId() const {
    return id_;
}

//Adding Times and Cycles
//Adds the randomly generated miningTime of 1-5 hours to total mining minutes
void Truck::addMiningTime(int minutes) {
    miningTime_ += minutes;
}

//Adding Travel Time of 30 minutes or time difference
void Truck::addTravelTime(int minutes) {
    travelTime_ += minutes;
}

//Adding Unloading Time of 5 minutes or time difference
void Truck::addUnloadTime(int minutes) {
    unloadTime_ += minutes;
}

//Adds time spent waiting for a UnloadStation to open up
void Truck::addWaitTime(int minutes) {
    waitTime_ += minutes;
}

//Incrrements cyclesCompleted by 1
void Truck::addCycle() {
    cyclesCompleted_++;
}

// Getting Times and Cycles
int Truck::getMiningTime() const {
    return miningTime_;
}

int Truck::getTravelTime() const {
    return travelTime_;
}

int Truck::getUnloadTime() const {
    return unloadTime_;
}

int Truck::getWaitTime() const {
    return waitTime_;
}

int Truck::getCycles() const {
    return cyclesCompleted_;
}
