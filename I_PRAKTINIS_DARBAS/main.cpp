#include <cmath>
#include <iostream>
#include <math.h>
#include <Windows.h>
#include <sstream>
#include <vector>


using namespace std;

void printf_function() {
    float number1, number2, number3;
    cin >> number1;
    cin >> number2;
    cin >> number3;

    printf("%.2f\n", number1);
    printf("%.1f\n", number2);
    printf("%.2e", number3);
    }

void DisplayErrorMessage(DWORD errorCode) {

    LPVOID lpMsgBuf;

    DWORD dwChars = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        errorCode,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPTSTR>(&lpMsgBuf),
        0,
        nullptr
    );

    if (dwChars == 0) {
        cerr << "No such error exists" << endl;
        return;
    }
    cout << "Message " << errorCode << ": " << reinterpret_cast<LPTSTR>(lpMsgBuf) << endl;

    LocalFree(lpMsgBuf);
}


char valuetochar(int v)
{
    char c;
    switch (v)
    {
        case 0: c='A';break;case 1: c='B';break;case 2: c='C';break;case 3: c='D';break;
        case 4: c='E';break;case 5: c='F';break;case 6: c='G';break;case 7: c='H';break;
        case 8: c='I';break;case 9: c='J';break;case 10: c='K';break;case 11: c='L';break;
        case 12: c='M';break;case 13: c='N';break;case 14: c='O';break;case 15: c='P';break;
        case 16: c='Q';break;case 17: c='R';break;case 18: c='S';break;case 19: c='T';break;
        case 20: c='U';break;case 21: c='V';break;case 22: c='W';break;case 23: c='X';break;
        case 24: c='Y';break;case 25: c='Z';break;case 26: c='a';break;case 27: c='b';break;
        case 28: c='c';break;case 29: c='d';break;case 30: c='e';break;case 31: c='f';break;
        case 32: c='g';break;case 33: c='h';break;case 34: c='i';break;case 35: c='j';break;
        case 36: c='k';break;case 37: c='l';break;case 38: c='m';break;case 39: c='n';break;
        case 40: c='o';break;case 41: c='p';break;case 42: c='q';break;case 43: c='r';break;
        case 44: c='s';break;case 45: c='t';break;case 46: c='u';break;case 47: c='v';break;
        case 48: c='w';break;case 49: c='x';break;case 50: c='y';break;case 51: c='z';break;
        case 52: c='0';break;case 53: c='1';break;case 54: c='2';break;case 55: c='3';break;
        case 56: c='4';break;case 57: c='5';break;case 58: c='6';break;case 59: c='7';break;
        case 60: c='8';break;case 61: c='9';break;case 62: c='+';break;case 63: c='/';break;
    }
    return c;
}

void base64_encode() {
    unsigned int padding = 0;
    char input[] = "Secure Programming";
    std::vector<int> v;
    for( unsigned int i=0; input[i] != '\0'; i+=3 )
    {
        v.push_back ((input[i] & 0xFC) >> 2);
        if(input[i+1] == '\0')
        {
            padding = 2;
            break;
        }
        v.push_back (((input[i] & 0x03) << 4) | ((input[i+1] & 0xF0) >> 4));
        if(input[i+2] == '\0')
        {
            padding = 1;
            break;
        }
        v.push_back (((input[i+1] & 0x0F) << 2) | ((input[i+2] & 0xC0) >> 6));
        if(input[i+2] == '\0')
        {
            padding = 1;
            break;
        }
        v.push_back (input[i+2] & 0x3F);
    }
    for( unsigned int i = 0; i < v.size(); i++) std::cout << valuetochar(v[i]);
    for( ;padding > 0; padding--) std::cout << "=" ;
    std::cout << std::endl;
}

void prime_checker(int n) {
    int count = 0;

    if (n >= 1)
       {

        for (int i = 1; i <= n; i++) {

            if (n % i == 0)
                count++;
        }

        if (count > 2)
            cout << "FALSE" << endl;

        else
            cout << "TRUE" << endl;
    }
}

int main() {

    int selection, number;
    SYSTEM_INFO systemInfo;
    int quote = 34;

    cout << "1. Formatting the entered number with printf." << endl;
    cout << "2. Displaying error messages using Windows API." << endl;
    cout << "3. Displaying system information." << endl;
    cout << "4. BASE64 computation for " << char(quote) << "Secure Programming" << char(quote) << " text."  << endl;
    cout << "5. Finding prime numbers." << endl;

    cin >> selection;

    switch (selection) {
        case 1:
            printf_function();
            break;
        case 2:
            while (true) {
                cin >> number;
                DisplayErrorMessage(number);
                if (number < 0){
                cout << "No such error exists" << endl;
                break;
            }
                }
        break;
        case 3:
            GetNativeSystemInfo(&systemInfo);
            cout << "Number of Logical Processors: " << systemInfo.dwNumberOfProcessors << endl;
            cout << "Page size: " << systemInfo.dwPageSize << " Bytes"<< endl;
            cout << "Processor Mask: " << systemInfo.dwActiveProcessorMask << endl;
            cout << "Minimum process address: " << systemInfo.lpMinimumApplicationAddress << endl;
            cout << "Maximum process address: " << systemInfo.lpMaximumApplicationAddress << endl;
            break;
        case 4:
                base64_encode();
            break;
        case 5:
        while (cin >> number) {
            prime_checker(number);
            if (number == 0) {
                break;
            }
        }
            break;
        default:
            break;
    }
    return 0;

}