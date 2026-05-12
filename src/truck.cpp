#include "Truck.h"

static constexpr int travelTime = 30;
static constexpr int unloadTime = 5;

// Constructor
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
void Truck::addMiningTime(int minutes) {
    miningTime_ += minutes;
}

void Truck::addTravelTime() {
    travelTime_ += travelTime;
}

void Truck::addUnloadTime() {
    unloadTime_ += unloadTime;
}

void Truck::addWaitTime(int minutes) {
    waitTime_ += minutes;
}

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