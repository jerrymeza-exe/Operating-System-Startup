#include <iostream>
#include "auth.h"

using namespace std;

void systemBoot() {  // Simulates the system booting
    cout << "System Booting..." << endl;
    int i = 0;
    while (i <= 100) {  // Simulates a loading progress from 0% to 100%
        cout << "Loading: " << i << "%" << endl;
        for (long j = 0; j < 1000000000; j++) { }
        i += 20;  // Loads in increments of 20%
    }
    cout << "Boot Complete!" << endl;
}



int main() {
    systemBoot(); 

    int choice;
    cout << "\n--- Startup Menu ---" << endl;
    cout << "1. Login" << endl;
    cout << "2. Create Account" << endl;
    cout << "3. Exit" << endl;
    cout << "Enter your choice: ";
    cin >> choice;

    bool loggedIn = false;

    if (choice == 1) {
        loggedIn = authenticateUser();  // Login function
    } 
    else if (choice == 2) {
        string newUsername, newPassword;
        cout << "Enter a username: ";
        cin >> newUsername;
        cout << "Enter a password: ";
        cin >> newPassword;
        addUser(newUsername, newPassword);  // Adds new user to the users.txt file
        loggedIn = true;  // New users get logged in automatically
    } 
    else if (choice == 3) {
        cout << "Exiting... Goodbye!" << endl;
        return 0;
    } 
    else {
        cout << "Invalid choice." << endl;
        return 0;
    }

    if (!loggedIn) {
        cout << "Access denied. Restart the system." << endl;
        return 0;
    }

    cout << "\nWelcome!" << endl;

    int currentTime = 0;

    // Creating 2 processes for simulation
    // Process ID, Arrival Time, Burst Time, Priority, Memory Required, I/O Operations

    Process p1(1, 0, 10, 1, 512, false);
    Process p2(2, 2, 5, 2, 256, true);
    
    // Process simulation
    cout << "\n--- Process Simulation ---" << endl;

    p1.setWaitingTime(currentTime);
    p1.updateState("Ready");
    p1.showProcess();

    p1.updateState("Running");
    p1.decrementRemainingTime(4);
    p1.setTurnaroundTime(currentTime);
    p1.showProcess();

    p1.decrementRemainingTime(6);
    currentTime += 6;
    p1.setTurnaroundTime(currentTime);
    p1.showProcess();

    cout << "\n--- Next Process ---" << endl;

    p2.setWaitingTime(currentTime);
    p2.updateState("Ready");
    p2.showProcess();

    p2.updateState("Running");
    p2.showProcess();
    p2.decrementRemainingTime(5);
    currentTime += 5;
    p2.setTurnaroundTime(currentTime);
    p2.showProcess();

    return 0;
}
