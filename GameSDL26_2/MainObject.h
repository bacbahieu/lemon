// Link tham khao: https://phattrienphanmem123az.com/lap-trinh-game-c-p2/game-cpp-phan-2-ky-thuat-load-nhan-vat-game.html //
#ifndef MAIN_OBJECT_H
#define MAIN_OBJECT_H

#include <vector>
#include "CommonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"


#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 20
#define MAX_FALL_SPEED_FLAPPY 90
#define PLAYER_SPEED 14
#define PLAYER_JUMP_VAL 48;
#define FLAPPY_JUMP_VAL 64;

const int NUM_TILES_TO_MOVE = 6;

class MainObject : public BaseObject
{
public :
	MainObject();
	~MainObject();

	enum WalkType
	{
		WALK_NONE = 0,
		WALK_RIGHT = 1,
	};

	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);
	void HandelInputAction(SDL_Event events, SDL_Renderer* screen);
	void set_clips();

	void DoPlayer(Map& map_data);
	void CheckToMap(Map& map_data);
	void SetMapXY(const int map_x, const int map_y) { map_x_ = map_x; map_y_ = map_y; }
	void CenterEntityOnMap(Map& map_data);
	void UpdateImagePlayer(SDL_Renderer* des);
	SDL_Rect GetRectFrame();

	void set_bullet_list(std::vector<BulletObject*> bullet_list)
	{
		p_bullet_list_ = bullet_list;
	}
	std::vector<BulletObject*> get_bullet_list() const { return p_bullet_list_; }

	void HandleBullet(SDL_Renderer* des);
	void RemoveBullet(const int& idx);
	void IncreaseMoney();
	int get_frame_width() const { return width_frame_; }
	int get_frame_height() const { return height_frame_; }
	void set_comeback_time(const int& cb_time) { come_back_time_ = cb_time; }
	bool CheckPlayerStartPosition() {return is_player_at_start_position;}
	int GetRegimeType() const;

	int GetX();

	void LoadExplosionImage(SDL_Renderer* screen) ;
	void ShowExplosion(SDL_Renderer* des);

private:

	SDL_Texture* explosion_texture_;

	int money_count;
	std::vector<BulletObject*> p_bullet_list_;
	float x_val_;
	float y_val_;

	float x_pos_;
	float y_pos_;

	float pre_y_pos_;

	int width_frame_;
	int height_frame_;

	SDL_Rect frame_clip_[8];
	SDL_Rect explosion_clips[8];
	Input input_type_;

	Regime regime_type_;

	int frame_;
	int status_;
	bool on_ground_;
	bool roi_tudo;
	bool va_cham_no;
	float previous_start_y;

	int map_x_;
	int map_y_;

	int currentTileX ;
	int come_back_time_;

	bool is_player_at_start_position; // check vi tri ban dau

	double rotate_angle;// goc quay

	Mix_Chunk* collision_sound = nullptr; // music vu no




};


#endif
