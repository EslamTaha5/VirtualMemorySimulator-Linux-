#include <bits/stdc++.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

using namespace std;

const int BUFFERSIZE = 1024;
#define SHARED_MEM_NAME "/OS_MMU"  // Name for the shared memory object
#define SHARED_MEM_NAME2 "/OS_APP"  // Name for the shared memory object

vector<string> options{"Add New Process", "R/W Process"};

void printOptions() {
    for (int i = 1; i <= options.size(); i++) {
        cout << i << ": " << options[i - 1] << endl;
    }
}

int main(int argc, char *argv[]) {
    string algorithm = argv[1];

    int shm_fd = shm_open(SHARED_MEM_NAME, O_CREAT | O_RDWR, 0666);
    int shm_fd2 = shm_open(SHARED_MEM_NAME2, O_CREAT | O_RDWR, 0666);

    if (shm_fd == -1) {
        perror("shm_open1");
        return 1;
    }
    if(shm_fd2 ==-1){
        perror("shm_open2");
        return 1;
    }
    ftruncate(shm_fd, BUFFERSIZE);
    ftruncate(shm_fd2, BUFFERSIZE);

    void *ptr = mmap(0, BUFFERSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    void *ptr2 = mmap(0, BUFFERSIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd2, 0);
    int messageLength = 0;

    memset(ptr2, 0, sizeof(messageLength) + messageLength);

    if (ptr == MAP_FAILED) {
        perror("mmap1");
        return 1;
    }

    if (ptr2 == MAP_FAILED) {
        perror("mmap2");
        return 1;
    }

    cout << "=========== Virtual Memory Simulator (OS) ==================" << endl;
    auto writeToMemory = [&](const string &message)-> void {

        int message_length = message.length();
        memcpy(ptr, &message_length, sizeof(message_length));

        strcpy((char *) ptr + sizeof(message_length), message.c_str());

    };

    writeToMemory(algorithm);
    while (true) {
        memcpy(&messageLength, ptr2, sizeof(messageLength));

        if (messageLength > 0 && messageLength < BUFFERSIZE - sizeof(messageLength)) {

            char message[BUFFERSIZE];

            memcpy(message, (char *) ptr2 + sizeof(messageLength), messageLength);
            message[messageLength] = '\0'; // Ensure null termination

            string msg(message);

            cout << msg  << endl;

            istringstream msgStream(msg);
            string operation;
            msgStream >> operation;
            if(operation == "New"){
                string pid, pages;
                msgStream >> pid >> pid >> pages >> pages;
                
                string message = "I " + pid + " " + pages;
                writeToMemory(message);

            }else if (operation == "Acess"){
                string pid, page;
                msgStream >> pid >> pid >> page >> page >> page;
                string access_message = "R/W " +pid + " " + page;
                writeToMemory(access_message);
            }else if(operation == "Show"){
                string pid;
                for(int i = 0; i < 6; i++){
                    msgStream >> pid;
                }

                writeToMemory("S " + pid);
            }else{
                string pid;
                msgStream >> pid;
                string delProcess = "D " + pid;
                writeToMemory(msg);
            }
            memset(ptr2, 0, sizeof(messageLength) + messageLength);
        }
        
    }

    // Clean up
    munmap(ptr, BUFFERSIZE);
    close(shm_fd);
    return 0;
}
