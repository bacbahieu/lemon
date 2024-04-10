#ifndef LOAD_TEXT_H
#define LOAD_TEXT_H

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

extern float percent_distance;
extern int death_count;
extern TTF_Font* font;
extern TTF_Font* font_2;
extern SDL_Texture* death_count_texture;

void InitDeathCounter(SDL_Renderer* renderer);
void UpdateDeathCount();
void RenderDeathCount(SDL_Renderer* renderer);
void CleanUpDeathCounter();
void UpdatePercent(float percent);

#endif // DEATH_COUNTER_H
