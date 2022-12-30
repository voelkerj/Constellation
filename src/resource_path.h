#ifndef RES_PATH_H
#define RES_PATH_H

#include <iostream>
#include <string>
#include "SDL.h"

//Adapted from https://www.willusher.io/sdl2%20tutorials/2014/06/16/postscript-0-properly-finding-resource-paths

std::string getResourcePath(const std::string &subDir = "")
{
    #ifdef _WIN32
        const char PATH_SEP = '\\';
    #else
        const char PATH_SEP = '/';
    #endif

    // Static so we can call SDL_GetBasePath one time
    static std::string baseRes;
    if (baseRes.empty())
    {
        char *basePath = SDL_GetBasePath();
        if (basePath) 
        {
            baseRes = basePath;
            SDL_free(basePath);
        }
        else
        {
            std::cerr << "Error getting resource path: " << SDL_GetError() << std::endl;
            return "";
        }

        // replaced tutorial code with my folder names
        size_t pos = baseRes.rfind("build");
        baseRes = baseRes.substr(0,pos) + "resources" + PATH_SEP;        
    }

    return subDir.empty() ? baseRes : baseRes + subDir + PATH_SEP;
}

#endif