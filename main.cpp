#include <iostream>
#include "scheduler.h"
#include "process.h"
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

    int totalProcesses;
    cout << "\nEnter total number of processes to simulate: ";
    cin >> totalProcesses;

    Scheduling scheduler;
    scheduler.runSimulation(totalProcesses, true); //Set to false if we dont want to run preemptive SJF\

    return 0;
}
