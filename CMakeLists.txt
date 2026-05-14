cmake_minimum_required(VERSION 3.10)

# Project name
project(LunarMiningSimulation)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Header file directory
include_directories(include)

# Create executable
add_executable(sim
    src/main.cpp
    src/Simulation.cpp
    src/Truck.cpp
    src/UnloadStation.cpp
    src/Event.cpp
)