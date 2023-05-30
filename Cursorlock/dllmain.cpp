// dllmain.cpp : Defines the entry point for the DLL application.

#include <string>
#include "MinHook.h"
#include <vector>
#include <iostream>
#include "GameCommons.hpp"

HMODULE DllHandle = NULL;
HWND g_HWND = NULL;
HHOOK wndHook = NULL;
bool active = false;


// For iterating through Windows
BOOL CALLBACK EnumWindowsProcMy(HWND hwnd, LPARAM lParam)
{
    DWORD lpdwProcessId;
    GetWindowThreadProcessId(hwnd, &lpdwProcessId);
    if (lpdwProcessId == lParam)
    {
        g_HWND = hwnd;
        return FALSE;
    }
    return TRUE;
}

void UpdateCursorRegionInit(HWND hwnd)
{
    // Retrieve the window dimensions
    RECT rect;
    GetClientRect(hwnd, &rect);

    // Set the window region to limit cursor movement within the window
    HRGN hRgn = CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
    SetWindowRgn(hwnd, hRgn, TRUE);
}


void ShowNotification(HWND hWnd, std::string szTip, std::string szInfoTitle, std::string szInfo)
{
    NOTIFYICONDATA nid = { sizeof(nid) };
    nid.hWnd = hWnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    //nid.uCallbackMessage = WM_USER_SHELLICON;
    nid.hIcon = NULL;
    lstrcpy(nid.szTip, (szTip.c_str()));

    // Add the notification icon to the system tray
    Shell_NotifyIcon(NIM_ADD, &nid);

    // Display the notification
    Shell_NotifyIcon(NIM_MODIFY, &nid);

    nid.uFlags = NIF_INFO;
    nid.dwInfoFlags = NIIF_INFO;
    lstrcpy(nid.szInfoTitle, (szInfoTitle.c_str()));
    lstrcpy(nid.szInfo, (szInfo.c_str()));
    Shell_NotifyIcon(NIM_MODIFY, &nid);
}

// Let my own proc cook
LRESULT CALLBACK HookedWindowProc(int nCode, WPARAM wParam, LPARAM lParam)
{

    if (active)
    {
        UpdateCursorRegionInit(g_HWND);

        POINT ptCursor;
        GetCursorPos(&ptCursor);
        ScreenToClient(g_HWND, &ptCursor);

        RECT rect;
        GetClientRect(g_HWND, &rect);

        if (ptCursor.x < rect.left)
            ptCursor.x = rect.left;
        else if (ptCursor.x > rect.right)
            ptCursor.x = rect.right;

        if (ptCursor.y < rect.top)
            ptCursor.y = rect.top;
        else if (ptCursor.y > rect.bottom)
            ptCursor.y = rect.bottom;

        SetCursorPos(ptCursor.x, ptCursor.y);

    }
    
    return CallNextHookEx(NULL, nCode, wParam, lParam);
    
}


void UnhookWindowProcedure()
{
    if (wndHook != nullptr)
    {
        // Remove the hook
        
        UnhookWindowsHookEx(wndHook);
        wndHook = nullptr;
    }
}



int AsyncMain()
{

    MH_STATUS status = MH_Initialize();
    if (status != MH_OK)
    {
        return 1;
    }

    while (true)
    {
        Sleep(50);
        EnumWindows(EnumWindowsProcMy, GetCurrentProcessId()); // Init gHwnd

        if (GetAsyncKeyState(VK_F3))
        {
            active = !active;
            //ShowNotification(g_HWND, "Active", "Active", "Active");
            Sleep(300);
        }

        if (GetAsyncKeyState(VK_F4))
        {
            active = false;
            UnhookWindowProcedure();
            Sleep(300);
        }

    }
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DllHandle = hModule;

        EnumWindows(EnumWindowsProcMy, GetCurrentProcessId()); // Init gHwnd

        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AsyncMain, NULL, 0, NULL); // Menu thread
        
        wndHook = SetWindowsHookExW(WH_CALLWNDPROC, HookedWindowProc, NULL, GetCurrentThreadId());
        if (wndHook == NULL)
        {
            MessageBoxA(g_HWND, "Failed", "failed", 0);
        }
        else
        {
            MessageBoxA(g_HWND, "Controls: F3: Toggle cursorlock, F4: Unload", "OK", 0);
        }

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

