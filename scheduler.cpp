#include "scheduler.h"
#include "memory.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

bool compareArrivalTime(const Process& a, const Process& b) {
    return a.getArrivalTime() < b.getArrivalTime();
}

bool compareBurstTime(const Process& a, const Process& b) {
    return a.getBurstTime() < b.getBurstTime();
}

bool compareRemainingTime(const Process& a, const Process& b) {
    return a.getRemainingTime() < b.getRemainingTime();
}

void Scheduling::sortFcfs(vector<Process>& queue) {
    sort(queue.begin(), queue.end(), compareArrivalTime);
}

void Scheduling::sortSjf(vector<Process>& queue) {
    sort(queue.begin(), queue.end(), compareBurstTime);
}

void Scheduling::sortSjfPreemptive(vector<Process>& queue) {
    sort(queue.begin(), queue.end(), compareRemainingTime);
}

void Scheduling::moveAgingProcess(vector<Process>& fromQueue, vector<Process>& toQueue, int currentCycle, int newPriority) {
    size_t i = 0;
    while (i < fromQueue.size()) {
        if (fromQueue[i].getWaitingSinceCycle() != -1 &&
            currentCycle - fromQueue[i].getWaitingSinceCycle() >= 10) {

            int pid = fromQueue[i].getPid();
            fromQueue[i].setPriority(newPriority);
            fromQueue[i].setWaitingSinceCycle(currentCycle);
            toQueue.push_back(fromQueue[i]);

            cout << "[Cycle " << currentCycle << "] Process " << pid << " promoted to Priority " << newPriority << " due to aging." << endl;

            fromQueue.erase(fromQueue.begin() + i);
        } else {
            i++;
        }
    }
}

void Scheduling::runSimulation(int totalProcesses, bool preemptiveSjf) {

    srand(time(0));

    vector<Process> highQueue;
    vector<Process> midQueue;
    vector<Process> lowQueue;
    vector<Process> waitQueue;
    vector<Process> doneProcesses;

    VirtualMemory memoryManager;

    int cycle = 0;
    int maxCycles = rand() % 50 + 30;
    int procCount = 1;
    int randomInterval = rand() % 5 + 2;

    while (cycle < maxCycles || highQueue.size() > 0 || midQueue.size() > 0 || lowQueue.size() > 0 || waitQueue.size() > 0) {

        // I/O waiting queue handler
        size_t i = 0;
        while (i < waitQueue.size()) {
            waitQueue[i].decrementIoTime();
            if (waitQueue[i].getIoTime() == 0) {
                int pri = waitQueue[i].getPriority();
                waitQueue[i].updateState("Ready");
                waitQueue[i].setWaitingSinceCycle(cycle);

                if (pri == 1) {
                    highQueue.push_back(waitQueue[i]);
                } else if (pri == 2) {
                    midQueue.push_back(waitQueue[i]);
                } else {
                    lowQueue.push_back(waitQueue[i]);
                }

                cout << "[Cycle " << cycle << "] Process " << waitQueue[i].getPid()
                     << " returned to Ready Queue after I/O." << endl;

                waitQueue.erase(waitQueue.begin() + i);
            } else {
                i++;
            }
        }

        // Create new processes dynamically
        if (cycle % randomInterval == 0 && procCount <= totalProcesses) {
            int burstTime = rand() % 10 + 1;
            int priorityLevel = rand() % 3 + 1;
            int memoryNeeded = rand() % 1024 + 256;
            bool hasIO = rand() % 2;

            //Allocate memory
            if (!memoryManager.allocateMemory(procCount, memoryNeeded)) {
                cout << "[Cycle " << cycle << "] Skipping Process " << procCount << " due to memory allocation failure.\n";
                procCount++;
                cycle++;
                continue;
            }

            // Logical address translation after process creation
            int sampleLogicalAddress = rand() % memoryNeeded;
            memoryManager.translateAddress(procCount, sampleLogicalAddress);

            // Create and queue process
            Process p(procCount, cycle, burstTime, priorityLevel, memoryNeeded, hasIO);
            p.updateState("Ready");
            p.setWaitingSinceCycle(cycle);

            if (priorityLevel == 1) {
                highQueue.push_back(p);
            } else if (priorityLevel == 2) {
                midQueue.push_back(p);
            } else {
                lowQueue.push_back(p);
            }

            cout << "\n[Cycle " << cycle << "] Process " << procCount
                 << " created (Burst Time: " << burstTime
                 << ", Priority: " << priorityLevel
                 << ", I/O: " << (hasIO ? "Yes" : "No")
                 << ", Memory: " << memoryNeeded << " KB)." << endl;

            procCount++;
        }

        // Aging
        moveAgingProcess(lowQueue, midQueue, cycle, 2);
        moveAgingProcess(midQueue, highQueue, cycle, 1);

        // Sort queues
        sortFcfs(highQueue);
        sortFcfs(midQueue);
        if (preemptiveSjf) {
            sortSjfPreemptive(lowQueue);
        } else {
            sortSjf(lowQueue);
        }

        // Pick process from highest available queue
        Process* currentProc = nullptr;
        vector<Process>* currentQueue = nullptr;

        if (!highQueue.empty()) {
            currentProc = &highQueue.front();
            currentQueue = &highQueue;
        } else if (!midQueue.empty()) {
            currentProc = &midQueue.front();
            currentQueue = &midQueue;
        } else if (!lowQueue.empty()) {
            currentProc = &lowQueue.front();
            currentQueue = &lowQueue;
        }

        if (currentProc != nullptr) {
            if (currentProc->getState() != "Running") {
                currentProc->updateState("Running");
                currentProc->setWaitingSinceCycle(-1);
                cout << "[Cycle " << cycle << "] Process " << currentProc->getPid() << " is Running." << endl;
                if (currentProc->hasIOOperations()) {
                    cout << "This process has I/O operations." << endl;
                }
            }

            currentProc->decrementRemainingTime(1);

            if (currentProc->getRemainingTime() == 0) {
                currentProc->setTurnaroundTime(cycle + 1);
                currentProc->setWaitingTime(cycle + 1);
                currentProc->updateState("Terminated");
                cout << "[Cycle " << cycle + 1 << "] Process " << currentProc->getPid() << " Terminated." << endl;
                currentProc->showProcess();
                memoryManager.releaseMemory(currentProc->getPid()); // release memory on termination
                doneProcesses.push_back(*currentProc);
                currentQueue->erase(currentQueue->begin());
            } else if (currentProc->hasIOOperations()) {
                if ((cycle - currentProc->getArrivalTime()) % 3 == 0) {
                    currentProc->setIoTime(3);
                    currentProc->updateState("Waiting");
                    waitQueue.push_back(*currentProc);
                    currentQueue->erase(currentQueue->begin());
                    cout << "[Cycle " << cycle + 1 << "] Process " << currentProc->getPid()
                         << " moved to Waiting Queue for I/O." << endl;
                }
            }
        }

        memoryManager.showMemoryStatus();
        cycle++;
    }

    int totalTurnaroundTime = 0;
    int totalWaitingTime = 0;

    cout << "Processes Finished: " << doneProcesses.size() << endl;

    for (int i = 0; i < doneProcesses.size(); i++) {
        totalTurnaroundTime += doneProcesses[i].getTurnaroundTime();
        totalWaitingTime += doneProcesses[i].getWaitingTime();
    }

    if (!doneProcesses.empty()) {
        cout << "Average Turnaround Time: " << (float)totalTurnaroundTime / doneProcesses.size() << endl;
        cout << "Average Waiting Time: " << (float)totalWaitingTime / doneProcesses.size() << endl;
    } else {
        cout << "No processes were finished." << endl;
    }
}
