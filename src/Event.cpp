#include "Event.h"
#include <stdexcept>

//Constructor creates Event object -> (time of Event,Event Type, Associated TruckId, Associated StationId)
Event::Event(int time,
             EventType type,
             int truckId,
             int stationId)
    : time_(time),
      type_(type),
      truckId_(truckId),
      stationId_(stationId) {
        if (time < 0) {
            throw std::invalid_argument("Event time cannot be negative");
        }
    }


//Getters
int Event::getTime() const {
    return time_;
}

EventType Event::getType() const {
    return type_;
}

int Event::getTruckId() const {
    return truckId_;
}

int Event::getStationId() const {
    return stationId_;
}

// Comparison for our events priority queue, ensures that earlier events come first in our priority queue
//Comparitor checks if time > other.time, if so it means that the current event object comes after the other event object,
// if the times are tied, then we go by event type priority of UnloadComplete,MiningComplete, and then ArriveAtStation
bool Event::operator>(const Event& other) const {
    if (time_ != other.time_) return time_ > other.time_;
    return static_cast<int>(type_) > static_cast<int>(other.type_);
}