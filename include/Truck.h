#pragma once
class Truck {
    public:
        //constructor that assigns an Id to a truck
        Truck(int id); 
        
        //random between 1 to 5 hours converted to minutes
        void addMiningTime(int minutes); 

        //travel always takes 30 mins; += 30
        void addTravelTime(); 

        //unloading always takes 5 mins += 5
        void addUnloadTime(); 

        //can depend on if stations are occupied
        void addWaitTime(int minutes); 
        
        void addCycle(); //cycle should always add 1

        // Getters for member variables and for use in metrics of performance and efficiency
        int getId() const;
        int getMiningTime() const;
        int getTravelTime() const;
        int getUnloadTime() const;
        int getWaitTime() const;
        int getCycles() const;

    private:

        //Unique Truck Id identifier
        int id_;

        //Total Time that this truck has spent mining
        int miningTime_;

        //Total Time that this truck has spent traveling between a mining site and a unloading station
        int travelTime_;

        //Total Time that this truck has spent unloading at an unloading station
        int unloadTime_;

        //Total Time that this truck has spent waiting in queue to unload at a busy unloading station
        int waitTime_;

        /*Total Cycles of 
        Mining -> Traveling to Unloading Station -> Unloading -> Traveling back to a mining site 
        that this truck has completed
        */
        int cyclesCompleted_;

};