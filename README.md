# Lunar Mining Simulation

## Overview

This project is a simulation of a lunar mining system over the course of 72 hours. It models a fleet of trucks that cycle between mining sites where they will mine for a random amount of time, travel to unload stations where they will spend time unloading their payload or entering a queue to wait for their turn to unload their payload.

Rather than a time-based mechanic that moves our simulation minute by minute, we use an event-based mechanic where time moves by processing events in an event queue at different times rather than using fixed time steps

---

## System Model

Each truck continuously repeats the following cycle until the 72 hours of the simulation are complete:

- **Mining Phase**: The truck mines Helium-3 for a randomly generated duration (60–300 minutes).
- **Travel Phase**: After mining, the truck travels to an unload station (fixed 30 minutes).
- **Unload Phase**: The truck unloads at a station (fixed 5 minutes). If all stations are busy, the truck waits in a FIFO queue at the station with the shortest wait time.
- **Repeat**: After unloading, the truck travels back another fixed 30 minutes to once again start the Mining Phase.

---

## Event-Based Design

The simulation is driven by a priority queue of events ordered by the time that they are to occur at.

### Event Types
- `MiningComplete`: A truck finishes mining and begins travel to a station
- `ArriveAtStation`: A truck arrives and either begins unloading or joins a queue
- `UnloadComplete`: A truck finishes unloading and travels back to a mining site before starting a new mining cycle

Each event contains:
- Event time
- Event type
- Truck ID of related Truck
- Station ID of related Station (if applicable)

---

## Core Components

### Simulation Engine
Responsible for:
- Initializing trucks and stations
- Generating initial mining events
- Processing events in chronological order
- Managing global simulation time
- Coordinating interactions between trucks and stations

---

### Truck
Tracks per-truck performance metrics across the full simulation:
- Total mining time
- Total travel time
- Total waiting time
- Total unloading time
- Total number of completed cycles

---

### Unload Station
Represents shared unloading infrastructure with constrained capacity:
- Processes one truck at a time
- Maintains a FIFO queue for waiting trucks
- Tracks utilization (busy time)
- Tracks number of trucks served
- Provides estimated wait time for load balancing

---

### Event
Represents a scheduled action in the system:
- Event execution time
- Event type
- Associated truck ID
- Optional station ID

Events are processed using a min-heap priority queue to ensure chronological execution.

---

## Station Selection Strategy

When a truck arrives at a station, the simulation selects the station with the lowest estimated wait time. This estimate considers:
- Current station availability
- Remaining busy time
- Queue length

This approach balances load across stations and reduces overall waiting time.

---

## Simulation Constants

- Simulation duration: **72 hours (4320 minutes)**
- Mining time: **random between 60–300 minutes**
- Travel time: **fixed 30 minutes**
- Unload time: **fixed 5 minutes**

---

## Build Instructions

### Requirements
- C++17 or later
- CMake 3.10+
- MSYS2 / MinGW or compatible toolchain

### Build Steps

```bash
mkdir build
cd build
cmake ..
make