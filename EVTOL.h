#pragma once

#include <string>
#include <iostream>
#include "Simulation.h" // Include the Simulation class for accessing its instance.

using namespace std;

/******************************************************************************
 * EVTOL class representing an eVTOL vehicle:
 * Contains data and methods to simulate an eVTOL vehicle,
 * including simulating a flight, charging the battery, and getting statistics.
 ******************************************************************************/
class EVTOL {
public:
    string companyName;            // Name of the eVTOL manufacturer     
    double cruiseSpeed;            // Cruise speed of the aircraft (mph)
    double batteryCapacity;        // Battery capacity (kWh)
    double chargeTime;             // Time required for a full charge (hours)
    double energyUsePerMile;       // Energy consumption per mile (kWh/mile)
    int passengerCount;            // Number of passengers the aircraft can carry
    double faultProbability;       // Probability of a fault occurring per hour
    double batteryLevel;           // Current battery level (kWh)
    double totalDistance;          // Total distance traveled (miles)
    double totalPassengerMiles;    // Total passenger miles traveled
    double totalFaults;            // Total number of faults encountered
    double totalTimeWaitingtoCharge; // Total time spent waiting to charge
    double totalTimeCharging;        // Total charge time during entire sim
    double accumulatedChargeTime;    // Time spent charging during the current charge cycle

    // Enum to represent the current state of the aircraft
    enum class AircraftState { FLYING, CHARGING, WAITING_TO_CHARGE, IDLE };
    AircraftState currentState;    // Current state of the aircraft

    Simulation* sim;               // Pointer to the Simulation singleton instance

public:
    // Constructor: Initializes the attributes and sets the initial state to FLYING.    
    EVTOL(string name, double speed, double capacity, double charge, double energy,
        int passengers, double fault);

    // Getters for various metrics:
    double getTotalDistance();
    double getBatteryLevel();
    double getChargeTime();
    double getTotalPassengerMiles();
    double getTotalTimeWaitingtoCharge();
    double getTotalTimeCharging();
    string getCompanyName(); 
    double getTotalFaults();    
    int aircraftID;                
    void setAircraftID(int aircraftID);
    int getAircraftID();      
    void SimulateCharging(double time);
    void SimulateWaitingToCharge(Simulation* sim, double time);
    void SimulateFlying(double time);

    // Simulate a flight for the given time period.
    // Handles transitions between FLYING, CHARGING, and WAITING_TO_CHARGE states.    
    void simulateFlight(double time);     
};



