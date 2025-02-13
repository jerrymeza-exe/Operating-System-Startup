#include <iostream>
#include "auth.h"

using namespace std;

void systemBoot() {
    cout << "System Booting..." << endl;
    int i = 0;
    while (i <= 100) {
        cout << "Loading: " << i << "%" << endl;
        for (long j = 0; j < 1000000000; j++) { }
        i += 20;
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
        bool success = authenticateUser();
        if (success) {
            std::cout << "Welcome!\n";
        } else {
            std::cout << "Access denied.\n";
        }
    } else if (choice == 2) {
        string newUsername, newPassword;
        cout << "Enter a username: ";
        cin >> newUsername;
        cout << "Enter a password: ";
        cin >> newPassword;
        addUser(newUsername, newPassword);
    } else {
        std::cout << "Invalid Choice.\n";
    }

    return 0;
}
