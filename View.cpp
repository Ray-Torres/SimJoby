#include <iostream>
#include <vector>

#include "View.h"
#include "EVTOL.h"

using namespace std;

/*****************************************************************************
 * View Class: Implementation for handling user interaction.
 * Provides methods for displaying messages and collecting input.
 *****************************************************************************/

 //------------------------------------------------------------------------
 // Initialize the static instance pointer. 
View* View::instance = nullptr;

//------------------------------------------------------------------------
// Private constructor to prevent direct instantiation.
//------------------------------------------------------------------------
View::View() {}


//------------------------------------------------------------------------
// Get the singleton instance.
//------------------------------------------------------------------------
View* View::getInstance()
{
    if (instance == nullptr)
    {
        instance = new View();
    }
    return instance;
}

//------------------------------------------------------------------------
// Called at the beginning of the program to display a welcome message.
//------------------------------------------------------------------------
void View::SayHello()
{
    printf("----------------------\n");
    printf("Joby eVTOL Simulation!\n");
    printf("----------------------\n\n");
    printf("Press <ENTER> key to BEGIN.");
    cin.get(); // Wait for user input.
}

//------------------------------------------------------------------------
// Called at the end of the program to display a message and wait for user input.
//------------------------------------------------------------------------
void View::SayGoodbye()
{
    printf("\n\n");
    printf("---------------------------------\n");
    printf("End of Joby eVTOL Simulation\n");
    printf("---------------------------------\n");
    printf("Press <ENTER> key to end program.");
    cin.get(); // Wait for user input.
}

//------------------------------------------------------------------------
// Get user input for simulation parameters.
//------------------------------------------------------------------------
void View::GetUserInput(double& durationHrs, int& numAircraft, int& numChargers, double& simTimeStep, bool& enableDebug)
{
    // For testing purposes set default values for simulation parameters:
    // durationHrs = 3.0; numAircraft = 20;  numChargers = 3;   
    // return;

    int scanRetVal = 0;
    double scanDouble = 0.0;
    char scanChar = 'n';
    

    printf("\n---------------------------------\n");

    printf("Enter simulation duration (hours): ");
    scanDouble = scanf_s("%lf", &durationHrs);
    // Validate user input:
    if (durationHrs < 0)
    {
        printf("Invalid input. Setting to default of 3 hours.\n");
        durationHrs = 3.0;
    }

    printf("Enter number of aircraft: ");
    scanRetVal = scanf_s("%d", &numAircraft);
    // Validate user input:
    if (scanRetVal != 1 || numAircraft < 0)
    {
        printf("Invalid input. Setting to default of 20.\n");
        numAircraft = 20;
    }

    printf("Enter number of charging stations: ");
    scanRetVal = scanf_s("%d", &numChargers);
    // Validate user input:
    if (scanRetVal != 1 || numChargers < 0)
    {
        printf("Invalid input. Setting to default of 3.\n");
        numChargers = 3;
    }

    printf("Enter simulation Time-step (hours): Enter 0 to set default of 0.016 hrs (1 minute) ");
    scanDouble = scanf_s("%lf", &simTimeStep);
    // Validate user input:
    if (simTimeStep <= 0)
    {
        printf("     Setting Time-step to default of 0.0166 hours.\n");  // 1 minute = 0.0166667 hours
        simTimeStep = 0.0166;  
    }

    printf("Enable Debug trace output? (y/n): ");
    cin >> scanChar;
    
    if (scanChar == 'y' || scanChar == 'Y')    
        enableDebug = true;
    else
        enableDebug = false;    
}


//------------------------------------------------------------------------
// Get user input for rerun choice.
//------------------------------------------------------------------------
bool View::AskUserRerunChoice()
{
    char rerunChoice;
	printf("\n\nDo you want to rerun the simulation? (y/n): ");
	
	cin >> rerunChoice;
    if (rerunChoice == 'y' || rerunChoice == 'Y')
    {
		return(true); 
	}
    else
    {
		return(false); 
	}
}

