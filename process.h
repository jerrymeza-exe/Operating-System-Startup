// process.h
#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <string>

using namespace std;

class Process {
private:
    int pid;
    int arrivalTime;
    int burstTime;
    int priority;
    string state;
    int remainingTime;
    int waitingTime;
    int turnaroundTime;
    int memoryRequired;
    bool io_operations;

public:

    Process(int id, int arrival, int burst, int priorityLevel, int memory, bool io);

    int getPid();
    int getArrivalTime();
    int getBurstTime();
    int getPriority();
    string getState();
    int getRemainingTime();
    int getWaitingTime();
    int getTurnaroundTime();
    int getMemoryRequired();
    bool hasIOOperations();

    void setWaitingTime(int startTime);
    void setTurnaroundTime(int completionTime);

    void updateState(string newState); 
    void decrementRemainingTime(int timeSlice);
    void showProcess();
};

#endif // PROCESS_H
