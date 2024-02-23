#include <SDL.h>
#include<stdbool.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480


#define IMAGE_WIDTH 256
#define IMAGE_HEIGHT 256

int main(int argc, char **argv)
{

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Hello World SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        SDL_Log("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return -1;
    }


    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        SDL_Log("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Surface* image = SDL_LoadBMP("image.bmp");
    if (image == NULL)
    {
        SDL_Log("SDL_LoadBMP Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, image);
    if (texture == NULL)
    {
        SDL_Log("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_FreeSurface(image);
    
    SDL_Rect rect;
    rect.x = (int)(WINDOW_WIDTH * 0.5f - IMAGE_WIDTH * 0.5f);
    rect.y = (int)(WINDOW_HEIGHT * 0.5f - IMAGE_HEIGHT * 0.5f);
    rect.w = IMAGE_WIDTH;
    rect.h = IMAGE_HEIGHT;

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_RenderPresent(renderer);

    bool exit = false;
    SDL_Event eventData;
    while (!exit)
    {
        while (SDL_PollEvent(&eventData))
        {
            switch (eventData.type)
            {
            case SDL_QUIT:
                exit = true;
                break;
            }
        }
    }

    // Free the texture
    SDL_DestroyTexture(texture);

    // Destroy the render, window and finalise SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}