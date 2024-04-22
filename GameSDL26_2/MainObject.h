// Link tham khao: https://phattrienphanmem123az.com/lap-trinh-game-c-p2/game-cpp-phan-2-ky-thuat-load-nhan-vat-game.html //
#ifndef MAIN_OBJECT_H
#define MAIN_OBJECT_H

#include <vector>
#include "CommonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"
#include "BossObject.h"
#include "Laser.h"


#define GRAVITY_SPEED 0.8
#define MAX_FALL_SPEED 12
#define MAX_FALL_SPEED_FLAPPY 90
#define PLAYER_SPEED 16
#define ROUND_SPEED 20
#define PLAYER_JUMP_VAL 48;
#define FLAPPY_JUMP_VAL 64;
#define ROUND_JUMP_VAL 32;
#define JUMP_6_TILE 68;
#define JUMP_4_TILE 48; 
#define JUMP_3_TILE 36;
#define JUMP_SPACE 76;


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

	float Get_x_pos_player() { return x_pos_; }

	void LoadExplosionImage(SDL_Renderer* screen) ;
	void ShowExplosion(SDL_Renderer* des);

	void AddPassedTile(int x, int y);
	void PrintRedTiles_NORMAL(SDL_Renderer* des, double elapsed_time);
	void PrintRedTiles_ROUND(SDL_Renderer* des, double elapsed_time);
	void PrintRedTiles_ROCKET(SDL_Renderer* des, double elapsed_time);
	void PrintRedTiles_FLAPPY(SDL_Renderer* des, double elapsed_time);

	void UpdateElapsedTime(double elapsed_time) {
		elapsed_time_ += 0.1;

		if (elapsed_time_ > 7.0) {
			elapsed_time_ -= 7.0;
		}
	}
	SDL_Color ChangeColor(double time) {
		// Tính toán mức độ thay đổi màu dựa trên thời gian
		double ratio = fmod(elapsed_time_ / 7.0, 1.0); // 7 màu cầu vồng

		// Tính toán giá trị màu RGB tương ứng với màu của cầu vồng
		Uint8 red, green, blue;
		if (ratio < 1.0 / 6) {
			red = 255;
			green = static_cast<Uint8>(255 * ratio * 6);
			blue = 0;
		}
		else if (ratio < 2.0 / 6) {
			red = static_cast<Uint8>(255 * (2.0 / 6 - ratio) * 6);
			green = 255;
			blue = 0;
		}
		else if (ratio < 3.0 / 6) {
			red = 0;
			green = 255;
			blue = static_cast<Uint8>(255 * (ratio - 2.0 / 6) * 6);
		}
		else if (ratio < 4.0 / 6) {
			red = 0;
			green = static_cast<Uint8>(255 * (4.0 / 6 - ratio) * 6);
			blue = 255;
		}
		else if (ratio < 5.0 / 6) {
			red = static_cast<Uint8>(255 * (ratio - 4.0 / 6) * 6);
			green = 0;
			blue = 255;
		}
		else {
			red = 255;
			green = 0;
			blue = static_cast<Uint8>(255 * (6.0 / 6 - ratio) * 6);
		}

		return { red, green, blue, 255 }; // Trả về màu mới
	}


private:

	SDL_Texture* explosion_texture_;
	std::vector<std::pair<int, int>> passed_tiles_;
	double elapsed_time_;
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
	double rotate_angle_2;
	double rotate_angle_fappy;

	bool jump_6_tile_200_201;
	bool jump_4_tile_202_203;
	bool jump_or_up_220_228;
	bool jump_3_tile_204_205;
	bool jump_3_tile_up_206_207;
	bool space_jump_262_271;

	bool type_ROUND_up;
	bool type_ROUND_down;

	bool type_FLAPPY_up;
	bool type_FLAPPY_down;

	bool type_ROCKET_up;
	bool type_ROCKET_down;

	int dem;

	Mix_Chunk* collision_sound = nullptr; // music vu no




};


#endif
