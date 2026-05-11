#include <queue>

class UnloadStation {
public:
    UnloadStation(int id);

    int getId() const;

    // Query
    bool isBusy(int currentTime) const;
    int getQueueLength() const;

    // Core behavior
    void addTruck(int truckId, int currentTime);
    void startUnload(int truckId, int currentTime);
    void finishUnload(int currentTime);

    // Metrics
    int getTotalTrucksServed() const;
    int getTotalBusyTime() const;

    // Decision helper
    int estimateWaitTime(int currentTime) const;

private:
    int id_;

    bool busy_;
    int busyUntil_;

    int currentTruck_;

    std::queue<int> queue_;

    int totalTrucksServed_;
    int totalBusyTime_;
};