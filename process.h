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
    int ioTime;
    int waitingSinceCycle;

public:

    Process(int id, int arrival, int burst, int priorityLevel, int memory, bool io);

    int getPid() const;
    int getArrivalTime() const;
    int getBurstTime() const;
    int getPriority() const;
    string getState() const;
    int getRemainingTime() const;
    int getWaitingTime() const;
    int getTurnaroundTime() const;
    int getMemoryRequired() const;
    bool hasIOOperations() const;

    void setWaitingTime(int startTime);
    void setTurnaroundTime(int completionTime);

    void updateState(string newState); 
    void decrementRemainingTime(int timeSlice);
    void showProcess();

    void setIoTime(int time);
    int getIoTime() const;
    void decrementIoTime();

    void setWaitingSinceCycle(int cycle);
    int getWaitingSinceCycle() const;
    void setPriority(int newPriority);

};

#endif // PROCESS_H
