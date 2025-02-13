#include <iostream>
#include "auth.h"

using namespace std;

void systemBoot() {  // Simulates the system booting
    cout << "System Booting..." << endl;
    int i = 0;
    while (i <= 100) {  //Simulates a loading progress from 0% to 100%
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
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice == 1) {
        bool success = authenticateUser();  // Attempt to authenticate user
        if (success) {
            std::cout << "Welcome!\n";  // Login successful
        } else {
            std::cout << "Access denied.\n";  // Login failed
        }
    } else if (choice == 2) {
        string newUsername, newPassword;
        cout << "Enter a username: ";  // Prompt for new user credentials
        cin >> newUsername;
        cout << "Enter a password: ";
        cin >> newPassword;
        addUser(newUsername, newPassword);  //adds new user to the users.txt file
    } else {
        std::cout << "Invalid Choice.\n";
    }

    return 0;
}
