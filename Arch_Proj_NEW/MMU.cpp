#include <bits/stdc++.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include "VMAlgorithms.h"
using namespace std;

const int BUFFERSIZE = 1024;
const int numberOfFrames = 200;
#define SHARED_MEM_NAME "/OS_MMU"  // Name for the shared memory object

void setAlgo(const string &str, VMAlgorithms * &algo) {
    if (str == "FIFO") {
        algo = new FIFO();
    } else if (str == "LRU") {
        algo = new LRU();
    } else if (str == "MRU") {
        algo = new MRU();
    }else if (str == "LFU") {
        algo = new LFU();
    }else if (str == "CLOCK") {
        algo = new CLOCK();
    }
}

int main() {
    // Open the shared memory object
    int shm_fd = shm_open(SHARED_MEM_NAME, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return 1;
    }

    // Map the shared memory to the address space
    void *ptr = mmap(0, BUFFERSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        cout << "CANNOT" << endl;
        perror("mmap");
        return 1;
    }

    cout << "=========== Virtual Memory Simulator (MMU) ==================" << endl;
    bool f = true;
    VMAlgorithms *algo = nullptr;
    while (true) {
        // Check if there is a message available in shared memory
        int messageLength = 0;
        memcpy(&messageLength, ptr, sizeof(messageLength));

        if (messageLength > 0 && messageLength < BUFFERSIZE - sizeof(messageLength)) {
            char message[BUFFERSIZE];
            memcpy(message, (char *) ptr + sizeof(messageLength), messageLength);
            message[messageLength] = '\0'; // Ensure null termination

            if (f) {
                setAlgo(message, algo);
                cout << message << " Algorithm was chosen!" << endl;
                f = false;
            } else {
                string operation;
                int pid, page;
                istringstream is;
                is.str(message);
                is >> operation ;
                assert(algo);
                if (operation == "I") {
                    is >> pid >> page;
                    algo->insertProcess(pid, page);
                } else if (operation == "R/W") {
                    is >> pid >> page;
                    algo->accessPage(pid, page);
                }else if(operation == "S"){
                    is >> pid;
                    algo->showProcessStatus(pid);
                }else{

                    is >> pid;
                    algo->deleteProcess(pid);
                }
            }
            // Clear shared memory only after processing the message
            memset(ptr, 0, sizeof(messageLength) + messageLength);
        }

        usleep(100000); // Sleep for a short time to avoid busy-waiting
    }

    // Clean up
    munmap(ptr, BUFFERSIZE);
    close(shm_fd);
    return 0;
}
