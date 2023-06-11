// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>

// YYTK is in this now
#include "MyHelper.h"

// Plugin functionality
#include <fstream>
#include <iterator>
#define _CRT_SECURE_NO_WARNINGS

//#define NODEBUG




// Unload
YYTKStatus PluginUnload()
{
    PmRemoveCallback(callbackAttr);

    return YYTK_OK;
}

YYTKStatus my_callback_fn(YYTKCodeEvent* codeEvent, void*)
{
    CCode* codeObj = std::get<CCode*>(codeEvent->Arguments());
    CInstance* selfInst = std::get<0>(codeEvent->Arguments());
    CInstance* otherInst = std::get<1>(codeEvent->Arguments());


    

    // If we have invalid data???
    if (!codeObj)
        return YYTK_INVALIDARG;

    if (!codeObj->i_pName)
        return YYTK_INVALIDARG;


    // Original event
    codeEvent->Call(selfInst, otherInst, codeObj, std::get<3>(codeEvent->Arguments()), std::get<4>(codeEvent->Arguments()));


    if (My::StringHasSubstr(codeObj->i_pName, "gml_Object_o_opt_full_button_Other_10"))
    {
        // Fullscreen toggle button has been pressed
        YYRValue isFullscreen;
        CallBuiltin(isFullscreen, "window_get_fullscreen", selfInst, otherInst, {});

        {
            My::Print("Changing to Borderless");
            YYRValue result;
            CallBuiltin(result, "window_handle", selfInst, otherInst, {});

            HWND window_handle = reinterpret_cast<HWND>(result.As<RValue>().Pointer);
           

            LONG lStyle = GetWindowLong(window_handle, GWL_STYLE);
            lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
            SetWindowLong(window_handle, GWL_STYLE, lStyle);
            LONG lExStyle = GetWindowLong(window_handle, GWL_EXSTYLE);
            lExStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
            SetWindowLong(window_handle, GWL_EXSTYLE, lExStyle);
            SetWindowPos(window_handle, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
        }
    }
    else if (My::StringHasSubstr(codeObj->i_pName, "gml_Room_rm_"))
    {
        My::AddToVectorNoDuplicates(codeObj->i_pName, &codeEventNames);
    }

   
    return YYTK_OK;
}


// Entry
DllExport YYTKStatus PluginEntry(
    YYTKPlugin* PluginObject // A pointer to the dedicated plugin object
)
{
    My::Print("To activate the borderless window, go to Settings->Fullscreen!");
    My::Print("The fullscreen button will now make the window borderless.");
    My::Print("Tip: To get borderless fullscreen, use the 'windowed' button options. (Windowed x 3 button)", CLR_GOLD);

    gThisPlugin = PluginObject;
    gThisPlugin->PluginUnload = PluginUnload;

    PluginAttributes_t* pluginAttributes = nullptr;
    if (PmGetPluginAttributes(gThisPlugin, pluginAttributes) == YYTK_OK)
    {
        PmCreateCallback(pluginAttributes, callbackAttr, reinterpret_cast<FNEventHandler>(my_callback_fn), EVT_CODE_EXECUTE, nullptr);
    }


    // Initialize the plugin, set callbacks inside the PluginObject.
    // Set-up buffers.
    return YYTK_OK; // Successful PluginEntry.
}
#ifndef NODEBUG
DWORD WINAPI Menu(HINSTANCE hModule)
{
    while (true)
    {
        Sleep(50);
        if (GetAsyncKeyState(VK_NUMPAD0))
        {
            My::Print("Dumping to file!");
            My::VectorToFile(&codeEventNames);
            Sleep(300);
        }
        if (GetAsyncKeyState(VK_NUMPAD1))
        {
            My::Print("Resetting event vector");
            codeEventNames.clear();
            Sleep(300);
        }
    }
}
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
#ifndef NODEBUG
        DllHandle = hModule;
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Menu, NULL, 0, NULL); // For the input
#endif
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

