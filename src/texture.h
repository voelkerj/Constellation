#ifndef TEXTURE_H
#define TEXTURE_H

// From https://www.willusher.io/sdl2%20tutorials/

SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren)
{

    SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
    if(texture == nullptr){
        logSDLError(std::cout, "LoadTexture");
    }
    return texture;
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h)
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y)
{
    int w, h;
    SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    renderTexture(tex, ren, x, y, w, h);
}

#endif