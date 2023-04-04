// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include <shellapi.h>
#include <string>
HINSTANCE DllHandle; // Self modhandle

HWND g_HWND = NULL;
HHOOK wndHook = NULL;
LONG_PTR style = NULL;
// Convert a std::string to an LPCWSTR
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

void ShowNotification(HWND hWnd, std::string szTip, std::string szInfoTitle, std::string szInfo)
{
    NOTIFYICONDATA nid = { sizeof(nid) };
    nid.hWnd = hWnd;
    nid.uID = 1;
    nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    //nid.uCallbackMessage = WM_USER_SHELLICON;
    nid.hIcon = NULL;
    lstrcpy(nid.szTip, StringToLPCWSTR(szTip));

    // Add the notification icon to the system tray
    Shell_NotifyIcon(NIM_ADD, &nid);

    // Display the notification
    Shell_NotifyIcon(NIM_MODIFY, &nid);

    nid.uFlags = NIF_INFO;
    nid.dwInfoFlags = NIIF_INFO;
    lstrcpy(nid.szInfoTitle, StringToLPCWSTR(szInfoTitle));
    lstrcpy(nid.szInfo, StringToLPCWSTR(szInfo));
    Shell_NotifyIcon(NIM_MODIFY, &nid);
}

void SayHi()
{
    MessageBoxA(NULL, "Hi", "Hi", 0);
}

void SetWindowBorderless(HWND window_handle)
{
    LONG lStyle = GetWindowLong(window_handle, GWL_STYLE);
    lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
    SetWindowLong(window_handle, GWL_STYLE, lStyle);
    LONG lExStyle = GetWindowLong(window_handle, GWL_EXSTYLE);
    lExStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
    SetWindowLong(window_handle, GWL_EXSTYLE, lExStyle);
    SetWindowPos(window_handle, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
}

void SetWindowFullscreen(HWND window_handle)
{
    // Get the handle to the primary display monitor
    HMONITOR hMonitor = MonitorFromWindow(window_handle, MONITOR_DEFAULTTOPRIMARY);

    // Get the dimensions of the monitor
    MONITORINFO monitorInfo = { sizeof(monitorInfo) };
    GetMonitorInfo(hMonitor, &monitorInfo);
    int cxScreen = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
    int cyScreen = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;


    // Set the window position and size
    SetWindowPos(window_handle, HWND_TOP, 0, 0, cxScreen, cyScreen, SWP_SHOWWINDOW);
}

void EnableMaximize()
{
    style = GetWindowLongPtr(g_HWND, GWL_STYLE);
    style &= ~WS_DISABLED;      // Remove the WS_DISABLED style
    style |= WS_MAXIMIZEBOX;    // Add the WS_MAXIMIZEBOX style
    SetWindowLongPtr(g_HWND, GWL_STYLE, style);

    // activate btn
    HMENU hmenu = GetSystemMenu(g_HWND, FALSE);
    EnableMenuItem(hmenu, SC_MAXIMIZE, MF_BYCOMMAND | MF_ENABLED);
}

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

void RedrawWindow(HWND hwnd)
{
    SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

DWORD WINAPI Controls(HINSTANCE hModule)
{
    while (true)
    {
        Sleep(50);
        EnableMaximize();

        if (GetAsyncKeyState(VK_F11))
        {
            SetWindowFullscreen(g_HWND);
            SetWindowBorderless(g_HWND);
            RedrawWindow(g_HWND);
            ShowNotification(g_HWND, "Changed to borderless!", "Borderless","Changed the window to borderless fullscreen!");
            Sleep(300);
        }
    }
}

LRESULT CALLBACK HookProc(int code, WPARAM wParam, LPARAM lParam)
{
    if (code == HC_ACTION)
    {
        CWPSTRUCT* msg = reinterpret_cast<CWPSTRUCT*>(lParam);
        HWND hwnd = msg->hwnd;
        switch (msg->message)
        {
        case WM_SYSCOMMAND:

            if (msg->wParam == SC_MAXIMIZE)
            {
                SetWindowFullscreen(g_HWND);
                SetWindowBorderless(g_HWND);
                RedrawWindow(g_HWND);
                ShowNotification(g_HWND, "Loop Hero", "Changed Window", "Window set to borderless!");
            }
            break;
            // Handle other messages here
        }
    }
    return CallNextHookEx(NULL, code, wParam, lParam);
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

        EnumWindows(EnumWindowsProcMy, GetCurrentProcessId()); // Initialize gHwnd

        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Controls, NULL, 0, NULL); // For the input

        wndHook = SetWindowsHookEx(WH_CALLWNDPROC, HookProc, NULL, GetCurrentThreadId()); // Hooking the menu messages

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


