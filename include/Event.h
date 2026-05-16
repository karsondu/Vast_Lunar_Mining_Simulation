#pragma once
//Represents the different types of events that can occur during the simulation, 
//gets used during processing of an event object to determine processing behavior 
enum class EventType {
    UnloadComplete = 0,
    MiningComplete = 1,
    ArriveAtStation = 2,
};

class Event {
    public:
        //Constructor
        Event(int time,    //Time value for us to know when this event occurs and to update currentTime_
            EventType type,     
            int truckId,        
            int stationId = -1); 

        //Getters for member variables information
        
        //Returns event timestamp
        int getTime() const; 
        
        //Returns event type
        EventType getType() const;

        //Returns associated TruckId
        int getTruckId() const; 

        //Returns associated StationId
        int getStationId() const;

        // Comparison operator used by the event priority queue.
        // Earlier events are given higher priority.
        bool operator>(const Event& other) const; 

    private:
        int time_;

        //EventType from above to let us know how to process the event
        EventType type_; 

        //Id of truck related to event
        int truckId_; 
        
        //default value of -1 for station since not all events require a station
        int stationId_; 
};