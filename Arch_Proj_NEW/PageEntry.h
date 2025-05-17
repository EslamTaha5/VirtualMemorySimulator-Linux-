//
// Created by eslam on 5/1/25.
//

#ifndef PAGEENTRY_H
#define PAGEENTRY_H

class PageEntry {
public:
    int valid, frame, req, lastAccessed, accessFrequency, pid, idx;

    PageEntry() {
        valid = req = lastAccessed = accessFrequency = pid = idx = 0;
        frame = -1;
    }
};

#endif //PAGEENTRY_H
