#include <iostream>
#include <cassert>

#include "Simulation.h"
#include "Truck.h"
#include "UnloadStation.h"
#include "Event.h"

// Testing if all Truck methods function properly
void testTruckBasics() {
    Truck t(0);

    t.addMiningTime(120);
    t.addTravelTime();
    t.addWaitTime(15);
    t.addUnloadTime();
    t.addCycle();

    assert(t.getId() == 0 && "Id Assignment Failed");
    assert(t.getMiningTime() == 120 && "Mining Time Assignment Failed");
    assert(t.getTravelTime() == 30 && "Travel Time Assignment Failed");
    assert(t.getWaitTime() == 15 && "Wait Time Assignment Failed");
    assert(t.getUnloadTime() == 5 && "Unload Time Increment Failed");
    assert(t.getCycles() == 1 && "Cycle Increment Failed");

    std::cout << "[PASS] Truck basic functions\n";
}

// Testing if event comparison ordering works properly
void testEventOrdering() {
    Event e1(10, EventType::MiningComplete, 0, -1);
    Event e2(20, EventType::ArriveAtStation, 1, -1);
    Event e3(40, EventType::UnloadComplete, 1, 0);

    assert(e2 > e1 && "Event ordering failed");
    assert(e3 > e1 && "Event ordering failed");
    assert(e3 > e2 && "Event ordering failed");

    std::cout << "[PASS] Event ordering\n";
}

// Testing negative event time handling
void testNegativeEventTime() {
    bool exceptionThrown = false;
    try {
        Event e(-10, EventType::MiningComplete, 0, -1);
    }
    catch (const std::invalid_argument&) {
        exceptionThrown = true;
    }

    assert(exceptionThrown && "Negative event time was allowed");

    std::cout << "[PASS] Negative event time handling\n";
}

// Testing queueing and busy state functionality
void testStationBasicBehavior() {
    UnloadStation s(0);

    // Station should initially be free
    assert(!s.isBusy(0) && "Station incorrectly marked busy");
    assert(s.getQueueLength() == 0 && "Queue should initially be empty");

    // First truck should immediately begin unloading
    s.addTruck(1, 0);

    assert(s.getQueueLength() == 0 && "First truck incorrectly added to queue");

    // Second truck should enter queue
    s.addTruck(2, 0);

    assert(s.getQueueLength() == 1 &&
           "Second truck was not added to queue");

    std::cout << "[PASS] Station basic behavior\n";
}

// Testing FIFO ordering of queued trucks
void testStationFIFO() {
    UnloadStation s(0);

    s.addTruck(0, 0);
    s.addTruck(1, 0);
    s.addTruck(2, 0);

    // One truck unloading, two waiting
    assert(s.getQueueLength() == 2 && "Queue length incorrect");

    int first = s.popNextTruck();
    int second = s.popNextTruck();

    assert(first == 1 && "FIFO ordering failed");
    assert(second == 2 && "FIFO ordering failed");

    std::cout << "[PASS] Station FIFO ordering\n";
}

// Testing unloading state transitions
void testFinishUnload() {
    UnloadStation s(0);

    s.startUnload(0, 0);

    assert(s.isBusy(0) && "Station should be busy during unload");

    s.finishUnload();

    assert(!s.isBusy(5) && "Station should not be busy after finishUnload");

    std::cout << "[PASS] finishUnload behavior\n";
}

// Testing estimated wait time calculations
void testEstimateWaitTime() {
    UnloadStation s(0);

    // Empty station should have no wait
    assert(s.estimateWaitTime(0) == 0 && "Empty station wait time incorrect");

    // First truck begins unloading
    s.addTruck(0, 0);

    // Wait should now be at one unload cycle of 5 minutes
    assert(s.estimateWaitTime(0) == 5 && "Unload wait time incorrect");

    // Additional truck joins queue
    s.addTruck(1, 0);

    // Wait should increase further
    assert(s.estimateWaitTime(0) == 10 && "Queue wait time incorrect");

    std::cout << "[PASS] Station wait estimation\n";
}

// Testing total trucks served tracking
void testTotalTrucksServed() {
    UnloadStation s(0);

    s.startUnload(0, 0);
    s.finishUnload();

    s.startUnload(1, 5);
    s.finishUnload();

    assert(s.getTotalTrucksServed() == 2 && "Truck served tracking failed");

    std::cout << "[PASS] Total trucks served tracking\n";
}

// Testing total busy time accumulation
void testBusyTimeTracking() {
    UnloadStation s(0);

    s.startUnload(0, 0);
    s.finishUnload();

    s.startUnload(1, 5);
    s.finishUnload();

    // Two unload cycles at 5 minutes each
    assert(s.getTotalBusyTime() == 10 && "Busy time tracking failed");

    std::cout << "[PASS] Busy time tracking\n";
}

// Testing queue shrinking after dequeue operations
void testQueueShrink() {
    UnloadStation s(0);

    s.addTruck(0, 0);
    s.addTruck(1, 0);
    s.addTruck(2, 0);

    assert(s.getQueueLength() == 2 && "Initial queue size incorrect");

    s.popNextTruck();

    assert(s.getQueueLength() == 1 && "Queue size did not shrink correctly");

    std::cout << "[PASS] Queue shrinking\n";
}

// Testing if simulation runs successfully
void testSimulation() {
    Simulation sim(2, 1);

    sim.run();

    std::cout << "[PASS] Simulation execution\n";
}

// Testing simulation under heavy load
void testLargeSimulation() {
    Simulation sim(100, 5);

    sim.run();

    std::cout << "[PASS] Large simulation stress test\n";
}


//main test runner
int main() {
    std::cout << "Running simulation test suite...\n\n";

    // Truck tests
    testTruckBasics();

    // Event tests
    testEventOrdering();
    testNegativeEventTime();

    // Station tests
    testStationBasicBehavior();
    testStationFIFO();
    testFinishUnload();
    testEstimateWaitTime();
    testTotalTrucksServed();
    testBusyTimeTracking();
    testQueueShrink();

    // Simulation tests
    testSimulation();
    testLargeSimulation();

    std::cout << "\nAll tests passed successfully.\n";

    return 0;
}