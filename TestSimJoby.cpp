#include "TestSimJoby.h"
#include "EVTOL.h"
#include "Simulation.h"
#include "View.h"
#include "Document.h"

//----------------------------
// Framework for unit tests.
//----------------------------
using namespace std;

// Test the EVTOL fleet creation
bool testFleetCreation(int n)
{
	Simulation* sim = Simulation::getInstance();
	sim->initialize(n, 1.0, 5, 0.0166, true);
	sim->createRandomFleet();
	
	// Check if the fleet size matches the expected size
	if (sim->getFleetSize() != n)
	{
		cout << "Fleet creation test failed: Expected fleet size " << n << " got " << sim->getFleetSize() << endl;
		return false;
	}
	else
	{
		cout << "Fleet creation test passed." << endl;
		return true;
	}
}


bool testEVTOL()
{
	EVTOL testVehicle("Test Co", 100, 200, 0.5, 2, 4, 0.1);
	return true;
}

// Test the Simulation class
bool testSimulation()
{
	return true;
}

// Test the View class
bool testView()
{
	return true;
}

// Test the Document class
bool testDocument()
{
	return true;
}



bool testChargerAvailability()
{
	return true;
}


// Test the simulation run
bool testSimulationRun()
{	
	return true;
}

bool runUnitTests()
{	
	if (!testEVTOL())
	{
		cout << "EVTOL test failed." << endl;
		return false;
	}

	if (!testSimulation())
	{
		cout << "Simulation test failed." << endl;
		return false;
	}
	
	if (!testView())
	{
		cout << "View test failed." << endl;
		return false;
	}
	
	if (!testDocument())
	{
		cout << "Document test failed." << endl;
		return false;
	}

	if (!testFleetCreation(20))
	{
		cout << "Fleet creation test failed." << endl;
		return false;
	}

	cout << "All tests passed." << endl;
	return true;

}