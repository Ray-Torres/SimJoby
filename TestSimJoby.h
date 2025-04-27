#pragma once

//----------------------------
// Framework for unit tests.
//----------------------------
class TestSimJoby
{
	public:
	// Test the EVTOL class
	bool testEVTOL();

	// Test the Simulation class
	bool testSimulation();

	// Test the View class
	bool testView();

	// Test the Document class
	bool testDocument();

	// Test the EVTOL fleet creation
	bool testFleetCreation(int n);

	// Test the charger availability
	bool testChargerAvailability();

	// Test the simulation run
	bool testSimulationRun();
};

