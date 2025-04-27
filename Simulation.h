#pragma once

#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime> 
#include <string>

class EVTOL; // Forward declaration of EVTOL class
   

class Simulation {
private:
    std::vector<EVTOL*> fleet; // Fleet of EVTOL vehicles
    static Simulation* instance; // Singleton instance
    int simNumberOfAircraft = 0;
    double simDurationHrs = 0;
    double SIM_TIME_STEP = 0.0166; // Time step for simulation in hours
    int simNumChargers = 0; // Changes during sim. Number of chargers currently available for EVTOLs
    int totalChargersInPool = 0; // This value doesn't change during sim. Total number of chargers in the pool.    

    // Private constructor to prevent direct instantiation
    Simulation();

    // Delete copy constructor and assignment operator to enforce Singleton behavior
    Simulation(const Simulation&) = delete;
    Simulation& operator=(const Simulation&) = delete;


public:
    // Returns the singleton instance
    static Simulation* getInstance();

    // Initializes the simulation with the given parameters:
    void initialize(int numAircraft, double durationHrs, int numChargers, double simTimeStep, bool enableDebug);

    // Charger-related methods:
    bool isChargerAvailable();
    void releaseCharger();
    bool useCharger();

    // Create a random fleet of EVTOLs
    void createRandomFleet();

    void runBasicSim();
    void runFullSim();
    // Runs the simulation
    void run();
    
    EVTOL* LookupEvtol(std::string companyName); // Lookup function to get EVTOL object by name.

    // Displays the results of the simulation
    void displayResults();
    void getVehicleDebugStats(EVTOL* vehicle, double simElapsedTime);
    void getVehicleStats(EVTOL* vehicle);
    int getFleetSize() { return fleet.size(); }

    // Cleans up the fleet of EVTOLs after each sim ru
    void CleanupVehicles();   

    // Destructor to clean up the fleet
    ~Simulation();

    bool debugMode = false; 
};

