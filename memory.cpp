#include <iostream>
#include <vector>
#include <list>
#include <unordered_map>
#include "memory.h"  

using namespace std;

// This struct marks all physical frames as available
VirtualMemory::VirtualMemory() {
    for (int i = 0; i < maxFrames; i++) {
        frameOccupied[i] = false;
    }
}

// Allocate memory for a process with a given process ID and memory size
// It calculates the number of pages needed and allocates a free frame for each page
bool VirtualMemory::allocateMemory(int pid, int memorySize) {
    int pagesNeeded = (memorySize + pageSize - 1) / pageSize;
    vector<PageTableEntry> pageTable;

    // For each page needed find a free frame and create a page table entry
    for (int i = 0; i < pagesNeeded; i++) {
        int freeFrame = -1;

        // Look for a free frame by going over all the frames
        for (int j = 0; j < maxFrames; j++) {
            if (!frameOccupied[j]) {
                freeFrame = j;
                break;
            }
        }

        // If no free frame is found, print a message and return false
        if (freeFrame == -1) {
            cout << "Not enough physical memory for process " << pid << ".\n";
            return false;
        }

        // Create a new page and mark it in the physical memory
        Page page;
        page.pid = pid;
        page.pageNum = i;
        page.frameNum = freeFrame;
        physicalMemory[freeFrame] = page;
        frameOccupied[freeFrame] = true;
        // Adding page to FIFO queue
        fifoQueue.push_back(page);

        // Create a page table entry for the page
        PageTableEntry entry;
        entry.frameNumber = freeFrame;
        entry.valid = true;
        pageTable.push_back(entry);
    }

    // Save the page table for the process
    pageTables[pid] = pageTable;
    cout << "Logical address space created for Process " << pid << " with " << pagesNeeded << " pages.\n";
    return true;
}

// Release memory for the process with the given proccess id 
void VirtualMemory::releaseMemory(int pid) {
    // Go through all frames and mark the ones busy by this process as free
    for (int i = 0; i < maxFrames; i++) {
        if (frameOccupied[i] && physicalMemory[i].pid == pid) {
            frameOccupied[i] = false;
        }
    }

    // Remove pages from the FIFO queue that belong to the process
    list<Page> tempQueue;
    while (!fifoQueue.empty()) {
        Page currentPage = fifoQueue.front();
        fifoQueue.pop_front();
        if (currentPage.pid != pid) {
            tempQueue.push_back(currentPage);
        }
    }
    fifoQueue = tempQueue;

    // Remove the page table for the process
    pageTables.erase(pid);
    cout << "Released memory for Process " << pid << ".\n";

    // Remove TLB entries that belong to the process
    for (list<TLBEntry>::iterator it = tlb.begin(); it != tlb.end(); ) {
        if (it->pid == pid) {
            it = tlb.erase(it);
        } else {
            ++it;
        }
    }
}

// Check if a page translation for is available in the TLB
// If a hit is found, the entry is moved to the front
int VirtualMemory::checkTLB(int pid, int pageNumber) {
    // Loop over each TLB entry
    for (list<TLBEntry>::iterator it = tlb.begin(); it != tlb.end(); ++it) {
        if (it->pid == pid && it->pageNumber == pageNumber) {
            // Found a TLB hit
            TLBEntry hitEntry = *it;
            // Remove from current position
            tlb.erase(it);
            // Insert at the front
            tlb.push_front(hitEntry);
            cout << "TLB: Hit -> PID " << pid << ", Page " << pageNumber
                 << " -> Frame " << hitEntry.frameNumber << endl;
            return hitEntry.frameNumber;
        }
    }
    // Return -1 if there is no hit 
    return -1;
}

// Update the TLB with an entry for the given process, page, and frame
void VirtualMemory::updateTLB(int pid, int pageNumber, int frameNumber) {
    // First remove any existing entry for this page
    for (list<TLBEntry>::iterator it = tlb.begin(); it != tlb.end(); ++it) {
        if (it->pid == pid && it->pageNumber == pageNumber) {
            tlb.erase(it);
            break;
        }
    }

    // If the TLB is full then remove the last entry
    if (tlb.size() >= tlbSize) {
        tlb.pop_back();
    }

    // Create a new entry and add it to the front of the TLB
    TLBEntry newEntry;
    newEntry.pid = pid;
    newEntry.pageNumber = pageNumber;
    newEntry.frameNumber = frameNumber;
    tlb.push_front(newEntry);

    cout << "TLB: Updated -> PID " << pid << ", Page " << pageNumber
         << " -> Frame " << frameNumber << endl;
}

// Page fault handler
// Finds a free frame or evicts an old page using FIFO if needed 
int VirtualMemory::handlePageFault(int pid, int pageNumber) {
    int freeFrame = -1;

    // Find a free frame
    for (int i = 0; i < maxFrames; i++) {
        if (!frameOccupied[i]) {
            freeFrame = i;
            break;
        }
    }

    // If no free frame is available then evict the oldest page
    if (freeFrame == -1) {
        // Evict the page at the front of the FIFO queue
        Page evicted = fifoQueue.front();
        fifoQueue.pop_front();
        freeFrame = evicted.frameNum;
        frameOccupied[freeFrame] = false;
        // Mark the evicted page's page table entry as invalid
        vector<PageTableEntry>& evictedPageTable = pageTables[evicted.pid];
        if (evicted.pageNum < (int)evictedPageTable.size()) {
            evictedPageTable[evicted.pageNum].valid = false;
        }
        cout << "Page Replacement: Evicted PID " << evicted.pid << " Page " << evicted.pageNum
             << " from Frame " << freeFrame << endl;
    }

    // Allocate a new page in the chosen frame
    Page newPage;
    newPage.pid = pid;
    newPage.pageNum = pageNumber;
    newPage.frameNum = freeFrame;
    physicalMemory[freeFrame] = newPage;
    frameOccupied[freeFrame] = true;
    fifoQueue.push_back(newPage);

    // Update the process's page table
    vector<PageTableEntry>& pageTable = pageTables[pid];
    // Expand the page table if needed
    if (pageNumber >= (int)pageTable.size()) {
        PageTableEntry defaultEntry;
        defaultEntry.frameNumber = -1;
        defaultEntry.valid = false;
        pageTable.resize(pageNumber + 1, defaultEntry);
    }
    // Mark the new page as valid
    pageTable[pageNumber].frameNumber = freeFrame;
    pageTable[pageNumber].valid = true;

    cout << "Page Fault Resolved!: Loaded PID " << pid << " Page " << pageNumber
         << " into Frame " << freeFrame << ".\n";
    return freeFrame;
}

// Translate a logical address to a physical address for a process
int VirtualMemory::translateAddress(int pid, int logicalAddress) {
    int pageNumber = logicalAddress / pageSize;
    int offset = logicalAddress % pageSize;

    // First check if the address is in the TLB
    int frameFromTLB = checkTLB(pid, pageNumber);
    if (frameFromTLB != -1) {
        int physicalAddress = frameFromTLB * pageSize + offset;
        cout << "Translate: (TLB) PID " << pid << " -> Logical: " << logicalAddress
             << " -> Physical: " << physicalAddress << endl;
        return physicalAddress;
    }

    // If the page table does not exist then print an error
    if (pageTables.find(pid) == pageTables.end()) {
        cout << "Translate: Error: No page table found for Process " << pid << ".\n";
        return -1;
    }

    // Use the page table to translate the logical address
    vector<PageTableEntry>& pageTable = pageTables[pid];
    if (pageNumber >= (int)pageTable.size() || !pageTable[pageNumber].valid) {
        cout << "Page Fault: PID " << pid << " requested Page " << pageNumber << ", not in memory.\n";
        int freeFrame = handlePageFault(pid, pageNumber);
        updateTLB(pid, pageNumber, freeFrame);
        int physicalAddress = freeFrame * pageSize + offset;
        cout << "Translate: PID " << pid << " -> Logical: " << logicalAddress
             << " -> Physical: " << physicalAddress << " - Page Fault Handled!\n";
        return physicalAddress;
    }

    // Get frame number from the valid page table entry
    int frameNumber = pageTable[pageNumber].frameNumber;
    updateTLB(pid, pageNumber, frameNumber);

    int physicalAddress = frameNumber * pageSize + offset;
    cout << "Translate: PID " << pid << " -> Logical: " << logicalAddress
         << " -> Physical: " << physicalAddress << " (Frame: " << frameNumber
         << ", Offset: " << offset << ")\n";
    return physicalAddress;
}

// Count the number of frames currently in use
int VirtualMemory::usedFrames() const {
    int count = 0;
    for (int i = 0; i < maxFrames; i++) {
        if (frameOccupied[i]) {
            count++;
        }
    }
    return count;
}

// Display the current memory usage status
void VirtualMemory::showMemoryStatus() {
    static int lastShown = -1;
    int used = usedFrames();
    if (used != lastShown) {
        cout << "Frames used: " << used << "/" << maxFrames << endl;
        lastShown = used;
    }
}
