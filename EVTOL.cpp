#include "EVTOL.h"
#include "Simulation.h"
#include <iostream>
#include <string>

/*****************************************************************************
 * EVTOL class implementation:
 * Contains methods for simulating a flight, charging the battery, and
 * retrieving statistics for the eVTOL vehicle.
 *****************************************************************************/

 //------------------------------------------------------------------------
 // Constructor:
 // Initializes the attributes and sets the initial state to FLYING.
 //------------------------------------------------------------------------
EVTOL::EVTOL(string name, double speed, double capacity, double charge, double energy,
    int passengers, double fault)
    : companyName(name), cruiseSpeed(speed), batteryCapacity(capacity), chargeTime(charge),
    energyUsePerMile(energy), passengerCount(passengers), faultProbability(fault), aircraftID(0)
{
    batteryLevel = batteryCapacity;
    totalDistance = totalPassengerMiles = totalTimeWaitingtoCharge = totalFaults = totalTimeCharging = accumulatedChargeTime = 0;
    currentState = AircraftState::FLYING;
    sim = Simulation::getInstance(); // Get the Simulation instance
}

void EVTOL::setAircraftID(int id) { aircraftID = id; }
int EVTOL::getAircraftID() { return aircraftID; }
double EVTOL::getTotalDistance() { return totalDistance; }
double EVTOL::getBatteryLevel() { return batteryLevel; }
double EVTOL::getChargeTime() { return chargeTime; }    
double EVTOL::getTotalPassengerMiles() { return totalPassengerMiles; }
double EVTOL::getTotalTimeWaitingtoCharge() {  return totalTimeWaitingtoCharge; } 
double EVTOL::getTotalTimeCharging() { return totalTimeCharging; }
string EVTOL::getCompanyName() { return companyName; }
double EVTOL::getTotalFaults() { return totalFaults; }


//------------------------------------------------------------------------
// Simulate charging the battery for the given time period.
// If the battery is fully charged, switch to FLYING state, and release 
// // the charger.
// If not fully charged, accumulate charge time, and keep charging.
//------------------------------------------------------------------------
void EVTOL::SimulateCharging(double time)
{
    accumulatedChargeTime += time;  // Current charge cycle
    totalTimeCharging += time; // Total charge time during entire sim

    if (accumulatedChargeTime >= chargeTime)
    {
        batteryLevel = batteryCapacity; // Fully charged
        accumulatedChargeTime = 0; // Reset accumulated charge time
        currentState = AircraftState::FLYING; // Switch state
        sim->releaseCharger(); // Release the charger
    }
}

//------------------------------------------------------------------------
// Simulate waiting to charge.
// If a charger is available, switch to CHARGING state.
// If not, accumulate waiting time.
//------------------------------------------------------------------------
void EVTOL::SimulateWaitingToCharge(Simulation* sim, double time)
{
    if (sim->isChargerAvailable())
    {
        currentState = AircraftState::CHARGING; // Switch state
        sim->useCharger();
    }
    else
    {
        totalTimeWaitingtoCharge += time; // Accumulate waiting time
    }
}


//---------------------------------------------------------------------
// Simulate a flight for the given time period.
// Updates the battery level, total distance, and total passenger miles.
//---------------------------------------------------------------------
void EVTOL::SimulateFlying(double time)
{
    double distance = cruiseSpeed * time;
    batteryLevel -= distance * energyUsePerMile;

    totalDistance += distance;
    totalPassengerMiles += passengerCount * distance;
    totalFaults += faultProbability * time;

    if (batteryLevel <= 0)
    {
        batteryLevel = 0;  // Prevent negative battery level.
        accumulatedChargeTime = 0; // Reset accumulated charge time.
        currentState = AircraftState::WAITING_TO_CHARGE; // Switch state
    }
}


//------------------------------------------------------------------------
// Simulate a flight for the given time period.
// Handles state transitions: FLYING -> WAITING_TO_CHARGE -> CHARGING -> FLYING.
//------------------------------------------------------------------------
void EVTOL::simulateFlight(double time)
{
    Simulation* sim = Simulation::getInstance(); // Get the singleton instance of Simulation

    if (currentState == AircraftState::FLYING)
    {
        SimulateFlying(time);
    }
    else if (currentState == AircraftState::WAITING_TO_CHARGE)
    {
        SimulateWaitingToCharge(sim, time);
    }
    else if (currentState == AircraftState::CHARGING)
    {
        SimulateCharging(time);
    }
}

