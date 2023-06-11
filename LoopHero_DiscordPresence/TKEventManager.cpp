#include "TKEventManager.h"
#include <fstream>
#include <iterator>


bool TKEventManager::EventCallbackVecGetFunc(TKEventCallback TKECB, std::vector<TKEventCallback>::iterator& OutIterator, std::vector<TKEventCallback>& EventCallbacks)
{
	OutIterator = std::find(EventCallbacks.begin(), EventCallbacks.end(), TKECB);// Try to find the iterator
	if (OutIterator == EventCallbacks.end())
	{
		return false;
	}
	return true;
}

std::vector<TKEventCallback>* TKEventManager::GetEventCallbacks(std::string EventTypeStr)
{
	auto it = mEventCallbackFunctions.find(EventTypeStr);
	if (it != mEventCallbackFunctions.end()) // Doesn't contain vector for callback
	{
		return &(it->second);
	}
	return nullptr;
}

bool TKEventManager::RegisterEventCallback(std::string EventTypeStr, TKEventCallback FunctionCB)
{
	// Set new callback function pointer
	auto it = mEventCallbackFunctions.find(EventTypeStr);

	if (it == mEventCallbackFunctions.end()) // Doesn't contain vector for callback
	{
		std::vector<TKEventCallback> ptrVec = std::vector<TKEventCallback>();
		ptrVec.push_back(FunctionCB);
		mEventCallbackFunctions.insert(std::make_pair(EventTypeStr, ptrVec));
	}
	else // Contains a vector
	{
		it->second.push_back(FunctionCB);
	}

	return true;
}


YYTKStatus TKEventManager::Callback(YYTKCodeEvent* CodeEvent, void* Self)
{
	TKEventManager* TKSelf = static_cast<TKEventManager*>(Self); // To access vars

	// Loop through vectors and see if registered
	CCode* CodeObj = std::get<CCode*>(CodeEvent->Arguments());
	CInstance* SelfInst = std::get<0>(CodeEvent->Arguments());
	CInstance* OtherInst = std::get<1>(CodeEvent->Arguments());

	if (!CodeObj || !CodeObj->i_pName)
	{
		return YYTKStatus::YYTK_INVALIDARG;
	}

	std::cout << CodeObj->i_pName << std::endl;

	CodeEvent->Call(SelfInst, OtherInst, CodeObj, std::get<3>(CodeEvent->Arguments()), std::get<4>(CodeEvent->Arguments()));

	std::vector<TKEventCallback>* AllCallbacksForEvent;
	if ((AllCallbacksForEvent = TKSelf->GetEventCallbacks(CodeObj->i_pName)) != nullptr)
	{
		for (TKEventCallback RegCallback : *AllCallbacksForEvent)
		{
			RegCallback(CodeObj, SelfInst, OtherInst, nullptr); // Call the callback function
		}
	}
	
	return YYTKStatus::YYTK_OK;
}


TKEventManager::TKEventManager(std::string PluginName, YYTKPlugin* ThisPlugin, FNPluginUnload OnUnload)
{
	mPluginName = PluginName;
	mThisPlugin = ThisPlugin;
	mThisPlugin->PluginUnload = OnUnload;
}
