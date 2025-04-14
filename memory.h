#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include <unordered_map>
#include <iostream>
#include <list>

using namespace std;

// Each page is 4 KB
const int pageSize = 4;

// Physical memory size is 2 MB
const int memorySize = 2048;

// Max number of frames in physical memory
const int maxFrames = memorySize / pageSize;

// TLB size
const int tlbSize = 4; 

struct Page{
    int pid;
    int pageNum;
    int frameNum;
};

struct PageTableEntry{
    int frameNumber;
    bool valid;
};

struct TLBEntry{
    int pid;
    int pageNumber;
    int frameNumber;
};

class VirtualMemory{
private:
    // Simulated physical memory
    Page physicalMemory[maxFrames];
    // Array to track if a frame is occupied
    bool frameOccupied[maxFrames];
    // Page tables for each process
    unordered_map<int, vector<PageTableEntry> > pageTables;
    list<TLBEntry> tlb;
    // FIFO queue for page replacement
    list<Page> fifoQueue;

public:
    VirtualMemory();

    bool allocateMemory(int pid, int memorySize);
    void releaseMemory(int pid);
    int translateAddress(int pid, int logicalAddress);
    void showMemoryStatus();
    int usedFrames() const;

private:
    int checkTLB(int pid, int pageNumber);
    void updateTLB(int pid, int pageNumber, int frameNumber);
    int handlePageFault(int pid, int pageNumber);
};

#endif
