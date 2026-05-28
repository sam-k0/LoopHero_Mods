#pragma once
#include <fstream>
#include <iterator>
#include <string>
#include <Windows.h>
#include <vector>
#include <direct.h>
#include <ostream>
#include <sstream>
// YYTK 
#define YYSDK_PLUGIN // Declare the following code as a plugin
//#define DEBUG // enables more prints
#include "SDK/SDK.hpp"
#include "Filesystem.h"

HINSTANCE DllHandle; // Self modhandle

std::string gPluginName = "sam-k0.BorderlessHero.yytk";
YYTKPlugin* gThisPlugin = nullptr;


DllExport std::string GetPluginName() // For yytk
{
    return gPluginName;
}


namespace Misc {
    void Print(std::string s, Color c = CLR_DEFAULT)
    {
        PrintMessage(c, (gPluginName + ": " + s).c_str());
    }


    bool VectorContains(std::string find, std::vector<std::string>* v)
    {
        if (std::find(v->begin(), v->end(), find) != v->end())
        {
            return true;
        }
        return false;
    }

    bool AddToVectorNoDuplicates(std::string str, std::vector<std::string>* v)
    {
        if (VectorContains(str, v))
        {
            return false;
        }
        v->push_back(str);
        return true;
    }


    void VectorToFile(std::vector<std::string>* v, std::string fname, std::string fending = ".txt")
    {
        std::string origFname = fname;
        int fnum = 0;
        while (Filesys::FileExists(fname))
        {
            fname = origFname + std::to_string(fnum) + fending;
            fnum++;
        }

        Print("Dumping to file " + fname, CLR_RED);
        std::ofstream ofile(fname);
        std::ostream_iterator<std::string> oiter(ofile, "\n");
        std::copy(std::begin(*v), std::end(*v), oiter);
    }

    // checks if a string s contains a substring subs
    bool StringHasSubstr(std::string s, std::string subs)
    {
        if (s.find(subs) != std::string::npos) {
            return true;
        }
        return false;
    }

    // checks if the vector contains a string containing a substringand returns the whole string if found
    std::string VectorFindSubstring(std::vector<std::string> strings, std::string subs)
    {
        if (strings.size() == 0)
        {
            return "";
        }

        for (std::string s : strings)
        {
            if (StringHasSubstr(s, subs))
            {
                return s;
            }
        }
        return "";
    }

    //https://www.techiedelight.com/check-if-a-string-ends-with-another-string-in-cpp/
    bool StringEndsWith(std::string const& str, std::string const& suffix) {
        if (str.length() < suffix.length()) {
            return false;
        }
        return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
    }


    std::string Join(std::vector<std::string> strings)
    {
        if (strings.size() == 0)
        {
            return "";
        }

        const char* const delim = " ";
        std::ostringstream imploded;
        std::string ret;
        std::copy(strings.begin(), strings.end(), std::ostream_iterator<std::string>(imploded, delim));
        ret = imploded.str();
        ret.pop_back();

        return ret;
    }

    void PrintArray(YYRValue var)
    {
        RValue gvrval = var.As<RValue>();

        const RefDynamicArrayOfRValue* array = reinterpret_cast<const RefDynamicArrayOfRValue*>(gvrval.RefArray);


        for (int i = 0; i < array->length; i++) {
            const RValue& element = array->m_Array[i];
            if (element.Kind == VALUE_STRING)
            {
                Misc::Print(element.String->Get());
            }
        }
    }

    const RefDynamicArrayOfRValue* ResolveArray(YYRValue var)
    {
        RValue gvrval = var.As<RValue>();
        const RefDynamicArrayOfRValue* thisarr = reinterpret_cast<const RefDynamicArrayOfRValue*>(gvrval.RefArray);
        return thisarr;
    }

    YYRValue CallBuiltin(const std::string name, CInstance* self, CInstance* other, const std::vector<YYRValue> args)
    {
        YYRValue var;
        CallBuiltin(var, name, self, other, args);
        return var;
    }
}