#pragma once
#include <queue>

class UnloadStation {
    public:
        //Constructor that assigns the station a unique Id
        UnloadStation(int id);

        //Gets the stations Id
        int getId() const;

        //Checks if the station is busy at currentTime
        bool isBusy(int currentTime) const;

        //Returns the current length of the queue which is how many trucks waiting to unload, doesn't include the truck currently unloading
        int getQueueLength() const;

        //If the station isn't currently busy, then immediately start unloading the truck with truckId at currentTime
        //Otherwise, the truck gets pushed to the station's FIFO queue of trucks waiting to unload
        void addTruck(int truckId, int currentTime);

        //Returns the next truck in the queue and removes it from the queue as well.
        int popNextTruck();

        //Unloads the truck with truckId at currentTime, sets truck as the station's current truck
        // changes the stations busy until time to currentTime + 5 minutes to account for the 5 minutes it takes to unload a truck
        //Also increments the station's count of total trucks serviced
        void startUnload(int truckId, int currentTime);

        //Finishes the Unloading Process by adding 5 minutes to the station's count of total unload time
        //and resets the station's currentTruck by setting it to -1
        void finishUnload();

        //Used for metrics reporting
        int getTotalTrucksServed() const;
        int getTotalBusyTime() const;

        // Used to help decide which stations trucks should go to
        int estimateWaitTime(int currentTime) const;

    private:
        //Unique Station Id
        int id_;

        //Time that the station is busy until, no other unloading operations will happen until after this time
        int busyUntil_;

        //Id of the truck that is currently unloading at the station
        int currentTruck_;

        //Total amount of trucks that have unloaded at this station
        int totalTrucksServed_;

        //Total amount of time that trucks have spent unloading at this station
        //Should be a multiple of 5 since it always takes 5 minutes for unloading
        int totalBusyTime_;

        //Queue used by the station to maintain a FIFO queue of trucks waiting to unload at the station while there
        // is currently a truck unloading at the station
        std::queue<int> queue_;
};