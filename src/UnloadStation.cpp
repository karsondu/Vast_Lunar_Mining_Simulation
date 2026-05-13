#include "UnloadStation.h"

// Constructor
/*All stations get initailized with an id, busyUntil_ of 0 to show that they're currently free,
currentTruck_ of -1 to show that no truck is currently assigned to the station,
and 0 for both total trucks served and total time the station is busy
*/
UnloadStation::UnloadStation(int id)
    : id_(id),
      busyUntil_(0),
      currentTruck_(-1),
      totalTrucksServed_(0),
      totalBusyTime_(0) {
}


//Returns the station id
int UnloadStation::getId() const {
    return id_;
}

//If the currentTime is earlier than the time that the station is busy until, then we know that the station is currently busy
//Otherwise if the current time is after the time the station is busy until, then we know that the station is currently free for this current time
bool UnloadStation::isBusy(int currentTime) const {
    return currentTime < busyUntil_;
}

//Check how many trucks are currently in queue
int UnloadStation::getQueueLength() const {
    return queue_.size();
}

//Checks if the station is busy at currentTime, if it isn't then we can immediately start unloading this truck at this currentTime
//Otherwise we add the truck to this station's queue
void UnloadStation::addTruck(int truckId, int currentTime) {
    if (!isBusy(currentTime)) {
        startUnload(truckId, currentTime);
    } else {
        queue_.push(truckId);
    }
}

//Returns the id of the next truck in the station's queue
int UnloadStation::popNextTruck() {
    int nextTruck = queue_.front();
    queue_.pop();
    return nextTruck;
}

//To start the unload process, we take the truckId and set it as the station's currentTruck_
//Unloading takes 5 minutes so the station is busy until currentTime + 5 minutes
//Then we increment the station's total amount of trucks served by 1
void UnloadStation::startUnload(int truckId, int currentTime) {
    currentTruck_ = truckId;
    busyUntil_ = currentTime + 5;
    totalTrucksServed_ += 1;
}

//When we finish unloading we call this function to add 5 minutes to the total busy time of this station
//and reset the stations current truck to -1 to signify there is currently no truck in the station
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
/*Initializes the wait time as 0, and if our currentTime is before the busyUntil time then we add
busyUntil_ -  currentTime to the wait time since we have to wait for the current truck at the station
to finish unloading, afterwards we also have to add 5 minutes to our wait time for every truck in the stations 
queue for them to unload before returning our wait time. If our current wait time is after the time the station is busy until, then our wait time
is 0 so we can we just return wait which is initalized to be 0
*/
int UnloadStation::estimateWaitTime(int currentTime) const {
    int wait = 0;
    if (currentTime < busyUntil_) {
        wait += (busyUntil_ - currentTime);
        wait += (queue_.size() * 5);
    }
    return wait;
}