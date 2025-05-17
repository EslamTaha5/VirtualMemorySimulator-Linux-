//
// Created by eslam on 5/1/25.
//

#include "VMAlgorithms.h"

void VMAlgorithms::showProcessStatus(int pid) {
    if (processes.find(pid) == processes.end()) {
        printf("Process %d does not exist.\n", pid);
        return;
    }
    printf("\n============== Status Of Process(%d) ==============\n", pid);


    for (auto &page : processes[pid]) {
        printf("Page %d: ", page->idx + 1);
        if (page->valid) {
            printf("In Memory, Frame = %d\n", page->frame);
        } else {
            printf("Not In Memory\n");
        }
    }

    printf("===================================================\n");
}

// FIFO Algorithm
FIFO::FIFO() : VMAlgorithms() {
}


void FIFO::insertProcess(int pid, int numberOfPages) {
    if (processes.find(pid) != processes.end()) {
        printf("Process %d already exists\n", pid);
        return;
    }
    vector<PageEntry *> pages(numberOfPages);
    for (int i = 0; i < numberOfPages; i++) {
        pages[i] = new PageEntry();
        pages[i]->pid = pid;
        pages[i]->idx = i;
    }
    processes[pid] = pages;
    printf("Process %d inserted\n", pid);
}

void FIFO::accessPage(int pid, int page) {
    if (processes.find(pid) == processes.end()) {
        printf("Process %d does not exist\n", pid);
        return;
    }
    page--;
    auto &processPages = processes[pid];
    if (page < 0 || page >= processPages.size()) {
        printf("Page %d is out of range for process %d\n", page + 1, pid);
        return;
    }
    if (processPages[page]->valid) {
        printf("Process %d: Page %d already exists in frame %d\n", pid, page + 1, processPages[page]->frame);
        return;
    }
    int frame = fullFrames + 1;
    if (fullFrames == totalFrames) {
        frame = chooseVictim();
    }
    fullFrames = std::min(fullFrames + 1, totalFrames);
    auto pg = processPages[page];
    pg->frame = frame;
    pg->valid = true;
    q.push(pg);
    printf("Process %d: Page %d inserted into frame %d!\n", pid, pg->idx + 1, frame);
}

int FIFO::chooseVictim() {
    auto entry = q.front();
    q.pop();
    int cur_frame = entry->frame;
    entry->frame = -1;
    entry->valid = false;
    printf("Page number %d of process %d has been swapped out!\n", entry->idx, entry->pid);
    return cur_frame;
}

void FIFO::deleteProcess(int pid) {
    if (processes.find(pid) == processes.end()) {
        printf("Process %d does not exist\n", pid);
        return;
    }

    queue<PageEntry*> newQueue;
    while (!q.empty()) {
        PageEntry* entry = q.front();
        q.pop();
        if (entry->pid != pid) {
            newQueue.push(entry);
        } else {
            if (entry->valid) fullFrames--;
        }
    }
    q = newQueue;

    auto& pages = processes[pid];
    for (auto page : pages) delete page;
    processes.erase(pid);

    printf("Process %d deleted\n", pid);
}

// LRU Algorithm

LRU::LRU() : VMAlgorithms() {
}

void LRU::insertProcess(int pid, int numberOfPages) {
    if (processes.find(pid) != processes.end()) {
        printf("Process %d already exists\n", pid);
        return;
    }
    vector<PageEntry *> pages(numberOfPages);
    for (int i = 0; i < numberOfPages; i++) {
        pages[i] = new PageEntry();
        pages[i]->pid = pid;
        pages[i]->idx = i;
    }
    processes[pid] = pages;
    printf("Process %d inserted\n", pid);
}

void LRU::accessPage(int pid, int page) {
    if (processes.find(pid) == processes.end()) {
        printf("Process %d does not exist\n", pid);
        return;
    }
    page--;
    auto &processPages = processes[pid];
    if (page < 0 || page >= processPages.size()) {
        printf("Page %d is out of range for process %d\n", page + 1, pid);
        return;
    }
    auto pg = processPages[page];
    if (pg->valid) {
        lruList.remove(pg);
        lruList.push_front(pg);
        printf("Process %d: Page %d already exists in frame %d\n", pid, page + 1, pg->frame);
        return;
    }
    int frame = fullFrames + 1;
    if (fullFrames == totalFrames) {
        frame = chooseVictim();
    }
    fullFrames = std::min(fullFrames + 1, totalFrames);
    pg->frame = frame;
    pg->valid = true;
    lruList.push_front(pg);
    printf("Process %d: Page %d inserted into frame %d!\n", pid, pg->idx + 1, frame);
}


int LRU::chooseVictim() {
    if (lruList.empty()) return -1;
    auto entry = lruList.back();
    lruList.pop_back();
    int cur_frame = entry->frame;
    entry->frame = -1;
    entry->valid = false;
    printf("Page number %d of process %d has been swapped out!\n", entry->idx + 1, entry->pid);
    return cur_frame;
}

void LRU::deleteProcess(int pid) {
    if (processes.find(pid) == processes.end()) {
        printf("Process %d does not exist\n", pid);
        return;
    }

    auto it = lruList.begin();
    while (it != lruList.end()) {
        if ((*it)->pid == pid) {
            if ((*it)->valid) fullFrames--;
            it = lruList.erase(it);
        } else {
            ++it;
        }
    }

    auto& pages = processes[pid];
    for (auto page : pages) delete page;
    processes.erase(pid);

    printf("Process %d deleted\n", pid);
}
// MRU Algorithm
MRU::MRU() : VMAlgorithms() {}

void MRU::insertProcess(int pid, int numberOfPages) {
    if (processes.find(pid) != processes.end()) {
        printf("Process %d already exists\n", pid);
        return;
    }
    vector<PageEntry *> pages(numberOfPages);
    for (int i = 0; i < numberOfPages; i++) {
        pages[i] = new PageEntry();
        pages[i]->pid = pid;
        pages[i]->idx = i;
    }
    processes[pid] = pages;
    printf("Process %d inserted\n", pid);
}

void MRU::accessPage(int pid, int page) {
    if (processes.find(pid) == processes.end()) {
        printf("Process %d does not exist\n", pid);
        return;
    }
    page--;
    auto &processPages = processes[pid];
    if (page < 0 || page >= processPages.size()) {
        printf("Page %d is out of range for process %d\n", page + 1, pid);
        return;
    }
    auto pg = processPages[page];
    if (pg->valid) {
        mruList.remove(pg);
        mruList.push_front(pg);
        printf("Process %d: Page %d already exists in frame %d\n", pid, page + 1, pg->frame);
        return;
    }
    int frame = fullFrames + 1;
    if (fullFrames == totalFrames) {
        frame = chooseVictim();
    }
    fullFrames = std::min(fullFrames + 1, totalFrames);
    pg->frame = frame;
    pg->valid = true;
    mruList.push_front(pg);
    printf("Process %d: Page %d inserted into frame %d!\n", pid, pg->idx + 1, frame);
}


int MRU::chooseVictim() {
    if (mruList.empty()) return -1;
    auto victim = mruList.front();
    mruList.pop_front();
    victim->valid = false;
    int frame = victim->frame;
    victim->frame = -1;
    printf("Page number %d of process %d has been swapped out!\n", victim->idx + 1, victim->pid);
    return frame;
}

void MRU::deleteProcess(int pid) {
    if (processes.find(pid) == processes.end()) {
        printf("Process %d does not exist\n", pid);
        return;
    }

    auto it = mruList.begin();
    while (it != mruList.end()) {
        if ((*it)->pid == pid) {
            if ((*it)->valid) fullFrames--;
            it = mruList.erase(it);
        } else {
            ++it;
        }
    }

    auto& pages = processes[pid];
    for (auto page : pages) delete page;
    processes.erase(pid);

    printf("Process %d deleted\n", pid);
}

// LFU Implementation
LFU::LFU() : VMAlgorithms() {}

void LFU::insertProcess(int pid, int numberOfPages) {
    if (processes.find(pid) != processes.end()) {
        printf("Process %d already exists\n", pid);
        return;
    }
    vector<PageEntry *> pages(numberOfPages);
    for (int i = 0; i < numberOfPages; i++) {
        pages[i] = new PageEntry();
        pages[i]->pid = pid;
        pages[i]->idx = i;
    }
    processes[pid] = pages;
    printf("Process %d inserted\n", pid);
}

void LFU::accessPage(int pid, int page) {
    static int accessTime = 0;
    if (processes.find(pid) == processes.end()) {
        printf("Process %d does not exist\n", pid);
        return;
    }
    page--;
    auto &processPages = processes[pid];
    if (page < 0 || page >= processPages.size()) {
        printf("Page %d is out of range for process %d\n", page + 1, pid);
        return;
    }
    auto pg = processPages[page];
    if (pg->valid) {
        pg->accessFrequency++;
        pg->lastAccessed = accessTime++;
        printf("Process %d: Page %d already exists in frame %d\n", pid, page + 1, pg->frame);
        return;
    }
    int frame = fullFrames + 1;
    if (fullFrames == totalFrames) {
        frame = chooseVictim();
    }
    fullFrames = std::min(fullFrames + 1, totalFrames);
    pg->frame = frame;
    pg->valid = true;
    pg->accessFrequency = 1;
    pg->lastAccessed = accessTime++;
    printf("Process %d: Page %d inserted into frame %d!\n", pid, pg->idx + 1, frame);
}
int LFU::chooseVictim() {
    vector<PageEntry *> validPages;
    for (auto &proc : processes) {
        for (auto pg : proc.second) {
            if (pg->valid) {
                validPages.push_back(pg);
            }
        }
    }
    if (validPages.empty()) return -1;

    PageEntry *victim = nullptr;
    int minFreq = INT_MAX;
    int oldest = INT_MAX;

    for (auto pg : validPages) {
        if (pg->accessFrequency < minFreq ||
           (pg->accessFrequency == minFreq && pg->lastAccessed < oldest)) {
            minFreq = pg->accessFrequency;
            oldest = pg->lastAccessed;
            victim = pg;
        }
    }

    if (victim) {
        victim->valid = false;
        int frame = victim->frame;
        victim->frame = -1;
        victim->accessFrequency = 0;
        printf("Page number %d of process %d has been swapped out!\n", victim->idx + 1, victim->pid);
        return frame;
    }
    return -1;
}

void LFU::deleteProcess(int pid) {
    if (processes.find(pid) == processes.end()) {
        printf("Process %d does not exist\n", pid);
        return;
    }

    auto& pages = processes[pid];
    for (auto page : pages) {
        if (page->valid) fullFrames--;
        delete page;
    }
    processes.erase(pid);

    printf("Process %d deleted\n", pid);
}

// CLOCK Implementation
CLOCK::CLOCK() : clockHand(0) {}

void CLOCK::insertProcess(int pid, int numberOfPages) {
    if (processes.find(pid) != processes.end()) {
        printf("Process %d already exists\n", pid);
        return;
    }
    vector<PageEntry *> pages(numberOfPages);
    for (int i = 0; i < numberOfPages; i++) {
        pages[i] = new PageEntry();
        pages[i]->pid = pid;
        pages[i]->idx = i;
    }
    processes[pid] = pages;
    printf("Process %d inserted\n", pid);
}

void CLOCK::accessPage(int pid, int page) {
    if (processes.find(pid) == processes.end()) {
        printf("Process %d does not exist\n", pid);
        return;
    }
    page--;
    auto &processPages = processes[pid];
    if (page < 0 || page >= processPages.size()) {
        printf("Page %d is out of range\n", page);
        return;
    }
    auto pg = processPages[page];
    if (pg->valid) {
        pg->req = 1; // Set reference bit
        printf("Page %d already exists in frame %d\n", page + 1, pg->frame);
        return;
    }
    int frame = fullFrames + 1;
    if (fullFrames == totalFrames) {
        frame = chooseVictim();
    }
    fullFrames = min(fullFrames + 1, totalFrames);
    pg->frame = frame;
    pg->valid = true;
    pg->req = 1;
    clockList.push_back(pg);
    printf("Page %d inserted into frame %d!\n", pg->idx + 1, frame);
}

int CLOCK::chooseVictim() {
    if (clockList.empty()) return -1;
    int start = clockHand;
    while (true) {
        if (clockHand >= clockList.size()) clockHand = 0;
        PageEntry *pg = clockList[clockHand];
        if (pg->req == 0) {
            pg->valid = false;
            int frame = pg->frame;
            pg->frame = -1;
            clockList.erase(clockList.begin() + clockHand);
            printf("Page number %d of process %d has been swapped out!\n", pg->idx, pg->pid);
            return frame;
        } else {
            pg->req = 0;
        }
        clockHand = (clockHand + 1) % clockList.size();
        if (clockHand == start) break;
    }
    // If all bits were 0, select the next page
    PageEntry *victim = clockList[clockHand];
    victim->valid = false;
    int frame = victim->frame;
    victim->frame = -1;
    clockList.erase(clockList.begin() + clockHand);
    printf("Page number %d of process %d has been swapped out!\n", victim->idx + 1, victim->pid);
    return frame;
}


void CLOCK::deleteProcess(int pid) {
    if (processes.find(pid) == processes.end()) {
        printf("Process %d does not exist\n", pid);
        return;
    }

    auto it = clockList.begin();
    while (it != clockList.end()) {
        if ((*it)->pid == pid) {
            if ((*it)->valid) fullFrames--;
            it = clockList.erase(it);
        } else {
            ++it;
        }
    }

    if (clockHand >= clockList.size()) clockHand = 0;

    auto& pages = processes[pid];
    for (auto page : pages) delete page;
    processes.erase(pid);

    printf("Process %d deleted\n", pid);
}