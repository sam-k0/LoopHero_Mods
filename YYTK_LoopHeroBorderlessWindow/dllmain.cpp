// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>

// YYTK is in this now
#include "MyPlugin.h"
#include "Assets.h"
#include "LHSprites.h"
// Plugin functionality
#include <fstream>
#include <iterator>
#include "LHCore.h"

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
#define _CRT_SECURE_NO_WARNINGS

CallbackAttributes_t* CodeCallbackAttr;

// Unload function, remove callbacks here
YYTKStatus PluginUnload()
{
    LHCore::pUnregisterModule(gPluginName);
    return YYTK_OK;
}

// Window management
HWND g_HWND = NULL;
HHOOK wndHook = NULL;
LONG_PTR style = NULL;

typedef long(__stdcall* present)(IDXGISwapChain*, UINT, UINT);
present p_present;
present p_present_target;
WNDPROC oWndProc;
bool init = false;
HWND window = NULL;
ID3D11Device* p_device = NULL;
ID3D11DeviceContext* p_context = NULL;
ID3D11RenderTargetView* mainRenderTargetView = NULL;

DWORD WINAPI Controls(HINSTANCE hModule);
LRESULT CALLBACK HookProc(int code, WPARAM wParam, LPARAM lParam);

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


bool CoreFoundCallback() // This function is ran once the core is resolved
                         // In this case, we want to wait with registering code callback until the core has registered itself to ensure safe calling
{

    //EnumWindows(EnumWindowsProcMy, GetCurrentProcessId()); // Initialize gHwnd

    // Overwrite window handle
    //YYRValue winHwnd = Misc::CallBuiltin("window_handle", nullptr, nullptr, {});
    //const char* c = winHwnd;
    //g_HWND = (HWND)c;

    g_HWND = FindWindowA(NULL, "LOOP HERO 1.154 (win)");

    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Controls, NULL, 0, NULL); // For the input

    wndHook = SetWindowsHookEx(WH_CALLWNDPROC, HookProc, NULL, GetCurrentThreadId()); // Hooking the menu messages
    return true;
}

// Entry
DllExport YYTKStatus PluginEntry(
    YYTKPlugin* PluginObject // A pointer to the dedicated plugin object
)
{
    gThisPlugin = PluginObject;
    gThisPlugin->PluginUnload = PluginUnload;
    // Get info
    LHCore::ResolveCoreParams_t params;
    params.plugin = gThisPlugin;
    params.callback = CoreFoundCallback;

    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LHCore::ResolveCore, (LPVOID)&params, 0, NULL); // Check if the Callback Core Module is loaded, and wait for it to load
    return YYTK_OK; // Successful PluginEntry.
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
                
            }
            break;
            // Handle other messages here
        }
    }
    return CallNextHookEx(NULL, code, wParam, lParam);
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
            Sleep(300);
        }
        if (GetAsyncKeyState(VK_F10))
        {
            //PatchGameVersion();
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
        DllHandle = hModule; // save our module handle
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

