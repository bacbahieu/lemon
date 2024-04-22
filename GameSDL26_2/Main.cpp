﻿#include "CommonFunc.h"
#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "BossObject.h"
#include "ImpTimer.h"
#include "Laser.h"
#include "LText.h"

enum GameState {
    MENU,
    PLAYING,
};

BaseObject g_background;
int bg_x_pos = 0;
int bg_y_pos = 0;

BaseObject menu_image;
int game_state = MENU;


// pause game//
BaseObject pause_button;
BaseObject option_image;
BaseObject start_button;
BaseObject setting_button;

bool is_game_paused = false;
bool cap_screen_one_more = false;
bool is_start_button_loaded = false;
bool is_setting_button_loaded = false;
SDL_Texture* paused_screen = NULL;

bool is_pause_button_loaded = false;



bool LoadPauseButton() {
    bool ret = pause_button.LoadImg("img//pause_pause.png", g_screen);
    if (!ret) {
        return false;
    }
    // Đặt vị trí cho nút tạm dừng ở góc phải màn hình
    pause_button.SetRect(SCREEN_WIDTH - pause_button.GetWidth(), 0);

    return true;
}

bool LoadStartButton() {
    bool ret = start_button.LoadImg("img//start_option.png", g_screen);
    if (!ret) {
        std::cout << "Failed to load start button image!" << std::endl;
        return false;
    }

    start_button.SetRect(520, 195);


    start_button.Render(g_screen);

    is_start_button_loaded = true;

    std::cout << "Start button loaded successfully!" << std::endl;

    return true;
}

bool LoadSettingButton() {
    bool ret = setting_button.LoadImg("img//setting_option.png", g_screen);
    if (!ret) {
        std::cout << "Failed to load setting button image!" << std::endl;
        return false;
    }

    setting_button.SetRect(SCREEN_WIDTH - setting_button.GetWidth(), SCREEN_HEIGHT - setting_button.GetHeight());


    setting_button.Render(g_screen);

    is_setting_button_loaded = true;

    return true;
}



// Function to play background music
void PlayBackgroundMusic(Mix_Music* bg_music) {
    Mix_PlayMusic(bg_music, -1); // -1 means loop indefinitely
}

void HandleMouseEventsMenu(SDL_Event& event, SDL_Renderer* renderer) {
    if (event.type == SDL_MOUSEMOTION) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (x >= START_BUTTON_X - 150 && x <= START_BUTTON_X + 150 &&
            y >= START_BUTTON_Y - 150 && y <= START_BUTTON_Y + 150) {
            LoadStartButton();
        }
        else {
            if (is_start_button_loaded) {
                start_button.Free();
                menu_image.Render(renderer);
            }
        }
    }
    else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (x >= START_BUTTON_X - 150 && x <= START_BUTTON_X + 150 &&
            y >= START_BUTTON_Y - 150 && y <= START_BUTTON_Y + 150) {
            game_state = PLAYING;
        }
        else if (x >= SETTING_BUTTON_X && x <= SETTING_BUTTON_X + SETTING_BUTTON_WIDTH &&
            y >= SETTING_BUTTON_Y && y <= SETTING_BUTTON_Y + SETTING_BUTTON_HEIGHT) {
            // Handle setting button click: Load setting image or perform setting action
            LoadSettingButton(); // Implement this function to load and render the setting image
        }
        else if (x >= RETURN_BUTTON_X && x <= RETURN_BUTTON_X + RETURN_BUTTON_WIDTH &&
            y >= RETURN_BUTTON_Y && y <= RETURN_BUTTON_Y + RETURN_BUTTON_HEIGHT) {
            // Handle return button click: Return to previous screen or perform action
            setting_button.Free();
            menu_image.Render(renderer);
        }
    }
}





void CaptureScreen();

void HandleMouseEventsPlaying(SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        // Kiểm tra xem người chơi có bấm vào nút tạm dừng không
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (x >= pause_button.GetRect().x && x <= pause_button.GetRect().x + pause_button.GetRect().w &&
            y >= pause_button.GetRect().y && y <= pause_button.GetRect().y + pause_button.GetRect().h) {
            // Nếu có, chuyển trạng thái tạm dừng
            is_game_paused = !is_game_paused;
            if (is_game_paused) {
                CaptureScreen(); // Chụp màn hình khi trò chơi tạm dừng

            }
      
        }
    }
}



void CaptureScreen() {
    if (paused_screen != NULL) {
        SDL_DestroyTexture(paused_screen);
    }
    SDL_Surface* screen_surface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
    if (screen_surface == NULL) {
        return;
    }

    if (SDL_RenderReadPixels(g_screen, NULL, SDL_PIXELFORMAT_ARGB8888, screen_surface->pixels, screen_surface->pitch) != 0) {
        SDL_FreeSurface(screen_surface);
        return;
    }

    paused_screen = SDL_CreateTextureFromSurface(g_screen, screen_surface);
    if (paused_screen == NULL) {
        SDL_FreeSurface(screen_surface);
        return;
    }

    SDL_FreeSurface(screen_surface);
}



void RenderPausedScreen() {
    // Render ảnh đè lên ảnh chụp màn hình
    if (paused_screen != NULL) {
        SDL_RenderCopy(g_screen, paused_screen, NULL, NULL);
    }


    // Hiển thị màn hình
    SDL_RenderPresent(g_screen);
}


SDL_Color CalculateBackgroundColor(int elapsed_time) {
    // Thời gian chuyển đổi từ xanh sang tím (tính theo mili giây)
    const int transition_time = 5000; // Ví dụ: 5 giây

    // Màu xanh và màu tím
    SDL_Color blue = { 0, 0, 255, 255 };
    SDL_Color purple = { 128, 0, 128, 255 };

    // Tính toán tỉ lệ chuyển đổi
    float ratio = static_cast<float>(elapsed_time % transition_time) / transition_time;

    // Tính toán màu sắc dựa trên tỉ lệ
    SDL_Color result_color;
    result_color.r = static_cast<Uint8>(blue.r + (purple.r - blue.r) * ratio);
    result_color.g = static_cast<Uint8>(blue.g + (purple.g - blue.g) * ratio);
    result_color.b = static_cast<Uint8>(blue.b + (purple.b - blue.b) * ratio);
    result_color.a = 255; // Alpha không thay đổi

    return result_color;
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
    bg_music = Mix_LoadMUS("img//PRESS_START.mp3");
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

    if (!InitTTF()) {
        return -1;
    }



    // Load menu music
    menu_music = Mix_LoadMUS("img//menu_music.mp3");
    if (!menu_music) {
        printf("Failed to load menu music! SDL_mixer Error: %s\n", Mix_GetError());
        return -1;
    }

    GameMap game_map;

    char dat[] = "map_2/map02.dat";
    game_map.LoadMap(dat);
    game_map.LoadTiles(g_screen);

    MainObject p_player;
    p_player.LoadImg("img//nhan_vat.png", g_screen);
    p_player.set_clips();


    BossObject p_boss;

    p_boss.LoadImg("img//boss_1.png", g_screen);
    p_boss.set_clips();

    int spawn_timer = 5000; // Adjusted spawn_timer to milliseconds


    InitDeathCounter(g_screen);

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
                HandleMouseEventsMenu(g_event,g_screen);
            }
            else if (game_state == PLAYING)
            {
                HandleMouseEventsPlaying(g_event);
                if (!is_game_paused)
                {
                    p_player.HandelInputAction(g_event, g_screen);
                }
                
            }
        }

        SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOW, RENDER_DRAW_COLOW, RENDER_DRAW_COLOW, RENDER_DRAW_COLOW);
        SDL_RenderClear(g_screen);

        if (!is_pause_button_loaded) {
            if (!LoadPauseButton()) {
                return -1;
            }
            is_pause_button_loaded = true;
        }

        if (game_state == MENU) {
            // Play menu music
            if (Mix_PlayingMusic() == 0) {
                Mix_PlayMusic(menu_music, -1);
            }

            // Render menu image based on mouse position
            LoadMenu();
            menu_image.Render(g_screen);

            // Render start button if it's loaded
            if (is_start_button_loaded) {
                start_button.Render(g_screen);
            }
        }

        else if (game_state == PLAYING && !is_game_paused)
        {
            cap_screen_one_more = false;
            // Free menu music resources
            Mix_FreeMusic(menu_music);
            menu_music = NULL;

            // Play background music if not already playing
            if (Mix_PlayingMusic() == 0) {
                PlayBackgroundMusic(bg_music);
            }

            // Update background position
            bg_x_pos -= 1;
            if (bg_x_pos <= -g_background.GetWidth()) {
                bg_x_pos = 0;
            }

            // Render background
            g_background.Render_bg(g_screen, NULL, bg_x_pos, bg_y_pos);
            if (bg_x_pos < SCREEN_WIDTH - g_background.GetWidth()) {
                g_background.Render_bg(g_screen, NULL, bg_x_pos + g_background.GetWidth(), bg_y_pos);
            }



            // Handle player actions
            Map map_data = game_map.getMap();
            p_player.HandleBullet(g_screen);
            p_player.ShowExplosion(g_screen);
            p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
            p_player.DoPlayer(map_data);

            // Load player image based on regime type
            if (p_player.GetRegimeType() == FLAPPY_MODE)
            {
                p_player.LoadImg("img//flappy_player.png", g_screen);
            }
            else if (p_player.GetRegimeType() == NORMAL_MODE)
            {
                p_player.LoadImg("img//lap_phuong_3.png", g_screen);
            }
            else if (p_player.GetRegimeType() == ROUND_MODE)
            {
                p_player.LoadImg("img//khoi_tron_quay.png", g_screen);
            }
            else if (p_player.GetRegimeType() == ROCKET_MODE)
            {
                p_player.LoadImg("img//rocket_player.png", g_screen);
            }

            // Render player and game map
            p_player.Show(g_screen);
            game_map.SetMap(map_data);
            game_map.DrawMap(g_screen);

            spawn_timer -= fps_timer.get_ticks();
            if (spawn_timer <= 1000 && spawn_timer >=0)
            {
                // Xuất hiện boss
                p_boss.DoBoss(map_data);
                p_boss.Show(g_screen);
                // Reset thời gian chờ

                std::cout << "Boss xuất hiện!" << std::endl;

            }

            pause_button.Render(g_screen);

            float percent_distance_ = (static_cast<float>(p_player.GetX()) / static_cast<float>(total_map_length)) * 100.0f;

            UpdatePercent(percent_distance_);

            // ve khung thanh phan tram
            SDL_Rect gray_frame;
            gray_frame.x = 490;
            gray_frame.y = 8;
            gray_frame.w = 300;
            gray_frame.h = 18;

            SDL_SetRenderDrawColor(g_screen, 169, 169, 169, 255); // 
            SDL_RenderFillRect(g_screen, &gray_frame);

            // Ve thanh phan tram tang dan
            SDL_Rect percent_bar;
            percent_bar.x = 490;
            percent_bar.y = 10;
            percent_bar.w = static_cast<int>(percent_distance_ * 3);
            percent_bar.h = 14;

            SDL_SetRenderDrawColor(g_screen, 255, 165, 0, 255);  // Orange
            SDL_RenderFillRect(g_screen, &percent_bar);

            RenderDeathCount(g_screen);

            
        }
        else if (is_game_paused) {
            
                RenderPausedScreen(); // Hiển thị khung ảnh đã chụp khi trò chơi tạm 
          
            
        }


        SDL_RenderPresent(g_screen);

        // Play background music if player returns to start position
        if (p_player.CheckPlayerStartPosition()) {
            Mix_HaltMusic();
            Mix_PlayMusic(bg_music, -1);
            spawn_timer = 5000;
        }

        // Cap frame rate
        int real_imp_time = fps_timer.get_ticks();
        int time_one_frame = 1000 / FRAME_PER_SECOND; // ms
        if (real_imp_time < time_one_frame)
        {
            int delay_time = time_one_frame - real_imp_time;
            if (delay_time >= 0)
            {
                SDL_Delay(delay_time);
            }
        }
    }

    // Clean up resources
    close(bg_music);

    return 0;
}
