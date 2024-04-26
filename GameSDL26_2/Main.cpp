#include "CommonFunc.h"
#include "BaseObject.h"
#include "game_map.h"
#include "MainObject.h"
#include "BossObject.h"
#include "ImpTimer.h"
#include "Laser.h"
#include "LText.h"
#include "Logo.h"
#include "Menu.h"
#include "Init.h"
#include "Before_run.h"
#include "Option_credits.h"


bool die_by_laser = false;


int main(int argc, char* argv[])
{
    ImpTimer fps_timer;

    Mix_Music* menu_music = NULL;
    Mix_Music* bg_music = NULL; // Initialize bg_music pointer

    if (InitData(bg_music, g_screen) == false)
        return -1;

    if (LoadBackground(g_screen) == false)
        return -1;

    SDL_SetTextureBlendMode(g_background.GetObjectW(), SDL_BLENDMODE_BLEND);

    if (LoadMenu() == false)
        return -1;

    if (!InitTTF()) {
        return -1;
    }

    // Load menu music
    menu_music = Mix_LoadMUS("sound//menu_music.mp3");
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
    p_boss.SplitImage(g_screen);


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
            if (!LoadPauseButton(g_screen)) {
                return -1;
            }
            is_pause_button_loaded = true;
        }

        if (game_state == MENU) {

            op_time = 14000;

            time_run_logo -= 80;

            if (time_run_logo >= 0)
            {
                LoadLogoUET(g_screen);

                logo_UET_start.Render(g_screen);
            }
            else
            {
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

                if (is_setting_button_loaded) {
                    setting_button.Render(g_screen);
                }

                if (is_map_3_loaded) {
                    LoadMap_3(g_screen);
                    map_3_option.Render(g_screen);
                }
            }
        }

        else if (game_state == BEFORE_RUN)
        {   
            Load_Bf_RUN(g_screen);
        }

        else if (game_state == AFTER_RUN) {
            Load_Img_After_Win(g_screen);
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

            // Render player and game map
            p_player.Show(g_screen);
            game_map.SetMap(map_data);
            game_map.DrawMap(g_screen);

            if (p_player.GetX() >= 800 && p_player.GetX() <= 1120) {
                SDL_SetTextureColorMod(g_background.GetObjectW(), 250,175,180);
            }

            if (p_player.GetX() >= 900 && p_player.GetX() <= 1070)
            {
                // Xuất hiện boss
                p_boss.DoBoss(map_data);
                p_boss.Show(g_screen);
                // Reset thời gian chờ

                std::cout << "Boss xuất hiện!" << std::endl;
                p_player.CheckCollisionWithLaser(p_player, p_boss);
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

        if (p_player.Check_End_Game()) {
            game_state = AFTER_RUN;
        }
        SDL_RenderPresent(g_screen);

        // Play background music if player returns to start position
        if (p_player.CheckPlayerStartPosition() ) {
            Mix_HaltMusic();
            Mix_PlayMusic(bg_music, -1);
            bf_run_timer = 3000;
            SDL_SetTextureColorMod(g_background.GetObjectW(), 255, 255, 255);
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
    close(bg_music, g_screen, g_window);

    return 0;
}
