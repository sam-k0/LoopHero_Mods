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
#include "SDK/SDK.hpp"

HINSTANCE DllHandle; // Self modhandle

std::string gPluginName = "Discord Hero";
std::string gVer = "2023.11.10";
YYTKPlugin* gThisPlugin = nullptr;
CallbackAttributes_t* callbackAttr = nullptr;

std::vector<std::string> codeEventNames; // holds all strings of code events that got triggered
int gDumpNum = 0;



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


    inline bool FileExists(const std::string& name) {
        if (FILE* file = fopen(name.c_str(), "r")) {
            fclose(file);
            return true;
        }
        else {
            return false;
        }
    }

    void VectorToFile(std::vector<std::string>* v)
    {
        std::string fname = "./event_dump_" + std::to_string(gDumpNum) + ".txt";
        while (FileExists(fname))
        {
            fname = "./event_dump_" + std::to_string(gDumpNum) + ".txt";
            gDumpNum++;
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

    std::string GetCurrentDir() // Returns EXE directory
    {
        char cCurrentPath[FILENAME_MAX]; // get working directory into buffer
        if (!_getcwd(cCurrentPath, sizeof(cCurrentPath)))
            exit(-1);
        cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; // not really required

        char* s = cCurrentPath; // save path from buffer into currentpath chararr
        std::string str(s);
        //free(s);
        return str;
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
}