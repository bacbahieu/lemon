#ifndef MENU_H
#define MENU_H

#include "CommonFunc.h"
#include "BaseObject.h"

enum GameState {
    MENU,
    BEFORE_RUN,
    PLAYING,
    AFTER_RUN,
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

BaseObject map_3_option;

bool is_game_paused = false;
bool cap_screen_one_more = false;
bool is_start_button_loaded = false;
bool is_setting_button_loaded = false;
bool in_first_menu = true;
SDL_Texture* paused_screen = NULL;
bool is_pause_button_loaded = false;

bool come_map = false;

bool is_map_3_loaded = false;



bool LoadPauseButton(SDL_Renderer* des) {
    bool ret = pause_button.LoadImg("img//pause_pause.png", des);
    if (!ret) {
        return false;
    }
    // Đặt vị trí cho nút tạm dừng ở góc phải màn hình
    pause_button.SetRect(SCREEN_WIDTH - pause_button.GetWidth(), 0);

    return true;
}

bool LoadStartButton(SDL_Renderer* des) {
    bool ret = start_button.LoadImg("img//start_option.png", des);
    if (!ret) {
        std::cout << "Failed to load start button image!" << std::endl;
        return false;
    }

    start_button.SetRect(520, 195);


    start_button.Render(des);

    is_start_button_loaded = true;

    std::cout << "Start button loaded successfully!" << std::endl;

    return true;
}

bool LoadSettingButton(SDL_Renderer* des) {
    bool ret = setting_button.LoadImg("img//setting_option.png", des);
    if (!ret) {
        std::cout << "Failed to load setting button image!" << std::endl;
        return false;
    }

    setting_button.Render(des);

    is_setting_button_loaded = true;

    std::cout << "Setting button loaded successfully!" << std::endl;

    return true;
}


bool LoadMap_3(SDL_Renderer* des) {
    bool ret = map_3_option.LoadImg("img//map_3_option.png", des);
    if (!ret) {
        std::cout << "Failed to load map 3!" << std::endl;
        return false;
    }

    map_3_option.Render(des);

    return true;

}


void HandleMouseEventsMenu(SDL_Event& event, SDL_Renderer* renderer) {
    if (event.type == SDL_MOUSEMOTION) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (x >= START_BUTTON_X - 150 && x <= START_BUTTON_X + 150 &&
            y >= START_BUTTON_Y - 150 && y <= START_BUTTON_Y + 150) {
            LoadStartButton(renderer);
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

        if (in_first_menu) {
            if (x >= START_BUTTON_X - 150 && x <= START_BUTTON_X + 150 &&
                y >= START_BUTTON_Y - 150 && y <= START_BUTTON_Y + 150) {
                come_map = true;

            }
            else if (x >= SETTING_BUTTON_X && x <= SETTING_BUTTON_X + SETTING_BUTTON_WIDTH &&
                y >= SETTING_BUTTON_Y && y <= SETTING_BUTTON_Y + SETTING_BUTTON_HEIGHT) {
                LoadSettingButton(renderer);
                in_first_menu = false;
            }
        }
        else {
            if (x >= RETURN_BUTTON_X && x <= RETURN_BUTTON_X + RETURN_BUTTON_WIDTH &&
                y >= RETURN_BUTTON_Y && y <= RETURN_BUTTON_Y + RETURN_BUTTON_HEIGHT) {
                setting_button.Free();
                menu_image.Render(renderer);
                in_first_menu = true;
                come_map = false; // Reset come_map when returning to the first menu
            }
        }

        if (come_map) {
            is_map_3_loaded = true;

            if (x >= 255 && x <= 1020 && y >= 105 && y <= 300)
            {
                is_map_3_loaded = false;
                map_3_option.Free();
                menu_image.Render(renderer);
                game_state = BEFORE_RUN;
            }
        }
    }

}






void CaptureScreen();

void HandleMouseEventsPlaying(SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (x >= pause_button.GetRect().x && x <= pause_button.GetRect().x + pause_button.GetRect().w &&
            y >= pause_button.GetRect().y && y <= pause_button.GetRect().y + pause_button.GetRect().h) {

            is_game_paused = !is_game_paused;
            if (is_game_paused) {
                CaptureScreen();

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
    if (paused_screen != NULL) {
        SDL_RenderCopy(g_screen, paused_screen, NULL, NULL);
    }

    SDL_RenderPresent(g_screen);
}




bool LoadMenu()
{
    bool ret = menu_image.LoadImg("img//menu_image.png", g_screen);
    if (ret == false)
        return false;

    return true;
}

#endif // MENU_H
