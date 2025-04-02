#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <windows.h>
using namespace std;

#define ID3V1_SIZE 128

struct MP3TAG {
    char tag[3];
    char title[30];
    char artist[30];
    char album[30];
    char year[4];
};

void viewAndChangeMetaData(const string& chFolderpath) {
    HANDLE hFind;
    WIN32_FIND_DATAA data2;


    hFind = FindFirstFileA(chFolderpath.c_str(), &data2);

    if (hFind == INVALID_HANDLE_VALUE) {
        cout << "FindFirstFileA error"  << GetLastError() << endl;
        exit(0);
    }

    do {
        if (!(data2.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            string fileName = data2.cFileName;
            cout << "Found MP3 file " << fileName  << endl;
            string filePath = "C:/Users/pkrin/CLionProjects/SISTEMINIS_IIND/New folder/" + fileName;


            HANDLE hFile = CreateFile(
                filePath.c_str(),
                GENERIC_READ | GENERIC_WRITE,
                0,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL
                );

            if (hFile == INVALID_HANDLE_VALUE) {
                cout << hFile << endl;
                cout << "CreateFile error " << std::to_string(GetLastError()) << endl;
                CloseHandle(hFile);
                exit(0);
            }

            LARGE_INTEGER fileSize;
            if (!GetFileSizeEx(hFile, &fileSize)) {
                cout << "GetFileSizeEx error" << GetLastError() << endl;
                CloseHandle(hFile);
                exit(0);
            }

            MP3TAG* id3Tag = (MP3TAG*)VirtualAlloc(NULL, ID3V1_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            if (!id3Tag) {
                cerr << "Memory allocation failed.\n";
                CloseHandle(hFile);
                continue;
            }

            SetFilePointer(hFile, -ID3V1_SIZE, NULL, FILE_END);


            unique_ptr<BYTE[]> fileBuffer = make_unique<BYTE[]>(fileSize.QuadPart);
            char newTitle[30], newArtist[30], newAlbum[30], newYear[4];


            DWORD bytesRead;
            if (!ReadFile(hFile, id3Tag, ID3V1_SIZE, &bytesRead, NULL) || bytesRead != ID3V1_SIZE) {
                cerr << "Failed to read ID3 tag.\n";
            } else {
                if (strncmp(id3Tag->tag, "TAG", 3) == 0) {
                    cout << "Current Title: " << id3Tag->title << endl;
                    cout << "Enter new Title: " << endl;
                    cin >> newTitle;
                    strncpy_s(id3Tag->title, newTitle, sizeof(id3Tag->title));
                    cout << "Current Artist: " << id3Tag->artist << endl;
                    cout << "Enter new Artist: " << endl;
                    cin >> newArtist;
                    strncpy(id3Tag->artist, newArtist, sizeof(id3Tag->artist));
                    cout << "Current Album: " << id3Tag->album << endl;
                    cout << "Enter new Album: " << endl;
                    cin >> newAlbum;
                    strncpy(id3Tag->album, newAlbum, sizeof(id3Tag->album));
                    cout << "Current Year: " << id3Tag->year << endl;
                    cout << "Enter new Year: " << endl;
                    cin >> newYear;
                    strncpy(id3Tag->year, newYear, sizeof(id3Tag->year));
                    SetFilePointer(hFile, -ID3V1_SIZE, NULL, FILE_END);

                    DWORD bytesWritten;
                    if (!WriteFile(hFile, id3Tag, ID3V1_SIZE, &bytesWritten, NULL) || bytesWritten != ID3V1_SIZE) {
                        cerr << "Failed to write ID3 tag.\n";
                    } else {
                        cout << "Updated Title Successfully!\n";
                    }
                } else {
                    cerr << "No ID3v1 tag found in this file.\n";
                }
            }

            VirtualFree(id3Tag, 0, MEM_RELEASE);

            CloseHandle(hFile);
        }
    } while (FindNextFileA(hFind, &data2));

    FindClose(hFind);
}

void viewMetaData(const string& chFolderpath) {

    HANDLE hFind;
    WIN32_FIND_DATAA data2;


    hFind = FindFirstFileA(chFolderpath.c_str(), &data2);

    if (hFind == INVALID_HANDLE_VALUE) {
        cout << "FindFirstFileA error"  << GetLastError() << endl;
        exit(0);
    }

    do {
        if (!(data2.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            string fileName = data2.cFileName;
            cout << "Found MP3 file " << fileName  << endl;
            string filePath = "C:/Users/pkrin/CLionProjects/SISTEMINIS_IIND/New folder/" + fileName;


            HANDLE hFile = CreateFile(
                filePath.c_str(),
                GENERIC_READ,
                0,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL
                );
            if (hFile == INVALID_HANDLE_VALUE) {
                cout << "CreateFile error" << GetLastError() << endl;
                CloseHandle(hFile);
                exit(0);
            }

            LARGE_INTEGER fileSize;
            if (!GetFileSizeEx(hFile, &fileSize)) {
                cout << "GetFileSizeEx error" << GetLastError() << endl;
                CloseHandle(hFile);
                exit(0);
            }

            MP3TAG* id3Tag = (MP3TAG*)VirtualAlloc(NULL, ID3V1_SIZE, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            if (!id3Tag) {
                cerr << "Memory allocation failed.\n";
                CloseHandle(hFile);
                continue;
            }

            SetFilePointer(hFile, -ID3V1_SIZE, NULL, FILE_END);


            unique_ptr<BYTE[]> fileBuffer = make_unique<BYTE[]>(fileSize.QuadPart);

            DWORD bytesRead;
            if (!ReadFile(hFile, id3Tag, ID3V1_SIZE, &bytesRead, NULL) || bytesRead != ID3V1_SIZE) {
                cerr << "Failed to read ID3 tag.\n";
            } else {
                if (strncmp(id3Tag->tag, "TAG", 3) == 0) {
                    cout << "Current Title: " << id3Tag->title << endl;
                    cout << "Current Artist: " << id3Tag->artist << endl;
                    cout << "Current Album: " << id3Tag->album << endl;
                    cout << "Current Year: " << id3Tag->year << endl;
                } else {
                    cerr << "No ID3v1 tag found in this file.\n";
                }
            }

            VirtualFree(id3Tag, 0, MEM_RELEASE);

            CloseHandle(hFile);
        }
    } while (FindNextFileA(hFind, &data2));

    FindClose(hFind);
}

int main() {
    string chFolderpath = "C:/Users/pkrin/CLionProjects/SISTEMINIS_IIND/New folder/*.mp3";
    int selection;
    cout << "Ką norite atlikti? " << endl;
    cout << "1. Peržiūrėti esamus meta duomenis. " << endl;
    cout << "2. Redaguoti esamus meta duomenis. " << endl;
    cin >> selection;

    switch (selection) {
        case 1:
            viewMetaData(chFolderpath);
        break;
        case 2:
            viewAndChangeMetaData(chFolderpath);
        break;
        default:
            cerr << "Invalid selection." << endl;
        break;
    }

    return EXIT_SUCCESS;
}