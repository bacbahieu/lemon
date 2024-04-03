#include "CommonFunc.h"
#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "ImpTimer.h"


enum GameState {
    MENU,
    PLAYING
};

BaseObject g_background;
int bg_x_pos = 0;
int bg_y_pos = 0;


BaseObject menu_image;
BaseObject menu_image_start;
int game_state = MENU;

// Function to play background music
void PlayBackgroundMusic(Mix_Music* bg_music) {
    Mix_PlayMusic(bg_music, -1); // -1 means loop indefinitely
}

void HandleMenuEvents(SDL_Event& event)
{

    if (event.type == SDL_KEYDOWN)
    {
        switch (event.key.keysym.sym)
        {
        case SDLK_RETURN:
            game_state = PLAYING;
            break;

        default:
            break;
        }
    }
}

bool LoadMenu()
{
    bool ret = menu_image.LoadImg("img//menu_image.png", g_screen);
    if (ret == false)
        return false;

    return true;
}

bool InitData(Mix_Music*& bg_music) // Modify to accept reference to bg_music
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
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOW, RENDER_DRAW_COLOW, RENDER_DRAW_COLOW, RENDER_DRAW_COLOW);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) && imgFlags))
                success = false;
        }
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    // Load background music
    bg_music = Mix_LoadMUS("img//BACK_ON_TRACK.mp3");
    if (!bg_music) {
        printf("Failed to load background music! SDL_mixer Error: %s\n", Mix_GetError());
        success = false;
    }

    return success;
}

bool LoadBackground()
{
    bool ret = g_background.LoadImg("img//background_8.png", g_screen);
    if (ret == false)
        return false;

    return true;
}

void close(Mix_Music* bg_music)
{
    g_background.Free();

    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;

    SDL_DestroyWindow(g_window);
    g_window = NULL;

    // Free resources and quit SDL_mixer
    Mix_FreeMusic(bg_music);
    bg_music = NULL;
    Mix_Quit();

    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    ImpTimer fps_timer;

    Mix_Music* menu_music = NULL;
    Mix_Music* bg_music = NULL; // Initialize bg_music pointer



    if (InitData(bg_music) == false)
        return -1;

    if (LoadBackground() == false)
        return -1;

    if (LoadMenu() == false)
        return -1;


    // Tai nhac menu
    menu_music = Mix_LoadMUS("img//menu_music.mp3");
    if (!menu_music) {
        printf("Failed to load menu music! SDL_mixer Error: %s\n", Mix_GetError());
        return -1;
    }

    GameMap game_map;

    char dat[] = "map/map01.dat";
    game_map.LoadMap(dat);
    game_map.LoadTiles(g_screen);

    MainObject p_player;
    p_player.LoadImg("img//frame_dichuyen.png", g_screen);
    p_player.set_clips();

    bool is_quit = false;

    while (!is_quit)
    {
        fps_timer.start();
        while (SDL_PollEvent(&g_event) != 0)
        {
            if (g_event.type == SDL_QUIT)
            {
                is_quit = true;
            }
            if (game_state == MENU)
            {
                HandleMenuEvents(g_event);
            }
            else if (game_state == PLAYING)
            {
                p_player.HandelInputAction(g_event, g_screen);
            }
        }

        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOW, RENDER_DRAW_COLOW, RENDER_DRAW_COLOW, RENDER_DRAW_COLOW);
        SDL_RenderClear(g_screen);

        if (game_state == MENU)
        {

            // Phat nhac menu
            if (Mix_PlayingMusic() == 0) {
                Mix_PlayMusic(menu_music, -1);
            }
            menu_image.LoadImg("img//menu_image.png", g_screen);
            menu_image.Render(g_screen);


        }
        else if (game_state == PLAYING)
        {
            Mix_FreeMusic(menu_music);
            menu_music = NULL;

            if (Mix_PlayingMusic() == 0) {
        
                PlayBackgroundMusic(bg_music);
            }

            bg_x_pos -= 1;
            if (bg_x_pos <= -g_background.GetWidth()) {
                bg_x_pos = 0;
            }

            g_background.Render_bg(g_screen, NULL, bg_x_pos, bg_y_pos);

            if (bg_x_pos < SCREEN_WIDTH - g_background.GetWidth()) {
                g_background.Render_bg(g_screen, NULL, bg_x_pos + g_background.GetWidth(), bg_y_pos);
            }

            Map map_data = game_map.getMap();

            p_player.HandleBullet(g_screen);
            p_player.ShowExplosion(g_screen);
            p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
            p_player.DoPlayer(map_data);

            if (p_player.GetRegimeType() == FLAPPY_MODE)
                p_player.LoadImg("img//frame_duthuyen_8frame.png", g_screen);
            else
                p_player.LoadImg("img//frame_dichuyen_2.png", g_screen);

            p_player.Show(g_screen);

            game_map.SetMap(map_data);
            game_map.DrawMap(g_screen);
        }

        SDL_RenderPresent(g_screen);

        if (p_player.CheckPlayerStartPosition()) {
            Mix_HaltMusic(); 
            Mix_PlayMusic(bg_music, -1); 
        }

        int real_imp_time = fps_timer.get_ticks();
        int time_one_frame = 800 / FRAME_PER_SECOND; // ms

        if (real_imp_time < time_one_frame)
        {
            int delay_time = time_one_frame - real_imp_time;
            if (delay_time >= 0)
            {
                SDL_Delay(delay_time);
            }
        }

        Sleep(10);
    }

    close(bg_music);

    return 0;
}
