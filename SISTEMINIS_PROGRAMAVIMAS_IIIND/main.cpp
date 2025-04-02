#include <windows.h>
#include <iostream>
#include <vector>
#include <queue>
#include <fstream>
#include <sstream>

CRITICAL_SECTION queueLock;
CONDITION_VARIABLE queueNotEmpty;
std::queue<int> numberQueue;
bool producerDone = false;
std::vector<HANDLE> consumers;
int consumerCount = 5;

bool isPrime(int num) {
    if (num < 2) return false;
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return false;
    }
    return true;
}

DWORD WINAPI consumerThread(LPVOID) {
    while (true) {
        EnterCriticalSection(&queueLock);
        while (numberQueue.empty() && !producerDone) {
            SleepConditionVariableCS(&queueNotEmpty, &queueLock, INFINITE);
        }
        if (numberQueue.empty() && producerDone) {
            LeaveCriticalSection(&queueLock);
            break;
        }
        int num = numberQueue.front();
        numberQueue.pop();
        LeaveCriticalSection(&queueLock);

        if (isPrime(num)) {
            EnterCriticalSection(&queueLock);
            std::cout << "Pirminis skaičius: " << num << std::endl;
            LeaveCriticalSection(&queueLock);
        }
    }
    return 0;
}

DWORD WINAPI producerThread(LPVOID param) {
    std::string directory = *(std::string*)param;
    delete (std::string*)param;

    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((directory + "*.txt").c_str(), &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) return 0;

    do {
        std::ifstream file(directory + "/" + findFileData.cFileName);
        std::string line;
        while (getline(file, line)) {
            std::stringstream ss(line);
            int num;
            while (ss >> num) {
                EnterCriticalSection(&queueLock);
                numberQueue.push(num);
                LeaveCriticalSection(&queueLock);
                WakeConditionVariable(&queueNotEmpty);
            }
        }
    } while (FindNextFile(hFind, &findFileData));
    FindClose(hFind);

    EnterCriticalSection(&queueLock);
    producerDone = true;
    LeaveCriticalSection(&queueLock);
    WakeAllConditionVariable(&queueNotEmpty);

    return 0;
}

void addConsumerThread() {
    HANDLE newConsumer = CreateThread(NULL, 0, consumerThread, NULL, 0, NULL);
    consumers.push_back(newConsumer);
    consumerCount++;
}

void removeConsumerThread() {
    if (!consumers.empty()) {
        TerminateThread(consumers.back(), 0);
        CloseHandle(consumers.back());
        consumers.pop_back();
        consumerCount--;
    }
}

int main() {
    InitializeCriticalSection(&queueLock);
    InitializeConditionVariable(&queueNotEmpty);

    std::string directory = "C:/Users/pkrin/CLionProjects/SISTEMINIS_PROGRAMAVIMAS_IIIND/rand_files/";
    HANDLE producer = CreateThread(NULL, 0, producerThread, new std::string(directory), 0, NULL);

    for (int i = 0; i < consumerCount; i++) {
        consumers.push_back(CreateThread(NULL, 0, consumerThread, NULL, 0, NULL));
    }

/*
    std::string command;
    while (true) {
        std::cout << "Enter command (add/remove/exit): ";
        std::cin >> command;
        if (command == "add") {
            addConsumerThread();
            std::cout << "Added a consumer. Total: " << consumerCount << std::endl;
        } else if (command == "remove") {
            if (consumerCount > 1) {
                removeConsumerThread();
                std::cout << "Removed a consumer. Total: " << consumerCount << std::endl;
            } else {
                std::cout << "Cannot remove last consumer!" << std::endl;
            }
        } else if (command == "exit") {
            break;
        }
    }
*/

    WaitForSingleObject(producer, INFINITE);
    for (HANDLE h : consumers) WaitForSingleObject(h, INFINITE);
    DeleteCriticalSection(&queueLock);
    return 0;
}
