class Truck {
    public:
        Truck(int id); //constructor
        int getId();

        //Time changes
        void addMiningTime(int minutes); //random between 1 to 5 hours
        void addTravelTime(); //travel always takes 30 mins;
        void addUnloadTime(); //unloading always takes 5 mins
        void addWaitTime(int minutes); //can depend on if stations are occupied
        
        void addCycle(); //cycle should always add 1

        // metrics
        int getMiningTime() const;
        int getTravelTime() const;
        int getUnloadTime() const;
        int getWaitTime() const;
        int getCycles() const;

    private:
        int id_;
        int miningTime_;
        int travelTime_;
        int unloadTime_;
        int waitTime_;
        int cyclesCompleted_;

};