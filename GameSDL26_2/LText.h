#ifndef LTEXT_H
#define LTEXT_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

extern int death_count;
extern float percent_distance;
extern TTF_Font* font;
extern TTF_Font* font_2;
extern SDL_Texture* death_count_texture;
extern SDL_Texture* percent_texture;

void InitDeathCounter(SDL_Renderer* renderer);
void UpdateDeathCount();
void UpdatePercent(float percent);
void RenderDeathCount(SDL_Renderer* renderer);
void CleanUpDeathCounter();

#endif // LTEXT_H
