
#include <iostream>
#include <cstdio> 
#include <map>
#include <string>
#include <chrono>
#include <ctime>
#include "Simulation.h"
#include "EVTOL.h"
#include "View.h"
#include "Document.h"

using namespace std;

/*****************************************************************************
 * Singleton Simulation Class
 * Implementation for managing and running simulations of EVTOL vehicles.
 *****************************************************************************/

 // Initialize the static instance pointer
Simulation* Simulation::instance = nullptr;


//------------------------------------------------------------------------
// Private constructor to prevent direct instantiation.
//------------------------------------------------------------------------
Simulation::Simulation() 
{   
}


//------------------------------------------------------------------------
// Returns the singleton instance.
//------------------------------------------------------------------------
Simulation* Simulation::getInstance()
{
    if (!instance)
    {
        instance = new Simulation();
    }
    return instance;
}


//------------------------------------------------------------------------
// Initializes the simulation parameters.
//------------------------------------------------------------------------
void Simulation::initialize(int numAircraft, double durationHrs, int numChargers, double simTimeStep, bool enableDebug)  
{
    cout << "\nInitializing simulation...\n" << endl;
    debugMode = enableDebug;
    simNumberOfAircraft = numAircraft;
    simDurationHrs = durationHrs;
    simNumChargers = numChargers; // Changes during sim. Number of chargers currently available for EVTOLs
    totalChargersInPool = numChargers; // Doesn't change during sim.   
    SIM_TIME_STEP = simTimeStep; // Time step for simulation in hours
    fleet.clear();
    createRandomFleet(); // Create a random fleet of EVTOLs
}


//------------------------------------------------------------------------
// Checks if a charger is available.
//------------------------------------------------------------------------
bool Simulation::isChargerAvailable()
{
    return simNumChargers > 0;
}


//------------------------------------------------------------------------
// Releases a charger.
//------------------------------------------------------------------------
void Simulation::releaseCharger()
{
    simNumChargers += 1;
}


//------------------------------------------------------------------------
// Aircraft attempts to use a charger.
//------------------------------------------------------------------------
bool Simulation::useCharger()
{
    if (isChargerAvailable())
    {
        simNumChargers -= 1;
        return true;
    }
    else
    {
        return false;
    }
}

//------------------------------------------------------------------------
// Creates a random fleet of EVTOL vehicles based on prototypes.
//------------------------------------------------------------------------
void Simulation::createRandomFleet()
{ 
    int randomIndex = 0; 

     vector<EVTOL*> prototypes = { 
        new EVTOL("Alpha",   120, 320, 0.6,  1.6, 4, 0.25),
        new EVTOL("Bravo",   100, 100, 0.2,  1.5, 5, 0.10),
        new EVTOL("Charlie", 160, 220, 0.81,  2.2, 3, 0.05),
        new EVTOL("Delta",    90, 120, 0.62, 0.8, 2, 0.22),
        new EVTOL("Echo",     30, 150, 0.3,  5.8, 2, 0.61)
    };

	// Set the random seed based on the current time:         
    auto now = chrono::system_clock::now();  // Get the current system time as a time_point       
    time_t currentTime = std::chrono::system_clock::to_time_t(now);  // Convert to a time_t                
    srand(time(&currentTime)); 

    for (int i = 0; i < simNumberOfAircraft; i++)
    {
        // int r = ((double)rand() / RAND_MAX) * (range_max - range_min) + range_min;
        randomIndex = ((double)rand() / RAND_MAX) * (prototypes.size());
        EVTOL* aircraft = new EVTOL(*prototypes[randomIndex]); // Generate randomly selected EVTOL Mfg
        aircraft->setAircraftID(i);  // Set unique ID
        fleet.push_back(aircraft); 
    }

    for (EVTOL* proto : prototypes) {
        delete proto; // Cleanup prototypes
    }
}


//------------------------------------------------------------------------
// Looks up an EVTOL object in the fleet by company name.
// Returns first match found or nullptr if not found.
//------------------------------------------------------------------------
EVTOL* Simulation::LookupEvtol(std::string companyName)
{
    for (EVTOL* vehicle : fleet)
    {
        if (vehicle->companyName == companyName)
        {
            return vehicle;
        }
    }

    return nullptr; // companyName not found
}


//-----------------------------------------------------------------------------
// Runs the Basic simulation by calculating and displaying statistics for each EVTOL type.
//-----------------------------------------------------------------------------
void Simulation::runBasicSim()
{
    double totalPassengerMiles = 0.0;
    double avgFlightTime = 0.0;
    double avgDistPerFlt = 0.0;
    double chargeTimePerSession = 0.0;
    double faults = 0.0;
    double totalChargingTime = 0.0;
    string companyName = ""; 
    int count = 0;
    
    // Create a map to store the number of EVTOLs per mfg type
	std::map<string, int> aircraftTypeCount;

	// Count the number of EVTOLs per type:
    for (EVTOL* vehicle : fleet)
    {
        aircraftTypeCount[vehicle->getCompanyName()]++;
	}

	// Print the statistics of EVTOLs per type:
    printf("----------------------------------------------\n");
	printf("BASIC SIMULATION - Statistics per Vehicle TYPE\n");
    printf("Total Aircraft: %zu\n", fleet.size()); // Use %zu for size_t
    printf("Simulation Duration: %.2f hours\n", simDurationHrs); // Assuming floating-point value
    printf("----------------------------------------------\n");
   
    printf("Company | Count | Avg-Flight-Time-Per-Flight | Avg-Distance-per-Flight | Avg-Charge-Time-per-Session | Total-Faults | Total-Passenger-Miles\n");
  
    for (const auto& pair : aircraftTypeCount)
    {		
        EVTOL* vehicle = LookupEvtol(pair.first.c_str());
        companyName = pair.first.c_str();
        count = pair.second;

        avgDistPerFlt = vehicle->batteryCapacity / vehicle->energyUsePerMile; 
        avgFlightTime = avgDistPerFlt / vehicle->cruiseSpeed;
        chargeTimePerSession = vehicle->getChargeTime(); 
        faults = vehicle->faultProbability * simDurationHrs;         
        totalChargingTime = simDurationHrs / avgFlightTime * chargeTimePerSession;  // Total time spent charging
        if (totalChargingTime > simDurationHrs)  totalChargingTime = totalChargingTime / 10.0; // Workaround for weird run-time bug!!! Calculation can be off by power of ten. 				
		totalPassengerMiles = (simDurationHrs - totalChargingTime) * vehicle->cruiseSpeed * count; 
        printf("%-8s| %-5d | %-26.2f | %-23.2f | %-27.2f | %-12.2f | %-10.2f\n", companyName.c_str(), count, avgFlightTime, avgDistPerFlt, chargeTimePerSession, faults, totalPassengerMiles);
	}
}


//-----------------------------------------------------------------------------
// Runs the FULL simulation framework by iterating over time steps and simulating 
// flights,chargers, timeWaitingforCharger,chargingTime, etc.
//-----------------------------------------------------------------------------
void Simulation::runFullSim()
{
    if (debugMode)
    {
        printf("--------------------------------------------------------------------------------------------------------------\n");
        printf("SimElapsedTime | Company | ID | Aircraft Miles | Passenger Miles | Faults | TimeCharging | TimeWaitingToCharge\n");
        printf("---------------------------------------------------------------------------------------------------------------\n");
    }

    for (double simElapsedTime = 0; simElapsedTime < simDurationHrs; simElapsedTime += SIM_TIME_STEP)
    {
        // Simulate flight for each EVTOL in the fleet:
        for (EVTOL* vehicle : fleet)
        {
            vehicle->simulateFlight(SIM_TIME_STEP);
            if (debugMode) getVehicleDebugStats(vehicle, simElapsedTime);
        }

    }
}

//------------------------------------------------------------------------
// Starting point to RUN the simulation. 
//------------------------------------------------------------------------
void Simulation::run()
{
    runBasicSim();
    runFullSim();
}


//------------------------------------------------------------------------
// Displays the results of the simulation.
//------------------------------------------------------------------------
void Simulation::displayResults()
{
    printf("\n\n");
    printf("------------------------------------------\n");
    printf("FULL SIMULATION RESULTS:\n");
    printf("------------------------------------------\n");
    printf("Total Aircraft: %zu\n", fleet.size()); // Use %zu for size_t
    printf("Total Chargers: %d\n", totalChargersInPool); // Assuming int type
    printf("Simulation Duration: %.2f hours\n", simDurationHrs); // Assuming floating-point value
    printf("------------------------------------------\n");

    printf("EVTOL Statistics:\n");
    printf("---------------------------------------------------------------------------------------------\n");
    printf("Company | ID | Aircraft Miles | Passenger Miles | Faults | TimeCharging | TimeWaitingToCharge\n");
    printf("----------------------------------------------------------------------------------------------\n");
   
    for (EVTOL* vehicle : fleet)
    {        
        getVehicleStats(vehicle);
	}

    printf("----------------------------------------------------------------------------------------------\n");   
}


void Simulation::getVehicleDebugStats(EVTOL* vehicle, double simElapsedTime)
{
    printf("%-16.4f %-9s %-4d %-16.2f %-18.2f %-10.2f %-13.2f %-6.2f \n",
        simElapsedTime,
        vehicle->getCompanyName().c_str(),
        vehicle->getAircraftID(),
        vehicle->getTotalDistance(),
        vehicle->getTotalPassengerMiles(),
        vehicle->getTotalFaults(),
        vehicle->getTotalTimeCharging(),
        vehicle->getTotalTimeWaitingtoCharge());
}


//------------------------------------------------------------------------
void Simulation::getVehicleStats(EVTOL* vehicle)
{
    printf("%-9s %-4d %-16.2f %-18.2f %-10.2f %-13.2f %-6.2f \n",
        vehicle->getCompanyName().c_str(),
        vehicle->getAircraftID(),
        vehicle->getTotalDistance(),
        vehicle->getTotalPassengerMiles(),
        vehicle->getTotalFaults(),
        vehicle->getTotalTimeCharging(),
        vehicle->getTotalTimeWaitingtoCharge());
}


//------------------------------------------------------------------------
// Cleans up the fleet of EVTOLs after each simulation run.
//------------------------------------------------------------------------
void Simulation::CleanupVehicles()
{
    for (EVTOL* vehicle : fleet) {
        delete vehicle; // Cleanup memory
    }
}

//------------------------------------------------------------------------
// Destructor to clean up the fleet.
//------------------------------------------------------------------------
Simulation::~Simulation()
{
    for (EVTOL* vehicle : fleet) {
        delete vehicle; // Cleanup memory
    }
}