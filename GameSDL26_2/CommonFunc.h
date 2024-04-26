

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
const int FRAME_PER_SECOND = 32; //fps

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;
const int SCREEN_BPP = 32;

const int COLOR_KEY_R = 167;
const int COLOR_KEY_G = 175;
const int COLOR_KEY_B = 180;

const int RENDER_DRAW_COLOW = 0xff;
const int START_BUTTON_X = SCREEN_WIDTH / 2;
const int START_BUTTON_Y = SCREEN_HEIGHT / 2;

const int SETTING_BUTTON_X = 860;
const int SETTING_BUTTON_Y = 260;
const int SETTING_BUTTON_WIDTH = 110;
const int SETTING_BUTTON_HEIGHT = 100;

const int RETURN_BUTTON_X = 16;
const int RETURN_BUTTON_Y = 10;
const int RETURN_BUTTON_WIDTH = 66;
const int RETURN_BUTTON_HEIGHT = 72;



const int total_map_length = 1128;

#define SPACE_PORTAL_NORMAL_TO_FLAPPY_DOWN 500
#define SPACE_PORTAL_NORMAL_TO_FLAPPY_UP 499
#define SPACE_PORTAL_FLAPPY_TO_NORMAL_UP 498
#define SPACE_PORTAL_FLAPPY_TO_NORMAL_DOWN 497

#define SPACE_PORTAL_ALL_TO_ROUND_MIN 230
#define SPACE_PORTAL_ALL_TO_ROUND_MAX 241

#define SPACE_PORTAL_ALL_TO_NORMAL_MIN 250
#define SPACE_PORTAL_ALL_TO_NORMAL_MAX 261

#define SPACE_PORTAL_ALL_TO_FLAPPY_MIN 272
#define SPACE_PORTAL_ALL_TO_FLAPPY_MAX 283

#define SPACE_PORTAL_ALL_TO_ROCKET_MIN 284
#define SPACE_PORTAL_ALL_TO_ROCKET_MAX 295

#define END_TILE_MIN 296
#define END_TILE_MAX 297


#define THREAT_TILE_MIN 100
#define THREAT_TILE_MAX 199

#define THREAT_TILE_MIN_2 300
#define THREAT_TILE_MAX_2 420


#define JUMP_6_TILE_L 200
#define JUMP_6_TILE_R 201
#define JUMP_4_TILE_L 202
#define JUMP_4_TILE_R 203
#define JUMP_3_TILE_L 204
#define JUMP_3_TILE_R 205
#define JUMP_3_TILE_UP_L 206
#define JUMP_3_TILE_UP_R 207
#define SPACE_JUMP_MIN 262
#define SPACE_JUMP_MAX 271

#define JUMP_OR_UP_MIN 220
#define JUMP_OR_UP_MAX 228

#define BLANK_TILE 0
#define TILE_SIZE  32

#define MAX_MAP_X 1128
#define MAX_MAP_X_MAP_1  124

#define MAX_MAP_Y  20

#define STATE_MONEY 477

typedef struct Input
{
	int left_;
	int right_;
	int up_;
	int down_;
	int jump_;

};

const int ROCKET_MODE = 3;
const int ROUND_MODE = 2;
const int FLAPPY_MODE = 1;
const int NORMAL_MODE = 0;

typedef struct Regime
{
	int NORMAL_;
	int FLAPPY_;
	int ROUND_;
	int ROCKET_;
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


