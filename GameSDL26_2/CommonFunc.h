

#ifndef  COMMON_FUNCTION_H_
#define COMMON_FUNCTION_H_

#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_screen = NULL;
static SDL_Event g_event;

// Sceen
const int FRAME_PER_SECOND = 30; //fps

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int RENDER_DRAW_COLOW = 0xff;
const int START_BUTTON_X = SCREEN_WIDTH / 2;
const int START_BUTTON_Y = SCREEN_HEIGHT / 2;

const int total_map_length = 400;

#define SPACE_PORTAL_NORMAL_TO_FLAPPY_DOWN 14
#define SPACE_PORTAL_NORMAL_TO_FLAPPY_UP 15
#define SPACE_PORTAL_FLAPPY_TO_NORMAL_UP 16
#define SPACE_PORTAL_FLAPPY_TO_NORMAL_DOWN 17

#define BLANK_TILE 0
#define TILE_SIZE  64

#define MAX_MAP_X  400
#define MAX_MAP_Y  20

#define STATE_MONEY 20

typedef struct Input
{
	int left_;
	int right_;
	int up_;
	int down_;
	int jump_;

};

const int FLAPPY_MODE = 1;
const int NORMAL_MODE = 0;

typedef struct Regime
{
	int NORMAL_;
	int FLAPPY_;
};

typedef struct Map
{
	int start_x_;
	int start_y_;

	int max_x_ ;
	int max_y_ ;

	int tile[MAX_MAP_Y][MAX_MAP_X];
	char* file_name_ ;

};



#endif 


