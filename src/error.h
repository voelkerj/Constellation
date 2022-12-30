#ifndef ERROR_H
#define ERROR_H

// From https://www.willusher.io/sdl2%20tutorials/

void logSDLError(std::ostream &os, const std::string &msg)
{
    os << msg << " error: " << SDL_GetError() << std::endl;
}

#endif