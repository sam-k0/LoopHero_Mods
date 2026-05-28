#pragma once
#include "Filesystem.h"
#include <SDK.hpp>
#include <string>
// Asset Loading
namespace Assets {
    // sprite_add(filepath, imgnum, removebg, smooth, xorig, yorig)
    // Filepath is relative to the EXE directory
    double AddSprite(std::string filepath, int imgnum, bool removebg, bool smooth, int xorig, int yorig)
    {
        // Get the current directory
        std::string currentDir = Filesys::GetCurrentDir();
        // Add the filepath to the current directory
        std::string fullpath = currentDir + "\\" + filepath;

        //PrintMessage(Color::CLR_AQUA, fullpath.c_str());

        // Check if the file exists
        if (!Filesys::FileExists(fullpath))
        {
            PrintMessage(Color::CLR_RED, (("Asset could not be resolved: ") + fullpath).c_str());
            return -1.0;
        }

        // Load the sprite
        YYRValue spriteref;
        CallBuiltin(spriteref, "sprite_add", nullptr, nullptr, { fullpath.c_str(), (double)imgnum, removebg, smooth, (double)xorig, (double)yorig });
        double res = static_cast<double>(spriteref);
        if (res != -1.0)
        {
            PrintMessage(Color::CLR_GREEN, (("Loaded asset: ") + fullpath).c_str());
        }
        return res; // return the sprite id
    }

    // Gets the number of subimages
    double GetSpriteImgnum(double spriteID)
    {
        YYRValue spritenum;
        CallBuiltin(spritenum, "sprite_get_number", nullptr, nullptr, { spriteID });
        return static_cast<double>(spritenum);
    }

    double GetSpriteWidth(double spriteID)
    {
        YYRValue yyrval;
        CallBuiltin(yyrval, "sprite_get_width", nullptr, nullptr, { spriteID });
        return static_cast<double>(yyrval);
    }

    double GetSpriteHeight(double spriteID)
    {
        YYRValue yyrval;
        CallBuiltin(yyrval, "sprite_get_height", nullptr, nullptr, { spriteID });
        return static_cast<double>(yyrval);
    }

    // Gets w and h and writes it back to width and height params
    void GetSpriteDimensions(double spriteID, double& width, double& height)
    {
        width = GetSpriteWidth(spriteID);
        height = GetSpriteHeight(spriteID);
    }

    double GetSpriteOffsetX(double spriteID)
    {
        YYRValue yyrval;
        CallBuiltin(yyrval, "sprite_get_xoffset", nullptr, nullptr, { spriteID });
        return static_cast<double>(yyrval);
    }

    double GetSpriteOffsetY(double spriteID)
    {
        YYRValue yyrval;
        CallBuiltin(yyrval, "sprite_get_yoffset", nullptr, nullptr, { spriteID });
        return static_cast<double>(yyrval);
    }

    void GetSpriteOffsets(double spriteID, double& xoff, double& yoff)
    {
        xoff = GetSpriteOffsetX(spriteID);
        yoff = GetSpriteOffsetY(spriteID);
    }

    void SetSpriteOffset(double spriteID, double xoff, double yoff)
    {     
        YYRValue yyrval;
        CallBuiltin(yyrval, "sprite_set_offset", nullptr, nullptr, { spriteID, xoff, yoff });        
    }

    //Replaces the spriteID with a sprite from fpath permanently
    void SpriteReplace(double spriteID, std::string fpath, double imgnum, bool removebg, bool smooth, double xorig, double yorig)
    {
        // Get the current directory
        std::string currentDir = Filesys::GetCurrentDir();
        // Add the filepath to the current directory
        std::string fullpath = currentDir + "\\" + fpath;

        //PrintMessage(Color::CLR_AQUA, fullpath.c_str());

        // Check if the file exists
        if (!Filesys::FileExists(fullpath))
        {
            PrintMessage(Color::CLR_RED, (("Asset could not be resolved: ") + fullpath).c_str());
            return;
        }

        YYRValue yyrval;
        CallBuiltin(yyrval, "sprite_replace", nullptr, nullptr, {spriteID, fpath, imgnum, removebg, smooth, xorig, yorig});
        PrintMessage(Color::CLR_GREEN, (("Replaced asset: ") + fullpath).c_str());
    }

    // only works on runtime-loaded assets
    void SpriteDelete(double spriteID)
    {
        YYRValue yyrval;
        CallBuiltin(yyrval, "sprite_delete", nullptr, nullptr, { spriteID });       
    }
}