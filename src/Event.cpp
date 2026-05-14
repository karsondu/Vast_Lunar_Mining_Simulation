#include "Event.h"

//Constructor creates Event object -> (time of Event,Event Type, Associated TruckId, Associated StationId)
Event::Event(int time,
             EventType type,
             int truckId,
             int stationId)
    : time_(time),
      type_(type),
      truckId_(truckId),
      stationId_(stationId) {}


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
//Comparitor checks if time > other.time, if so it means that the current event object comes after the other event object
bool Event::operator>(const Event& other) const {
    return time_ > other.time_;
}