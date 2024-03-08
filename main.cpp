
#include "CommonFunc.h"
#include "BaseObject.h"
#include "Map.h"
#include "mainSquare.h"

BaseObject g_background;
// KHoi tao moi truong SDL
bool InitData()
{
    bool success = true; 
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0)
        return false;
   
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

    // Tao cua so Window
    g_window = SDL_CreateWindow("GAME CUA LE TUAN CANH", 
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
        else
        {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlag = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlag) && imgFlag))
                success = true;
        }
    }
    return success;
}

bool LoadBackground()
{
    bool ret = g_background.LoadImg("img//background.jpg", g_screen);
    if (!ret)
        return false;
    return true;
}


void close()
{
    g_background.Free();

    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    if (InitData() == false)
        return -1;
    if (LoadBackground() == false)
        return -1;


    // print tile_map 0 1 2 3 4
    GameMap game_map;

    char dat[] = "map/tile_map.dat";
    game_map.LoadMap(dat);
    game_map.LoadTiles(g_screen);

    // Khoi tao nhan vat - Khoi lap phuong
    SquareObject g_player;
    g_player.LoadImg("img//KHOI_LAP_PHUONG.png", g_screen);

    bool is_quit = false;
    while (!is_quit)
    {
        while (SDL_PollEvent(&g_event) != 0)
        {
            if (g_event.type == SDL_QUIT)
            {
                is_quit = true;
            }

            g_player.HandleInputAction(g_event, g_screen);
        }
        
        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
        SDL_RenderClear(g_screen);

        g_background.Render(g_screen, NULL);
        game_map.DrawMap(g_screen);

        // Vẽ khối lập phương lên màn hình
        g_player.Render(g_screen);

        SDL_RenderPresent(g_screen);
    }

    close();
    return 0;
}