#include "process.h"
#include <iostream>

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
    ioTime = 0;
    waitingSinceCycle = -1;
}

int Process::getPid() const {
    return pid;
}

int Process::getArrivalTime() const {
    return arrivalTime;
}

int Process::getBurstTime() const {
    return burstTime;
}

int Process::getPriority() const{
    return priority;
}

string Process::getState() const {
    return state;
}

int Process::getRemainingTime() const{
    return remainingTime;
}

int Process::getWaitingTime() const {
    return waitingTime;
}

int Process::getTurnaroundTime() const {
    return turnaroundTime;
}

int Process::getMemoryRequired() const{
    return memoryRequired;
}

bool Process::hasIOOperations() const{
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
    waitingTime = turnaroundTime - burstTime;
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

void Process::setIoTime(int time) {
    ioTime = time;
}

int Process::getIoTime() const {
    return ioTime;
}

void Process::decrementIoTime() {
    if (ioTime > 0) {
        ioTime--;
    }
}

void Process::setWaitingSinceCycle(int cycle) {
    waitingSinceCycle = cycle;
}

int Process::getWaitingSinceCycle() const {
    return waitingSinceCycle;
}

void Process::setPriority(int newPriority) {
    priority = newPriority;
}
