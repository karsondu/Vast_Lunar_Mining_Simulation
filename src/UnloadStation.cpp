#include "UnloadStation.h"

// Constructor
UnloadStation::UnloadStation(int id)
    : id_(id),
      busyUntil_(0),
      currentTruck_(-1),
      totalTrucksServed_(0),
      totalBusyTime_(0) {
}



int UnloadStation::getId() const {
    return id_;
}

//Status of Station at currentTime
bool UnloadStation::isBusy(int currentTime) const {
    return currentTime < busyUntil_;
}

//Check how many trucks in queue
int UnloadStation::getQueueLength() const {
    return queue_.size();
}


void UnloadStation::addTruck(int truckId, int currentTime) {
    if (!isBusy(currentTime)) {
        startUnload(truckId, currentTime);
    } else {
        queue_.push(truckId);
    }
}

int UnloadStation::popNextTruck() {
    int nextTruck = queue_.front();
    queue_.pop();
    return nextTruck;
}

void UnloadStation::startUnload(int truckId, int currentTime) {
    currentTruck_ = truckId;
    busyUntil_ = currentTime + 5;
    totalTrucksServed_ += 1;
}

void UnloadStation::finishUnload() {
    totalBusyTime_ += 5;
    currentTruck_ = -1;
}

//Metrics for stats later
int UnloadStation::getTotalTrucksServed() const {
    return totalTrucksServed_;
}

int UnloadStation::getTotalBusyTime() const {
    return totalBusyTime_;
}

//Helper for load balancing trucks and queue times.
int UnloadStation::estimateWaitTime(int currentTime) const {
    int wait = 0;
    if (currentTime < busyUntil_) {
        wait += (busyUntil_ - currentTime);
        wait += (queue_.size() * 5);
    }
    return wait;
}