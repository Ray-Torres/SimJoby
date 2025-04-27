#pragma once

#include <iostream>

/*****************************************************************************
 * View Class: A Singleton. Encapsulates user input and output.
 * Implements Document-View design pattern.
 *****************************************************************************/
class View {
private:
    static View* instance; // Static instance pointer

    // Private constructor to prevent direct instantiation
    View();

    // Delete copy constructor and assignment operator to enforce Singleton behavior
    View(const View&) = delete;
    View& operator=(const View&) = delete;

public:
    
    // Static method to get the singleton instance.   
    static View* getInstance();
 
    // Called at beginning of the program to display a welcome message.  
    void SayHello();
    
    // Called at end of the program to display a message and wait for user input.   
    void SayGoodbye();

    // Get user input for simulation parameters.
    void GetUserInput(double& durationHrs, int& numAircraft, int& numChargers, double& simTimeStep, bool& enableDebug);

    // Get user input for rerun choice.
    bool AskUserRerunChoice();
};


