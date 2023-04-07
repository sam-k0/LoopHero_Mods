// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include <shellapi.h>
#include <string>
#include <TlHelp32.h>
#include <tchar.h> // _tcscmp
#include <vector>
#include <iostream>
#include "include/MinHook.h"
#include <d3d11.h>
#include <memory>


#pragma comment(lib, "d3d11.lib")

HINSTANCE DllHandle; // Self modhandle

// Window management
HWND g_HWND = NULL;
HHOOK wndHook = NULL;
LONG_PTR style = NULL;

//DX11 Hook
typedef long(__stdcall* present)(IDXGISwapChain*, UINT, UINT);
present p_present;
present p_present_target;
WNDPROC oWndProc;
bool init = false;
HWND window = NULL;
ID3D11Device* p_device = NULL;
ID3D11DeviceContext* p_context = NULL;
ID3D11RenderTargetView* mainRenderTargetView = NULL;
// Drawing onto the hook


// Convert a std::string to an LPCWSTR
#pragma region Helper
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

void SayHi()
{
    MessageBoxA(NULL, "Hi", "Hi", 0);
}
#pragma endregion Helper
// https://pastebin.com/N0ahjU2E
#pragma region Memory
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

void PatchGameVersion()
{
    //MessageBoxA(g_HWND, "Patcher", "Sus", 0);
    //1202590843
    DWORD pid = NULL;
    GetWindowThreadProcessId(g_HWND, &pid);

    HANDLE procHandle = NULL;
    procHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    if (procHandle == INVALID_HANDLE_VALUE || procHandle == NULL) { // error handling
        std::cout << "Failed to open process" << std::endl;
        return;
    }

    char gameName[] = "Loop Hero.exe";
    DWORD gameBaseAddress = GetModuleBaseAddress(_T(gameName), pid);

    AllocConsole();
    freopen("CONOUT$", "w", stdout);

    std::cout << "sus" << std::endl;
    DWORD offsetGameToBase = 0x0173EA8C;
    std::vector<DWORD> pointsOffsets{ 0xAC, 0x10, 0x54, 0xEB0 };

    DWORD baseAddr = NULL;

    ReadProcessMemory(procHandle, (LPVOID)(gameBaseAddress + offsetGameToBase), &baseAddr, sizeof(baseAddr), NULL);
    std::cout << "debugginfo: baseaddress = " << std::hex << baseAddr << std::endl;


    DWORD verAddr = baseAddr;
    for (int i = 0; i < pointsOffsets.size()-1; i++)
    {
        ReadProcessMemory(procHandle, (LPVOID)(verAddr + pointsOffsets.at(i)), &verAddr, sizeof(verAddr), NULL);
        std::cout << "debugginfo: Value at offset = " << std::hex << verAddr << std::endl;
    }
    verAddr += pointsOffsets.at(pointsOffsets.size() - 1);

    double newVersion = 1.59;
    WriteProcessMemory(procHandle, (LPVOID)(verAddr), (LPCVOID)&newVersion, 4, 0);
}
#pragma endregion Memory

#pragma region DX11Hook

bool get_present_pointer()
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = GetForegroundWindow();
    sd.SampleDesc.Count = 1;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    IDXGISwapChain* swap_chain;
    ID3D11Device* device;

    const D3D_FEATURE_LEVEL feature_levels[] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    if (D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        0,
        feature_levels,
        2,
        D3D11_SDK_VERSION,
        &sd,
        &swap_chain,
        &device,
        nullptr,
        nullptr) == S_OK)
    {
        void** p_vtable = *reinterpret_cast<void***>(swap_chain);
        swap_chain->Release();
        device->Release();
        //context->Release();
        p_present_target = (present)p_vtable[8];
        return true;
    }
    return false;
}

static long __stdcall detour_present(IDXGISwapChain* p_swap_chain, UINT sync_interval, UINT flags) {
    if (!init) {
        if (SUCCEEDED(p_swap_chain->GetDevice(__uuidof(ID3D11Device), (void**)&p_device)))
        {
            p_device->GetImmediateContext(&p_context);
            DXGI_SWAP_CHAIN_DESC sd;
            p_swap_chain->GetDesc(&sd);
            window = sd.OutputWindow;
            ID3D11Texture2D* pBackBuffer;
            p_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
            p_device->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
            pBackBuffer->Release();          
            
            

            init = true;
        }
        else
            return p_present(p_swap_chain, sync_interval, flags);
    }
    
    p_context->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
    return p_present(p_swap_chain, sync_interval, flags);
}

#pragma endregion DX11Hook


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
    

    if (!get_present_pointer())
    {
        return 1;
    }

    MH_STATUS status = MH_Initialize();
    if (status != MH_OK)
    {
        return 1;
    }

    if (MH_CreateHook(reinterpret_cast<void**>(p_present_target), &detour_present, reinterpret_cast<void**>(&p_present)) != MH_OK) {
        return 1;
    }

    if (MH_EnableHook(p_present_target) != MH_OK) {
        return 1;
    }

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
        if(GetAsyncKeyState(VK_F10))
        {
            //PatchGameVersion();
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


