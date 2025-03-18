
#ifndef SCHEDULING_H
#define SCHEDULING_H

#include "process.h"
#include <vector>

using namespace std;

class Scheduling {

public:
    void sortFcfs(vector<Process>& queue);
    void sortSjf(vector<Process>& queue);
    void sortSjfPreemptive(vector<Process>& queue);
    void runSimulation(int totalProcesses, bool preemptiveSjf);
    void moveAgingProcess(vector<Process>& fromQueue, vector<Process>& toQueue, int currentCycle, int newPriority);
};

#endif
