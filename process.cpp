// process.cpp
#include "process.h"
#include <iostream>

// Constructor
Process::Process(int id, int arrival, int burst, int priorityLevel, int memory, bool io) {
    pid = id;
    arrivalTime = arrival;
    burstTime = burst;
    priority = priorityLevel;
    state = "New";  
    remainingTime = burst;
    waitingTime = 0;
    turnaroundTime = 0;
    memoryRequired = memory;
    io_operations = io;
}

// Returns the process attributes
int Process::getPid() {
    return pid;
}

int Process::getArrivalTime() {
    return arrivalTime;
}

int Process::getBurstTime() {
    return burstTime;
}

int Process::getPriority() {
    return priority;
}

string Process::getState() {
    return state;
}

int Process::getRemainingTime() {
    return remainingTime;
}

int Process::getWaitingTime() {
    return waitingTime;
}

int Process::getTurnaroundTime() {
    return turnaroundTime;
}

int Process::getMemoryRequired() {
    return memoryRequired;
}

bool Process::hasIOOperations() {
    return io_operations;
}


// Update process state
void Process::updateState(string newState) {
    state = newState;
}

// Decrease remaining time and update turnaround time and terminate if its 0
void Process::decrementRemainingTime(int timeSlice) {
    if (remainingTime > 0) {
        remainingTime -= timeSlice;
        if (remainingTime <= 0) {
            remainingTime = 0;
            updateState("Terminated");
        }
    }
}

// Calculate waiting time for the process
void Process::setWaitingTime(int startTime) {
    waitingTime = startTime - arrivalTime;
}

// Calculate turnaround time 
void Process::setTurnaroundTime(int completionTime) {
    turnaroundTime = completionTime - arrivalTime;
}

void Process::showProcess() {
    cout << "Process ID: " << pid << endl;
    cout << "State: " << state << endl;
    cout << "Remaining Time: " << remainingTime << endl;
    cout << "Waiting Time: " << waitingTime << endl;
    cout << "Turnaround Time: " << turnaroundTime << endl;
    cout << "Memory Required: " << memoryRequired << endl;
    cout << "I/O Operations: " << (io_operations ? "Yes" : "No") << endl;
    cout << "----------------------------------" << endl;
}
