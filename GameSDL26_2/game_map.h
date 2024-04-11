 // Link tham khao: https://phattrienphanmem123az.com/lap-trinh-game-c-p2/game-cpp-phan2-ky-thuat-tile-map.html //
#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "CommonFunc.h"
#include "BaseObject.h"

#define MAX_TILES 100

class TileMat : public BaseObject
{
public:
	TileMat() { ; }
	~TileMat() { ; }
};

class GameMap
{
public:
	GameMap() { ; }
	~GameMap() { ; }

	void LoadMap( char* name);
	void LoadTiles(SDL_Renderer* screen);
	void DrawMap(SDL_Renderer* screen);
	Map getMap() const { return game_map_; };
	void SetMap(Map& map_data) { game_map_ = map_data; };
private:
	Map game_map_;
	TileMat tile_mat[MAX_TILES];

};

#endif 
