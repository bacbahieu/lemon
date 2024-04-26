#ifndef INIT_H
#define INIT_H

#include "CommonFunc.h"
#include "BaseObject.h"

// Function to play background music
void PlayBackgroundMusic(Mix_Music* bg_music) {
    Mix_PlayMusic(bg_music, -1); // -1 means loop indefinitely
}


bool InitData(Mix_Music*& bg_music, SDL_Renderer* des) // Modify to accept reference to bg_music
{
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0)
        return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    g_window = SDL_CreateWindow("Game cua LE TUAN CANH",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);
    if (g_window == NULL)
    {
        success = false;
    }
    else
    {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen == NULL)
            success = false;
        else {
            SDL_SetRenderDrawColor( des, RENDER_DRAW_COLOW, RENDER_DRAW_COLOW, RENDER_DRAW_COLOW, RENDER_DRAW_COLOW);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags))
                success = false;
        }
    }


    // Init icon game
    SDL_Surface* iconSurface = IMG_Load("img//icon_game.png");
    if (!iconSurface) {
        SDL_Log("Không thể tải ảnh icon: %s", IMG_GetError());
    }
    else {
        SDL_SetWindowIcon(g_window, iconSurface);
        SDL_FreeSurface(iconSurface);
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    // Load background music
    bg_music = Mix_LoadMUS("sound//PRESS_START.mp3");
    if (!bg_music) {
        printf("Failed to load background music! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    return success;
}


bool InitTTF() {
    // Khoi tao SDL_ttf
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return false;
    }
    return true;
}

bool LoadBackground(SDL_Renderer* des)
{
    bool ret = g_background.LoadImg("img//background_8.png", des);
    if (ret == false)
        return false;

    return true;
}

void close(Mix_Music* bg_music, SDL_Renderer* des, SDL_Window* win)
{
    g_background.Free();

    SDL_DestroyRenderer(des);
    g_screen = NULL;

    SDL_DestroyWindow(win);
    g_window = NULL;

    // Free resources and quit SDL_mixer
    Mix_FreeMusic(bg_music);
    bg_music = NULL;
    Mix_Quit();

    IMG_Quit();
    SDL_Quit();
}

#endif // INIT_H
