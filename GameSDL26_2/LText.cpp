// tham khảo link: https://lazyfoo.net/tutorials/SDL/16_true_type_fonts/index.php //

#include "LText.h"

int death_count = 0;
float percent_distance = 0.0f;
TTF_Font* font = NULL;
TTF_Font* font_2 = NULL;
SDL_Texture* death_count_texture = NULL;
SDL_Texture* percent_texture = NULL;

void InitDeathCounter(SDL_Renderer* renderer) {
    font = TTF_OpenFont("font//Rinnero.ttf", 60); 
    if (font == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
    }

    font_2 = TTF_OpenFont("font//Sunny Spells Basic.ttf", 24);
    if (font_2 == NULL) {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
    }
}

void UpdateDeathCount() {
    death_count++;
}

void UpdatePercent(float percent) {
    percent_distance = percent;
}

void RenderDeathCount(SDL_Renderer* renderer) {
    SDL_Color text_color = { 255, 255, 255 };

    // Render death
    SDL_Surface* text_surface = TTF_RenderText_Solid(font, ("Deaths: " + std::to_string(death_count)).c_str(), text_color);
    if (text_surface == NULL) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }
    else {
        death_count_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        if (death_count_texture == NULL) {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        else {
            SDL_Rect render_quad = { 10, 10, text_surface->w, text_surface->h };
            SDL_RenderCopy(renderer, death_count_texture, NULL, &render_quad);
        }
        SDL_FreeSurface(text_surface);
    }

    // Render %
    SDL_Surface* percent_text_surface = TTF_RenderText_Solid(font_2, ( std::to_string(static_cast<int>(percent_distance)) + "%").c_str(), text_color);
    if (percent_text_surface == NULL) {
        printf("Unable to render percent text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }
    else {
        percent_texture = SDL_CreateTextureFromSurface(renderer, percent_text_surface);
        if (percent_texture == NULL) {
            printf("Unable to create texture from rendered percent text! SDL Error: %s\n", SDL_GetError());
        }
        else {
            SDL_Rect percent_render_quad = { 800, 8, percent_text_surface->w, percent_text_surface->h };
            SDL_RenderCopy(renderer, percent_texture, NULL, &percent_render_quad);
        }
        SDL_FreeSurface(percent_text_surface);
    }
}

void CleanUpDeathCounter() {
    TTF_CloseFont(font);
    font = NULL;
    SDL_DestroyTexture(death_count_texture);
    SDL_DestroyTexture(percent_texture);
}
