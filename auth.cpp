#include "auth.h"
#include <iostream>
#include <fstream>

using namespace std;

// Function to authenticate a user from the users.txt file with only 3 attempts
bool authenticateUser() {
    string username;
    string password;
    int attempts = 3;

    while (attempts > 0) {
        cout << "Please enter username: ";
        cin >> username;

        cout << "Please enter password: ";
        cin >> password;

        ifstream file("users.txt");
        string savedUsername, savedPassword;

        while (file >> savedUsername >> savedPassword) { // Read username and password from users.txt
            if (username == savedUsername && password == savedPassword) { // Check if they're an exact match
                cout << "Login successful!\n";
                return true;
            }
        }

        attempts--; // Decrease attempts if username or password are still incorrect
        cout << "Invalid: Wrong username or password. Attempts left: " << attempts << "\n";
    }

    cout << "Too many failed attempts. Access denied.\n";
    return false;
}


// Function to add user to the users.txt file acting as a database
void addUser(string username, string password) {
    ofstream file("users.txt", ios::app);
    file << username << " " << password << "\n"; // Write the new username and password to the file
    cout << "New user added! Welcome\n";
}
