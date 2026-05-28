#pragma once
#include <Windows.h>
#include "MyPlugin.h"

namespace LHCore {

    // Ready-check function
    typedef bool (*CoreReady)();
    CoreReady pCoreReady = nullptr;
    bool isCoreReady = false;

    // Saying hello to the core function
    typedef bool (*RegisterModule)(std::string, YYTKPlugin*);
    RegisterModule pRegisterModule = nullptr;

    // unregistering from the core function
    typedef bool (*UnregisterModule)(std::string);
    UnregisterModule pUnregisterModule = nullptr;

    typedef bool (*CoreFoundCallback_t)();

    typedef struct ResolveCoreParams_t {
        YYTKPlugin* plugin;
        CoreFoundCallback_t callback;
    } ResolveCoreParams_t;

    DWORD WINAPI ResolveCore(LPVOID lpParam)
    {
        ResolveCoreParams_t* params = static_cast<ResolveCoreParams_t*>(lpParam);
        YYTKPlugin* plugin = params->plugin;
        CoreFoundCallback_t callback = params->callback;
#ifdef DEBUG
        Misc::Print("Importing Core function");
#endif
        void* rawCoreReady;
        void* rawRegisterModule;
        void* rawUnregisterModule;
        while (true)
        {
            Sleep(10);
            // Loading ready function
            if (PmGetExported("CoreReady", rawCoreReady) == YYTK_OK)
            {
                pCoreReady = reinterpret_cast<CoreReady>(rawCoreReady);
                if (pCoreReady() == true)
                {
#ifdef DEBUG
                    Misc::Print("Core is present", CLR_GREEN);
#endif
                    isCoreReady = true;

                    // Loading register function
                    if (PmGetExported("RegisterModule", rawRegisterModule) == YYTK_OK)
                    {
                        pRegisterModule = reinterpret_cast<RegisterModule>(rawRegisterModule);
                        pRegisterModule(gPluginName, gThisPlugin);
#ifdef DEBUG
                        Misc::Print("Registered to Core", CLR_GREEN);
#endif
                        if (callback != NULL)
                        {
                            callback();
                        }
                    }

                    // Loading unregister function
                    if (PmGetExported("UnregisterModule", rawUnregisterModule) == YYTK_OK)
                    {
                        pUnregisterModule = reinterpret_cast<UnregisterModule>(rawUnregisterModule);
                    }

                    return TRUE;
                }
            }
            Misc::Print("Waiting for Core. Did you install LoopHeroCallbackCore.dll?", CLR_RED);
        }
    }

}