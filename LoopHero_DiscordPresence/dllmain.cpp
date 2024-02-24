// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>

// YYTK is in this now
#include "MyHelper.h"
#include "LHCore.h"

// Plugin functionality
#include <fstream>
#include <iterator>
#include "discord_rpc.h"
#define _CRT_SECURE_NO_WARNINGS


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

YYTKStatus CodeExecuteCallback(YYTKCodeEvent* codeEvent, void*)
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
    // TODO: Find a way to dynamically check if core is present or nah
    //codeEvent->Call(selfInst, otherInst, codeObj, std::get<3>(codeEvent->Arguments()), std::get<4>(codeEvent->Arguments()));
    // alloc presence
    DiscordRichPresence presence;
    memset(&presence, 0, sizeof(presence));
    bool update = false;
    
    /*
        Check for room events
        room names:
            rm_titles - idk
            rm_creater_dialog - idk
            rm_priamidka - Honestly idk
            rm_load  - Behind the scenes loading?
            rm_intro - intro cutscene
            rm_camp  - the camp
            rm_tutor - the tutorial
            rm_game - the game room
            rm_cutscenes - probably for cutscenes
            rm_music_maker - Music maker
    */

    if (Misc::StringHasSubstr(codeObj->i_pName, "gml_Room_rm_intro_Create"))
    {
        Misc::Print("Entering intro...");
        presence.state = "Introduction";
        presence.details = "Cutscene";
        update = true;
    }
    else
    if (Misc::StringHasSubstr(codeObj->i_pName, "gml_Room_rm_camp_Create"))
    {
        Misc::Print("Entering camp...");
        presence.state = "Playing";
        presence.details = "In Camp";    
        update = true;

    }
    else
    if(Misc::StringHasSubstr(codeObj->i_pName, "gml_Room_rm_game_Create"))
    {
        Misc::Print("Entering battle room");
        presence.state = "Playing";
        presence.details = "On Expedition";
        update = true;
    }
    else
    if (Misc::StringHasSubstr(codeObj->i_pName, "gml_Room_rm_music_maker_Create"))
    {
        Misc::Print("Entering music maker");
        presence.state = "Music Maker";
        presence.details = "Vibing";
        update = true;
    }


    if (update)
    {
        presence.largeImageKey = "loop-hero-new-key-art-logo";
        presence.largeImageText = "Loop Hero modded";
        Discord_UpdatePresence(&presence);
        update = false;
    }
   
    return YYTK_OK;
}

bool CoreFoundCallback() // set hooks
{
    PluginAttributes_t* pluginAttributes = nullptr;
    if (PmGetPluginAttributes(gThisPlugin, pluginAttributes) == YYTK_OK)
    {
        PmCreateCallback(pluginAttributes, callbackAttr, reinterpret_cast<FNEventHandler>(CodeExecuteCallback), EVT_CODE_EXECUTE, nullptr);
    }

    DiscordEventHandlers handlers;
    memset(&handlers, 0, sizeof(handlers));
    handlers.ready = handleDiscordReady;
    handlers.disconnected = handleDiscordDisconnected;

    // Initialize the Discord RPC
    Discord_Initialize(APP_ID, &handlers, 1, NULL);

    // Set to default presence
    DiscordRichPresence presence;
    memset(&presence, 0, sizeof(presence));
    presence.details = "Main Menu";
    presence.state = "";
    presence.largeImageKey = "loop-hero-new-key-art-logo";
    presence.largeImageText = "Loop Hero modded";
    Discord_UpdatePresence(&presence);
    
    return true;
}

// Entry
DllExport YYTKStatus PluginEntry(
    YYTKPlugin* PluginObject // A pointer to the dedicated plugin object
)
{
   

    Misc::Print("Loading RPC - ver " + gVer);

    gThisPlugin = PluginObject;
    gThisPlugin->PluginUnload = PluginUnload;

    // Get info
    LHCore::ResolveCoreParams_t params;
    params.plugin = gThisPlugin;
    params.callback = CoreFoundCallback;

    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)LHCore::ResolveCore, (LPVOID)&params, 0, NULL); // Check if the Callback Core Module is loaded, and wait for it to load

    return YYTK_OK; // Successful PluginEntry.
}


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

