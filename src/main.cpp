#include <chrono>
#include <thread>
#include <mutex>
#include "satellite.h"
#include "input_parser.h"
#include "SDL.h"
#include "SDL_image.h"
#include "resource_path.h"
#include "cleanup.h"
#include "error.h"
#include "texture.h"

int main(int argc, char **argv)
{
    // Constants
    const int SCREEN_WIDTH = 1440;
    const int SCREEN_HEIGHT = 720;

    // Display the full path of the satellite (on permanently)
    const bool full_path = true;

    // Read inputs.txt
    input_parser::SimInputs inputs = input_parser::readSimInputs();

    // Read satellites.txt and Process Satellites
    std::vector<Satellite> satellite_vector = input_parser::readSatellites();
    std::vector<std::thread> threads;

    for (int sat_idx = 0; sat_idx < satellite_vector.size(); sat_idx++)
    {
        threads.emplace_back(std::thread(&Satellite::simulate, &satellite_vector[sat_idx], inputs.START_TIME, inputs.END_TIME, inputs.TIMESTEP));
    }

    for (int thread_idx = 0; thread_idx < threads.size(); thread_idx++)
    {
        threads[thread_idx].join();
    }    

    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        logSDLError(std::cout, "SDL_Init");
        return 1;
    }

    // Initialize SDL_image
    // Helps with speed later on
    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
    {
        logSDLError(std::cout, "IMG_Init");
        SDL_Quit();
        return 1;
    }

    // Create window
    SDL_Window *win = SDL_CreateWindow("Constellation", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (win == nullptr)
    {
        logSDLError(std::cout, "SDL_CreateWindow");
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == nullptr)
    {
        logSDLError(std::cout, "SDL_CreateRenderer");
        cleanup(win);
        SDL_Quit();
        return 1;
    }

    // Create Textures
    const std::string resourcePath = getResourcePath();
    SDL_Texture *background = loadTexture(resourcePath + "world_map.png", ren);
    SDL_Texture *dot = loadTexture(resourcePath + "whitedot.png", ren);

    // Make sure textures loaded
    if (background == nullptr || dot == nullptr)
    {
        cleanup(background, dot, ren, win);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Input Event
    SDL_Event e;
    bool quit = false;

    // Timing to maintain FPS Target
    Uint32 frame_start;
    Uint32 frame_end;
    Uint32 frame_duration;
    float target_frame_duration = 1000 / inputs.FPS; // milliseconds per frame for 60 fps

    int idx = 0;
    int idx_max = satellite_vector[0]._groundTrack.size();

    if (full_path)
    {
        renderTexture(background, ren, 0, 0, 1440, 720);
    }
    
    // GAME Loop
    while (!quit)
    {
        // TIMING START
        auto start_time(std::chrono::high_resolution_clock::now());

        // Events
        while (SDL_PollEvent(&e)) //INPUT Loop
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        // Clear renderer if not using full path
        if (!full_path)
        {
            SDL_RenderClear(ren);
        }

        // Render Textures
        if (!full_path)
        {
            renderTexture(background, ren, 0, 0, 1440, 720);
        }
        
        for (int sat_idx = 0; sat_idx < satellite_vector.size(); sat_idx++)
        {
            // Calculate satellite position
            int x = convertLonToScreenCoords(round(satellite_vector[sat_idx]._groundTrack[idx].lon), SCREEN_WIDTH);
            int y = convertLatToScreenCoords(round(satellite_vector[sat_idx]._groundTrack[idx].lat), SCREEN_HEIGHT);
            
            // Display satellite at center of dot sprite
            int dot_width, dot_height;

            SDL_QueryTexture(dot, NULL, NULL, &dot_width, &dot_height);

            SDL_SetTextureColorMod(dot, satellite_vector[sat_idx]._color.red, satellite_vector[sat_idx]._color.green, satellite_vector[sat_idx]._color.blue);
            renderTexture(dot, ren, x - (dot_width / 2), y - (dot_height / 2), dot_width, dot_height);            
        }

        SDL_RenderPresent(ren);

        // TIMING END
        auto end_time(std::chrono::high_resolution_clock::now());
        auto duration(std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time));

        if (duration.count() < target_frame_duration)
        {
            SDL_Delay(target_frame_duration - duration.count());
        }

        idx++;

        // If we've reached end of propagation, clear screen and we will restart it
        if (idx == idx_max)
        {
            idx = 0;
            SDL_RenderClear(ren);
            renderTexture(background, ren, 0, 0, 1440, 720);
        }
    }

    cleanup(background, dot, ren, win);
    SDL_Quit();

    return 0;
}