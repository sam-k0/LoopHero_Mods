// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#define YYSDK_PLUGIN // Declare the following code as a plugin
#include "SDK/SDK.hpp"

HINSTANCE DllHandle;

std::string gPluginName = "ModHero";
YYTKPlugin* gThisPlugin = nullptr;
CallbackAttributes_t* callbackAttr = nullptr;
std::vector<std::string> codeEventNames;

bool MyVectorContains(std::string find, std::vector<std::string>* v)
{
    if (std::find(v->begin(), v->end(), find) != v->end())
    {
        return true;
    }
    return false;
}

bool MyAddToVectorNoDuplicates(std::string str, std::vector<std::string>* v)
{
    if (MyVectorContains(str, v))
    {
        return false;
    }
    v->push_back(str);
    return true;
}

void MyPrint(std::string s, Color c = CLR_DEFAULT)
{
    PrintMessage(c, (gPluginName + ": " + s).c_str());
}

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

    // If we're not executing the relevant piece of code
    //if (!strstr(codeObj->i_pName, "obj_menu_WindowMode_Other_10"))
    //   return YYTK_UNAVAILABLE;
    
    // Add all event names to the vector without duplicates
    MyAddToVectorNoDuplicates(codeObj->i_pName, &codeEventNames);

    codeEvent->Call(selfInst, otherInst, codeObj, std::get<3>(codeEvent->Arguments()), std::get<4>(codeEvent->Arguments()));

    return YYTK_OK;
}


// Entry
DllExport YYTKStatus PluginEntry(
    YYTKPlugin* PluginObject // A pointer to the dedicated plugin object
)
{
    MyPrint("Hello! with sus");

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

DWORD WINAPI Menu(HINSTANCE hModule)
{
    while (true)
    {
        Sleep(50);
        if (GetAsyncKeyState(VK_NUMPAD0))
        {
            MyPrint("pressed!");
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
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Menu, NULL, 0, NULL);
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

