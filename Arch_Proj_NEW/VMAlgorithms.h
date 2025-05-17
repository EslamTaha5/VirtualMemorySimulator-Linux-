//
// Created by eslam on 5/1/25.
//

#ifndef VMALGORITHMS_H
#define VMALGORITHMS_H
#include  <bits/stdc++.h>
using namespace std;
#include "PageEntry.h"
const int totalFrames = 5;

class VMAlgorithms {
public:
    virtual ~VMAlgorithms() = default;

    VMAlgorithms() = default;

    int fullFrames = 0;

    map<int, vector<PageEntry *> > processes;
    vector<PageEntry *> pagesInMemory;

    virtual void insertProcess(int pid, int numberOfPages) = 0;

    virtual void accessPage(int pid, int page) = 0;

    virtual int chooseVictim() = 0;
    void showProcessStatus(int pid);
};

class FIFO final : public VMAlgorithms {
    queue<PageEntry *> q;

public:
    FIFO();

    void insertProcess(int pid, int numberOfPages) override;

    void accessPage(int pid, int page) override;

    int chooseVictim() override;
    
};

class LRU final : public VMAlgorithms {
    list<PageEntry *> lruList; // To keep track of the order of page usage
public:
    LRU();

    void insertProcess(int pid, int numberOfPages) override;

    void accessPage(int pid, int page) override;

    int chooseVictim() override;


};

class MRU final : public VMAlgorithms {
    list<PageEntry *> mruList; // Track most recently used pages
public:
    MRU();
    void insertProcess(int pid, int numberOfPages) override;
    void accessPage(int pid, int page) override;
    int chooseVictim() override;

};

class LFU final : public VMAlgorithms {
public:
    LFU();
    void insertProcess(int pid, int numberOfPages) override;
    void accessPage(int pid, int page) override;
    int chooseVictim() override;


};

class CLOCK final : public VMAlgorithms {
    vector<PageEntry *> clockList; // Circular buffer of pages
    int clockHand; // Current position
public:
    CLOCK();
    void insertProcess(int pid, int numberOfPages) override;
    void accessPage(int pid, int page) override;
    int chooseVictim() override;


};


#endif //VMALGORITHMS_H
