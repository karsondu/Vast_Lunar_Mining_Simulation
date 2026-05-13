#include <vector>
#include <queue>
#include <ctime>

#include "Truck.h"
#include "UnloadStation.h"
#include "Event.h"


class Simulation {
    public:
        Simulation(int n, int m, unsigned int seed=time(0));
        void run();
        void printStatistics() const;
    
    private:
    int currentTime_; //Current Simulation Time
    int simulationEndTime_; //72 hours, so 72*60 to convert to minutes

    std::vector<Truck> trucks_;
    std::vector<UnloadStation> stations_;
    std::priority_queue<Event,
                        std::vector<Event>,
                        std::greater<Event>> eventQueue_;

    void initializeEvents();
    void processEvent(const Event& event);
    int chooseBestStation(int currentTime); // Picks out the station with smallest wait time
    int generateMiningTime(); //Generates a random time from 1 - 5 hours converted to minutes, so
                            //60 - 300 minutes


};