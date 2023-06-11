// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>

// YYTK is in this now
#include "MyHelper.h"

// Plugin functionality
#include <fstream>
#include <iterator>
#include "discord_rpc.h"
#define _CRT_SECURE_NO_WARNINGS

#define NODEBUG

const char* APP_ID = "1117388622975471739";

void handleDiscordReady(const DiscordUser* connectedUser)
{
    printf("Discord: connected to user %s#%s\n",
        connectedUser->username,
        connectedUser->discriminator);
}

void handleDiscordDisconnected(int errcode, const char* message)
{
    printf("Discord: disconnected (%d: %s)\n", errcode, message);
}

// Unload
YYTKStatus PluginUnload()
{
    Discord_Shutdown();

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

    
    if (My::StringHasSubstr(codeObj->i_pName, "gml_Room_rm_camp_Create"))
    {
        My::Print("Entering camp...");
        DiscordRichPresence presence;
        memset(&presence, 0, sizeof(presence));
        presence.state = "Playing";
        presence.details = "In Camp";
        presence.largeImageKey = "loop-hero-new-key-art-logo";
        presence.largeImageText = "Loop Hero modded";
        Discord_UpdatePresence(&presence);
    }
    else if(My::StringHasSubstr(codeObj->i_pName, "gml_Room_rm_game_Create"))
    {
        My::Print("Entering battle room");
        DiscordRichPresence presence;
        memset(&presence, 0, sizeof(presence));
        presence.state = "Playing";
        presence.details = "On Expedition";
        presence.largeImageKey = "loop-hero-new-key-art-logo";
        presence.largeImageText = "Loop Hero modded";
        Discord_UpdatePresence(&presence);
    }
   
    return YYTK_OK;
}


// Entry
DllExport YYTKStatus PluginEntry(
    YYTKPlugin* PluginObject // A pointer to the dedicated plugin object
)
{
    My::Print("Loading RPC");

    gThisPlugin = PluginObject;
    gThisPlugin->PluginUnload = PluginUnload;

   PluginAttributes_t* pluginAttributes = nullptr;
    if (PmGetPluginAttributes(gThisPlugin, pluginAttributes) == YYTK_OK)
    {
        PmCreateCallback(pluginAttributes, callbackAttr, reinterpret_cast<FNEventHandler>(my_callback_fn), EVT_CODE_EXECUTE, nullptr);
    }

    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    handlers.ready = handleDiscordReady;
    handlers.disconnected = handleDiscordDisconnected;

    // Initialize the Discord RPC
    Discord_Initialize(APP_ID, &handlers, 1, NULL);


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

