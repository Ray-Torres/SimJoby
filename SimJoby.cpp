
//*****************************************************************************
// SimJoby.cpp : This file contains the 'main' function. Program execution begins here.
//*****************************************************************************

#include "EVTOL.h"
#include "Simulation.h"
#include "View.h"
#include "Document.h"


//*****************************************************************************
// Main Function: Entry point of the program.
//*****************************************************************************
int main() {    
    double durationHrs;
    double simTimeStep = 0.1; // in hours
    int numAircraft;
    int numChargers; 
    bool isUserRerun = false; // Flag to check if user wants to rerun the simulation
    bool enableDebug = false; 
    Simulation* sim = Simulation::getInstance(); // Get the singleton instance of Simulation
    View* view = View::getInstance();            //             "                 and View

    view->SayHello();

    do
    {
		view->GetUserInput(durationHrs, numAircraft, numChargers, simTimeStep, enableDebug); 
		sim->initialize(numAircraft, durationHrs, numChargers, simTimeStep, enableDebug);
		sim->run(); 
		sim->displayResults();              
        sim->CleanupVehicles(); // Clean up the fleet of EVTOLs after each simulation run.
        isUserRerun = view->AskUserRerunChoice();
	} while (isUserRerun); // Continue until User chooses not to rerun.

    view->SayGoodbye();
    return 0;
}


