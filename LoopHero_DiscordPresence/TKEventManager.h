#pragma once
#define YYSDK_PLUGIN
#include "SDK/SDK.hpp"
#include <vector>
#include <string>
#include <map>
#include <iostream>

typedef bool (*TKEventCallback)(CCode* CodeObj, CInstance* SelfInst, CInstance* OtherInst, void*);

class TKEventManager
{
private:
	// Member Variables
	std::map<std::string, std::vector<TKEventCallback>> mEventCallbackFunctions;
	std::string mPluginName;

	// Returns: Wether OutIterator is a valid iterator or .end()
	bool EventCallbackVecGetFunc(TKEventCallback TKECB, std::vector<TKEventCallback>::iterator& OutIterator, std::vector<TKEventCallback>& EventCallbacks);
	

public:
	// Public Member Vars
	// These have to be public due to how YYTK wants to modify them
	YYTKPlugin* mThisPlugin;
	CallbackAttributes_t* mCallbackAttributes = nullptr;
	PluginAttributes_t* mPluginAttributes = nullptr;

	// Public Functions
	bool RegisterEventCallback(std::string EventTypeStr, TKEventCallback FunctionCB);
	std::vector<TKEventCallback>* GetEventCallbacks(std::string EventTypeStr);
	// Constructor
	TKEventManager(std::string PluginName, YYTKPlugin* ThisPlugin, FNPluginUnload OnUnload);

	// Static Member
	static YYTKStatus Callback(YYTKCodeEvent* CodeEvent, void* Self);
	void Print(std::string Str, Color c = CLR_DEFAULT)
	{
		PrintMessage(c, (mPluginName + ":" + Str).c_str());
	}
};

