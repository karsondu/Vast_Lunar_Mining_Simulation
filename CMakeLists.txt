cmake_minimum_required(VERSION 3.10)

project(LunarMiningSimulation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

#Creating a shared library
add_library(sim_core
    src/Simulation.cpp
    src/Truck.cpp
    src/UnloadStation.cpp
    src/Event.cpp
)

# Main executable
add_executable(sim
    src/main.cpp
)
target_link_libraries(sim sim_core)

# Test executable
add_executable(sim_tests
    tests/test_simulation.cpp
)
target_link_libraries(sim_tests sim_core)