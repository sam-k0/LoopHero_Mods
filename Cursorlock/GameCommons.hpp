#pragma once

#include <Windows.h>
#include <shellapi.h>
#include <TlHelp32.h>
#include <tchar.h> // _tcscmp
#include <memory>
#include <string>

LPCWSTR StringToLPCWSTR(const std::string& s)
{
    // Get the length of the wide-character string
    int length = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);

    // Allocate memory for the wide-character string
    LPWSTR wideStr = new WCHAR[length];

    // Convert the string to wide characters
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, wideStr, length);

    // Return a pointer to the wide-character string
    return wideStr;
}

DWORD GetModuleBaseAddress(TCHAR* lpszModuleName, DWORD pID) {
    DWORD dwModuleBaseAddress = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pID); // make snapshot of all modules within process
    MODULEENTRY32 ModuleEntry32 = { 0 };
    ModuleEntry32.dwSize = sizeof(MODULEENTRY32);

    if (Module32First(hSnapshot, &ModuleEntry32)) //store first Module in ModuleEntry32
    {
        do {
            if (_tcscmp(ModuleEntry32.szModule, lpszModuleName) == 0) // if Found Module matches Module we look for -> done!
            {
                dwModuleBaseAddress = (DWORD)ModuleEntry32.modBaseAddr;
                break;
            }
        } while (Module32Next(hSnapshot, &ModuleEntry32)); // go through Module entries in Snapshot and store in ModuleEntry32


    }
    CloseHandle(hSnapshot);
    return dwModuleBaseAddress;
}

