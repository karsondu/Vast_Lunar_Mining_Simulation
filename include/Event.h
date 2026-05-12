enum class EventType { //event types for tracking, only adding time when done
    MiningComplete,
    ArriveAtStation,
    UnloadComplete,
    ReturnToMine
};

class Event {
    public:
        Event(int time,
            EventType type,
            int truckId,
            int stationId = -1); //default value of -1 for station since not all events require a station

        int getTime() const; //We can use this to update our simulation time

        EventType getType() const;

        int getTruckId() const; 

        int getStationId() const;

    private:
        int time_;

        EventType type_;

        int truckId_;

        int stationId_;
};