// Link tham khao: https://phattrienphanmem123az.com/lap-trinh-game-c-p2/game-cpp-phan-2-ky-thuat-load-nhan-vat-game.html //
#include "MainObject.h"
#include "LText.h"

MainObject::MainObject()
{
	frame_ = 0;
	x_pos_ = 32; // 11776
	y_pos_ = 280; // 192

	pre_y_pos_ = 480;
	x_val_ = 0;
	y_val_ = 0;
	width_frame_ = 0;
	height_frame_ = 0;
	status_ = WALK_NONE;
	input_type_.jump_ = 0;
	input_type_.right_ = 0;
	input_type_.up_ = 0;

	regime_type_.NORMAL_ = 1;
	regime_type_.FLAPPY_ = 0;
	regime_type_.ROUND_ = 0;
	regime_type_.ROCKET_ = 0;

	type_ROUND_up = true;
	type_ROUND_down = false;

	type_FLAPPY_down = true;
	type_FLAPPY_up = false;

	type_ROCKET_up = true;
	type_ROCKET_down = false;

	on_ground_ = false;
	roi_tudo = false;
	va_cham_no = false;
	rotate_angle_2 = 0;
	rotate_angle_fappy = 0;
	map_x_ = 0;
	map_y_ = 0;


	currentTileX = 0;
	previous_start_y = 0;
	come_back_time_ = 0;

	is_player_at_start_position = false;
	jump_6_tile_200_201 = false;
	jump_4_tile_202_203 = false;
	jump_or_up_220_228 = false;
	jump_3_tile_204_205 = false;
	jump_3_tile_up_206_207 = false;
	space_jump_262_271 = false;

	money_count = 0;

	explosion_texture_ = nullptr;

	elapsed_time_ = 0.1;
	dem = 0;
}

MainObject::~MainObject()
{
	if (explosion_texture_ != nullptr)
	{
		SDL_DestroyTexture(explosion_texture_);
		explosion_texture_ = nullptr;
	}

	if (collision_sound != nullptr) {
		Mix_FreeChunk(collision_sound);
		collision_sound = nullptr;
	}
}

bool MainObject::LoadImg(std::string path, SDL_Renderer* screen)
{

	bool ret = BaseObject::LoadImg(path, screen);
	if (ret == true)
	{
		width_frame_ = rect_.w ;
		height_frame_ = rect_.h ;
	}

	return ret;
}


SDL_Rect MainObject::GetRectFrame()
{
	SDL_Rect rect;
	rect.x = rect_.x;
	rect.y = rect_.y;
	rect.w = width_frame_;
	rect.h = height_frame_;	

	return rect;
}

void MainObject::set_clips()
{
	if (width_frame_ > 0 && height_frame_ > 0) for (int i = 0; i < 8; ++i)
	{
		frame_clip_[i].x = i * width_frame_;
		frame_clip_[i].y = 0;
		frame_clip_[i].w = width_frame_;
		frame_clip_[i].h = height_frame_;
	}
}

void MainObject::LoadExplosionImage(SDL_Renderer* screen)
{
	
	SDL_Surface* explosionSurface = IMG_Load("img//exp3.png");
	if (explosionSurface == nullptr)
	{
		// Handle error
		std::cerr << "Failed to load explosion image. SDL Error: " << IMG_GetError() << std::endl;
	}
	else
	{
		// Convert surface to texture
		explosion_texture_ = SDL_CreateTextureFromSurface(screen, explosionSurface);
		if (explosion_texture_ == nullptr)
		{
			// Handle error
			std::cerr << "Failed to create texture from explosion surface. SDL Error: " << SDL_GetError() << std::endl;
		}
		else
		{
			// Set clips for each frame
			for (int i = 0; i < 8; ++i)
			{
				explosion_clips[i].x = i * width_frame_;
				explosion_clips[i].y = 0;
				explosion_clips[i].w = width_frame_;
				explosion_clips[i].h = height_frame_; 
			}
		}

		// Free the surface
		SDL_FreeSurface(explosionSurface);
	}
}


void MainObject::ShowExplosion(SDL_Renderer* des)
{
	static int current_frame = 0;
	SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };
	SDL_Rect* current_clip = &explosion_clips[current_frame];
	SDL_RenderCopy(des, explosion_texture_, current_clip, &renderQuad);

	
	current_frame++;
	if (current_frame >= 8)
	{
		current_frame = 0; 
	}
}



void MainObject::AddPassedTile(int x, int y) {
	passed_tiles_.push_back(std::make_pair(x, y));
}

void MainObject::PrintRedTiles_NORMAL(SDL_Renderer* des, double elapsed_time) {
	UpdateElapsedTime(elapsed_time);

	for (int i = passed_tiles_.size() - 10; i < passed_tiles_.size(); ++i) {
		int tile_x = passed_tiles_[i].first;
		int tile_y = passed_tiles_[i].second;
		SDL_Rect rect = { tile_x * TILE_SIZE - map_x_ -16, tile_y * TILE_SIZE - map_y_ + 32 +16, TILE_SIZE/2, TILE_SIZE/2 };
		SDL_Color color = ChangeColor(elapsed_time_);
		SDL_SetRenderDrawColor(des, color.r, color.g, color.b, color.a);

		SDL_RenderFillRect(des, &rect);
	}
}

void MainObject::PrintRedTiles_ROUND(SDL_Renderer* des, double elapsed_time) {
	UpdateElapsedTime(elapsed_time);

	for (int i = passed_tiles_.size() - 10; i < passed_tiles_.size(); ++i) {
		int tile_x = passed_tiles_[i].first;
		int tile_y = passed_tiles_[i].second;
		SDL_Rect rect = { tile_x * TILE_SIZE - map_x_ - 16 , tile_y * TILE_SIZE - map_y_, TILE_SIZE *2, TILE_SIZE * 2 };
		SDL_Color color = ChangeColor(elapsed_time_);
		SDL_SetRenderDrawColor(des, color.r, color.g, color.b, color.a);

		SDL_RenderFillRect(des, &rect);
	}
}

void MainObject::PrintRedTiles_ROCKET(SDL_Renderer* des, double elapsed_time) {
	UpdateElapsedTime(elapsed_time);

	// Tính toán màu dựa trên thời gian trôi qua
	SDL_Color color = ChangeColor(elapsed_time_);

	// Đặt màu vẽ cho renderer
	SDL_SetRenderDrawColor(des, color.r, color.g, color.b, color.a);

	// Duyệt qua tất cả các ô đã đi qua và vẽ chúng
	for (int i = 0; i < passed_tiles_.size(); ++i) {
		int tile_x = passed_tiles_[i].first;
		int tile_y = passed_tiles_[i].second;
		SDL_Rect rect_1 = { tile_x * TILE_SIZE - map_x_ - 16, tile_y * TILE_SIZE - map_y_ , TILE_SIZE/4 , TILE_SIZE /4 };
		SDL_Rect rect_2 = { tile_x * TILE_SIZE - map_x_ - 16, tile_y * TILE_SIZE - map_y_ +16 , TILE_SIZE / 4 , TILE_SIZE / 4 };
		SDL_RenderFillRect(des, &rect_1);
		SDL_RenderFillRect(des, &rect_2);
	}
}



void MainObject::PrintRedTiles_FLAPPY(SDL_Renderer* des, double elapsed_time) {
	UpdateElapsedTime(elapsed_time);
	SDL_Color color = ChangeColor(elapsed_time_);
	SDL_SetRenderDrawColor(des, color.r, color.g, color.b, color.a);
	for (int i = passed_tiles_.size() - 10; i < passed_tiles_.size(); ++i) {
		int tile_x = passed_tiles_[i].first;
		int tile_y = passed_tiles_[i].second;
		

		SDL_Rect rect_2 = { tile_x * TILE_SIZE - map_x_ - 16, tile_y * TILE_SIZE - map_y_ +38 , TILE_SIZE / 2 , TILE_SIZE / 2 };

		SDL_RenderFillRect(des, &rect_2);
	}
}




void MainObject::Show(SDL_Renderer* des)
{
	UpdateImagePlayer(des);
	if (regime_type_.NORMAL_)
	{
		if (!on_ground_)
		{
			// Render the character with the current rotation angle
			rect_.x = x_pos_ - map_x_;
			rect_.y = y_pos_ - map_y_;

			SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };

			// Render the rotated image
			SDL_Point center = { width_frame_ / 2, height_frame_ / 2 }; // Rotation center (in this case, the center of the texture)
			SDL_RendererFlip flip = SDL_FLIP_NONE; // No flip
			SDL_RenderCopyEx(des, p_object_, nullptr, &renderQuad, rotate_angle, &center, flip);

			// Increase the rotation angle gradually
			const double ROTATION_SPEED = 14.0; // Rotation speed in degrees per frame
			rotate_angle += ROTATION_SPEED;
			// Ensure the angle does not exceed 360 degrees
			if (rotate_angle >= 360)
				rotate_angle = 0;

		}
		else
		{
			// Reset rotation angle when jumping
			rotate_angle = 0;

			// Render the character without rotation
			rect_.x = x_pos_ - map_x_;
			rect_.y = y_pos_ - map_y_;

			SDL_Rect* current_clip = nullptr;
			SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };

			SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
		}

		PrintRedTiles_NORMAL(des, elapsed_time_);
	}
	else if (regime_type_.ROUND_)
	{
		// Render the character with the current rotation angle
		rect_.x = x_pos_ - map_x_;
		rect_.y = y_pos_ - map_y_;

		SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };

		// Render the rotated image
		SDL_Point center = { width_frame_ / 2, height_frame_ / 2 }; // Rotation center (in this case, the center of the texture)
		SDL_RendererFlip flip = SDL_FLIP_NONE; // No flip
		SDL_RenderCopyEx(des, p_object_, nullptr, &renderQuad, rotate_angle, &center, flip);

		// Increase the rotation angle gradually
		const double ROTATION_SPEED = 14.0; // Rotation speed in degrees per frame
		rotate_angle += ROTATION_SPEED;
		// Ensure the angle does not exceed 360 degrees
		if (rotate_angle >= 360)
			rotate_angle = 0;
	}
	else if (regime_type_.FLAPPY_)
	{

		// Render the character with the current rotation angle
		rect_.x = x_pos_ - map_x_;
		rect_.y = y_pos_ - map_y_;

		SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };
		const double ROTATION_SPEED_DOWN = 8.0; // Rotation speed in degrees per frame
		const double ROTATION_SPEED_UP = 60.0;
		// Initialize the rotation angle
		double target_angle = 0.0;

		// Determine the target angle based on the direction of motion
		if (y_val_ > 0) {
			target_angle = 45.0; // Angle when moving downwards
		}
		else if (y_val_ < 0) {
			target_angle = -45.0; // Angle when moving upwards
		}

		// Gradually adjust the rotation angle towards the target angle
		if (rotate_angle_fappy < target_angle) {
			rotate_angle_fappy += ROTATION_SPEED_DOWN;
			if (rotate_angle_fappy > target_angle) {
				rotate_angle_fappy = target_angle;
			}
		}
		else if (rotate_angle_fappy > target_angle) {
			rotate_angle_fappy -= ROTATION_SPEED_UP;
			if (rotate_angle_fappy < target_angle) {
				rotate_angle_fappy = target_angle;
			}
		}

		if (y_pos_ <= 32 || y_pos_ >= 618) {
			rotate_angle_fappy = 0;;
		}

		SDL_Point center = { 3*width_frame_ / 4, 3 * height_frame_ / 4 };
		SDL_RendererFlip flip = SDL_FLIP_NONE; // No flip
		SDL_RenderCopyEx(des, p_object_, nullptr, &renderQuad, rotate_angle_fappy, &center, flip);

		PrintRedTiles_FLAPPY(des, elapsed_time_);

		
	}
	else if (regime_type_.ROCKET_)
	{
		// Render the character with the current rotation angle
		rect_.x = x_pos_ - map_x_;
		rect_.y = y_pos_ - map_y_;

		SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };

		// Render the rotated image
		SDL_Point center = { width_frame_ / 2, height_frame_ / 2 }; // Rotation center (in this case, the center of the texture)
		SDL_RendererFlip flip = SDL_FLIP_NONE; // No flip

		if (type_ROCKET_down)
		{
			rotate_angle = 45;
		}
		else if (type_ROCKET_up)
		{
			rotate_angle = 315;
		}

		SDL_RenderCopyEx(des, p_object_, nullptr, &renderQuad, rotate_angle, &center, flip);
		PrintRedTiles_ROCKET(des, elapsed_time_);
	}
	
}





void MainObject::HandelInputAction(SDL_Event events, SDL_Renderer* screen)
{
	status_ = WALK_RIGHT;
	input_type_.right_ = 1;
	UpdateImagePlayer(screen);

	if (regime_type_.NORMAL_ == 1) {
		
		if (events.type == SDL_KEYDOWN)
		{
			if (events.key.keysym.sym == SDLK_UP || events.key.keysym.sym == SDLK_SPACE)
			{
				if (on_ground_)
				{
					input_type_.jump_ = 1;
				}
				else
				{
					jump_or_up_220_228 = true;
				}

			}
		}
		
	}
	else if (regime_type_.ROUND_ == 1)
	{

		if (events.type == SDL_KEYDOWN)
		{
			if (events.key.keysym.sym == SDLK_UP || events.key.keysym.sym == SDLK_SPACE)
			{
				
				input_type_.jump_ = 1;
				
			}
		}
		else if (events.type == SDL_MOUSEBUTTONDOWN)
		{
			if (events.button.button == SDL_BUTTON_LEFT)
			{

				input_type_.jump_ = 1;
			
			}
		}
	}


	else if(regime_type_.FLAPPY_ == 1)
	{
		
		if (events.type == SDL_KEYDOWN)
		{
			if (events.key.keysym.sym == SDLK_UP)
			{	
					input_type_.jump_ = 1;		
			}

		}

		if (events.type == SDL_KEYUP)
		{

			if (events.key.keysym.sym == SDLK_SPACE)
			{
				BulletObject* p_bullet = new BulletObject();
				p_bullet->LoadImgBullet(screen);
				p_bullet->SetRect(this->rect_.x + width_frame_ - 20, rect_.y + height_frame_ * 0.25);

				p_bullet->set_x_val(20);
				p_bullet->set_y_val(20);
				p_bullet->set_is_move(true);
				p_bullet_list_.push_back(p_bullet);
			}
		}
	}

	else if (regime_type_.ROCKET_ == 1)
	{

		if (events.type == SDL_KEYDOWN)
		{
			if (events.key.keysym.sym == SDLK_UP || events.key.keysym.sym == SDLK_SPACE)
			{
				if (type_ROCKET_up)
				{
					type_ROCKET_up = false;
					type_ROCKET_down = true;
				}
				else
				{
					type_ROCKET_up = true;
					type_ROCKET_down = false;

				}
			}
		}

	}
		
}

void MainObject::HandleBullet(SDL_Renderer* des)
{
	for (int i = 0; i < p_bullet_list_.size(); i++)
	{
		BulletObject* p_bullet = p_bullet_list_.at(i);
		if (p_bullet != NULL)
		{
			if (p_bullet->get_is_move() == true)
			{
				p_bullet->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
				p_bullet->Render(des);
			}
			else
			{
				p_bullet_list_.erase(p_bullet_list_.begin() + i);
				if (p_bullet != NULL) 
				{
					delete p_bullet;
					p_bullet = NULL;
				}
			}
		}
	}
}


void MainObject::RemoveBullet(const int& idx)
{
	int size = p_bullet_list_.size();
	if (size > 0 && idx < size)
	{
		BulletObject* p_bullet = p_bullet_list_.at(idx);
		p_bullet_list_.erase(p_bullet_list_.begin() + idx);

		if (p_bullet)
		{
			delete p_bullet;
			p_bullet = NULL;
		}
	}
}

void MainObject::DoPlayer(Map& map_data)
{
	if (come_back_time_ == 0)
	{
		is_player_at_start_position = false;
		x_val_ = 0;

		int dem = 0;
		 
		
		 if (regime_type_.NORMAL_ == 1) {

			 if (input_type_.right_ == 1)
			 {
				 x_val_ += PLAYER_SPEED;
			 }

			 y_val_ += 8;
			 if (y_val_ >= MAX_FALL_SPEED) y_val_ = MAX_FALL_SPEED;
			 
			 if (input_type_.jump_ == 1)
			 {
				 if (on_ground_ == true )
				 {
					 y_val_ = -PLAYER_JUMP_VAL;
					 
				 }
				 
				 input_type_.jump_ = 0;
			 }
			 

			 // cham_o_6_jump 
			 if (jump_6_tile_200_201)
			 {
				 y_val_ = -JUMP_6_TILE;
				 jump_6_tile_200_201 = false;
			 }
			
			 // cham_o_4_jump
			 if (jump_4_tile_202_203)
			 {
				 y_val_ = -JUMP_4_TILE;
				 jump_4_tile_202_203 = false;
				 jump_or_up_220_228 = false;
			 }

			 // cham_o_3_jump
			 if (jump_3_tile_204_205)
			 {
				 y_val_ = -JUMP_3_TILE;
				 jump_3_tile_204_205 = false;
			 }
			 
			 if (jump_3_tile_up_206_207)
			 {
				 y_val_ = JUMP_3_TILE;
				 jump_3_tile_up_206_207 = false;
			 }
			 
			 // cong khong gian bay len
			 if (space_jump_262_271)
			 {
				 y_val_ = -JUMP_SPACE;
				 space_jump_262_271 = false;
			 }
		

			 on_ground_ = false;
		 }
		 

		 else if (regime_type_.ROUND_ == 1)
		 {

			 if (input_type_.right_ == 1)
			 {
				 x_val_ += ROUND_SPEED;
			 }
			 if (input_type_.jump_ == 1)
			 {
				 
				 if (type_ROUND_up)
				 {
						 y_val_ = -ROUND_JUMP_VAL;
						 type_ROUND_up = false;
						 type_ROUND_down = true; 
				 }
				 else if (type_ROUND_down)
				 {
						 y_val_ = +ROUND_JUMP_VAL;
						 type_ROUND_up = true;
						 type_ROUND_down = false;
						 
				 }

				 input_type_.jump_ = 0;
				 on_ground_ = false;
				
			 }

		 }

		 else if (regime_type_.FLAPPY_ == 1)
		 {
			 if (input_type_.right_ == 1)
			 {
				 x_val_ += PLAYER_SPEED;
			 }

			 y_val_ = 8;
			 if (input_type_.jump_ == 1)
			 {
				 type_FLAPPY_up = true;
				 y_val_ = -56;
				 input_type_.jump_ = 0;
			 }
			 type_FLAPPY_up = false;


		 }

		 else if (regime_type_.ROCKET_ == 1)
		 {
			 input_type_.jump_ = 1;
			 x_val_ = 20;
			
				 if (type_ROCKET_up)
				 {
					 y_val_ = -20;
				 }

				 else
				 {
					 y_val_ = 20;
				 }

			if (y_pos_ <= 32 || y_pos_ >= 608)
				 {
					 come_back_time_++;
			     }
			 
			 
		 }
		 


		currentTileX += y_val_;


		CheckToMap(map_data);

		if (va_cham_no) { std::cout << "CA_1 "; }

		CenterEntityOnMap(map_data);

		int current_tile_x = x_pos_ / TILE_SIZE;
		int current_tile_y = y_pos_ / TILE_SIZE;

		
	    AddPassedTile(current_tile_x, current_tile_y);
		

		if (on_ground_)
		{
			y_val_ = 0;
		}
		std::cout << x_pos_ << "\n";

	}


	if (come_back_time_ > 0)
	{
		if (va_cham_no) {
			// Play sound effect
			if (collision_sound == nullptr) {
				collision_sound = Mix_LoadWAV("img//vu_no_3.mp3"); // Adjust the path to your sound file
				if (collision_sound == nullptr) {
					// Failed to load the sound
					std::cerr << "Failed to load collision sound effect. SDL_mixer Error: " << Mix_GetError() << std::endl;
				}
			}
			else {
				Mix_PlayChannel(-1, collision_sound, 0); // Play the sound effect
			}
			va_cham_no = false;
		}
		
		is_player_at_start_position = true;

		
		come_back_time_--;
		if (come_back_time_ == 0)// Reset again
		{
			UpdateDeathCount();
			x_pos_ = 32; // 64
			y_pos_ = 480;
			x_val_ = 0;
			y_val_ = 0;

			type_ROUND_up = true;
			type_ROUND_down = false;

			regime_type_.ROCKET_ = 0;
			regime_type_.ROUND_ = 0;
			regime_type_.FLAPPY_ = 0;
			regime_type_.NORMAL_ = 1;
		}
	}
	
}

void MainObject::CenterEntityOnMap(Map& map_data)
{
	// map theo chieu ngang, luon co dinh nhan vat o giua man hinh
	map_data.start_x_ = x_pos_ - (SCREEN_WIDTH / 2);

	if (map_data.start_x_ < 0)
	{
		map_data.start_x_ = 0;
	}
	else if (map_data.start_x_ + SCREEN_WIDTH >= map_data.max_x_)
	{
		map_data.start_x_ = map_data.max_x_ - SCREEN_WIDTH;
	}


}



void MainObject::CheckToMap(Map& map_data)
{
	int x1 = 0;
	int x2 = 0;

	int y1 = 0;
	int y2 = 0;

	//check horizontal
	int height_min = min(height_frame_, TILE_SIZE);


	x1 = (x_pos_ + x_val_) / TILE_SIZE;
	x2 = (x_pos_ + x_val_ + 56 - 0.01) / TILE_SIZE;

	y1 = (y_pos_) / TILE_SIZE;
	y2 = (y_pos_ + height_min - 0.01) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 <= MAX_MAP_Y)
	{
		if (x_val_ > 0) //main object is moving to right
		{

			int val1 = map_data.tile[y1][x2];
			int val2 = map_data.tile[y2][x2];

			if (val1 == STATE_MONEY || val2 == STATE_MONEY)
			{
				map_data.tile[y1][x2] = 0;
				map_data.tile[y2][x2] = 0;
				IncreaseMoney();
			}
			else if ((val1 >= THREAT_TILE_MIN && val1 <= THREAT_TILE_MAX) || (val2 >= THREAT_TILE_MIN && val2 <= THREAT_TILE_MAX))
			{
				;
			}
			else if ((val1 >= JUMP_OR_UP_MIN && val1 <= JUMP_OR_UP_MAX) || (val2 >= JUMP_OR_UP_MIN && val2 <= JUMP_OR_UP_MAX))
			{
				if (jump_or_up_220_228) {
					jump_4_tile_202_203 = true;
				}
			}
			else if ((val1 >= SPACE_JUMP_MIN && val1 <= SPACE_JUMP_MAX) || (val2 >= SPACE_JUMP_MIN && val2 <= SPACE_JUMP_MAX))
			{
				if ((val1 >= 268 && val1 <= 270) || (val2 >= 268 && val2 <= 270))
				{
					space_jump_262_271 = true;
				}
			}
			else if ((val1 == JUMP_4_TILE_L || val1 == JUMP_4_TILE_R) || (val2 == JUMP_4_TILE_L || val2 == JUMP_4_TILE_R))
			{
				jump_4_tile_202_203 = true;
			}
			else if ((val1 == JUMP_6_TILE_L || val1 == JUMP_6_TILE_R) || (val2 == JUMP_6_TILE_L || val2 == JUMP_6_TILE_R))
			{
				jump_6_tile_200_201 = true;
			}
			else if ((val1 == JUMP_3_TILE_L || val1 == JUMP_3_TILE_R) || (val2 == JUMP_3_TILE_L || val2 == JUMP_3_TILE_R))
			{
				jump_3_tile_204_205 = true;
			}
			else if ((val1 == JUMP_3_TILE_UP_L || val1 == JUMP_3_TILE_UP_R) || (val2 == JUMP_3_TILE_UP_L || val2 == JUMP_3_TILE_UP_R))
			{
				jump_3_tile_up_206_207 = true;
			}
			else if ((val1 >= SPACE_PORTAL_ALL_TO_ROCKET_MIN && val1 <= SPACE_PORTAL_ALL_TO_ROCKET_MAX) || (val2 >= SPACE_PORTAL_ALL_TO_ROCKET_MIN && val2 <= SPACE_PORTAL_ALL_TO_ROCKET_MAX))
			{
				regime_type_.ROCKET_ = 1;
				regime_type_.ROUND_ = 0;
				regime_type_.FLAPPY_ = 0;
				regime_type_.NORMAL_ = 0;

			}
			else if ((val1 >= SPACE_PORTAL_ALL_TO_NORMAL_MIN && val1 <= SPACE_PORTAL_ALL_TO_NORMAL_MAX) || (val2 >= SPACE_PORTAL_ALL_TO_NORMAL_MIN && val2 <= SPACE_PORTAL_ALL_TO_NORMAL_MAX))
			{
				regime_type_.ROCKET_ = 0;
				regime_type_.ROUND_ = 0;
				regime_type_.FLAPPY_ = 0;
				regime_type_.NORMAL_ = 1;
			}
			else if ((val1 >= SPACE_PORTAL_ALL_TO_ROUND_MIN && val1 <= SPACE_PORTAL_ALL_TO_ROUND_MAX) || (val2 >= SPACE_PORTAL_ALL_TO_ROUND_MIN && val2 <= SPACE_PORTAL_ALL_TO_ROUND_MAX))
			{

				regime_type_.ROCKET_ = 0;
				regime_type_.ROUND_ = 1;
				regime_type_.FLAPPY_ = 0;
				regime_type_.NORMAL_ = 0;
			}
			else if ((val1 >= SPACE_PORTAL_ALL_TO_FLAPPY_MIN && val1 <= SPACE_PORTAL_ALL_TO_FLAPPY_MAX) || (val2 >= SPACE_PORTAL_ALL_TO_FLAPPY_MIN && val2 <= SPACE_PORTAL_ALL_TO_FLAPPY_MAX))
			{
				
				regime_type_.ROCKET_ = 0;
				regime_type_.ROUND_ = 0;
				regime_type_.FLAPPY_ = 1;
				regime_type_.NORMAL_ = 0;
			}
			else if (val1 == SPACE_PORTAL_NORMAL_TO_FLAPPY_UP || val2 == SPACE_PORTAL_NORMAL_TO_FLAPPY_UP || val1 == SPACE_PORTAL_NORMAL_TO_FLAPPY_DOWN || val2 == SPACE_PORTAL_NORMAL_TO_FLAPPY_DOWN)
			{
				regime_type_.FLAPPY_ = 1;
				regime_type_.NORMAL_ = 0;
			}
			else if (val1 == SPACE_PORTAL_FLAPPY_TO_NORMAL_UP || val2 == SPACE_PORTAL_FLAPPY_TO_NORMAL_UP || val1 == SPACE_PORTAL_FLAPPY_TO_NORMAL_DOWN || val2 == SPACE_PORTAL_FLAPPY_TO_NORMAL_DOWN)
			{
				regime_type_.FLAPPY_ = 0;
				regime_type_.NORMAL_ = 1;
			}
			else 
			{
				if (val1 != BLANK_TILE|| val2 != BLANK_TILE )
				{
					/*x_pos_ = x2 * TILE_SIZE;
					x_pos_ -= width_frame_ + 0.01;
					x_val_ = 0;*/
					va_cham_no = true;
					come_back_time_++;
				}
				
			}
			
		}
	
	}


	// Check threat


	x1 = (x_pos_ + x_val_+ TILE_SIZE/2) / TILE_SIZE;
	x2 = (x_pos_ + x_val_ + width_frame_ - TILE_SIZE / 2) / TILE_SIZE;

	y1 = (y_pos_) / TILE_SIZE;
	y2 = (y_pos_ + height_min - TILE_SIZE / 2) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 <= MAX_MAP_Y)
	{
		if (x_val_ > 0) //main object is moving to right
		{

			int val1 = map_data.tile[y1][x2];
			int val2 = map_data.tile[y2][x2];

			if ((val1 >= THREAT_TILE_MIN && val1 <= THREAT_TILE_MAX) || (val2 >= THREAT_TILE_MIN && val2 <= THREAT_TILE_MAX))
			 {
				 va_cham_no = true;
				 come_back_time_++;
			 }
			 
		}

	}


	//check vertical

	int width_min = min(width_frame_, TILE_SIZE);
	x1 = (x_pos_) / TILE_SIZE;
	x2 = (x_pos_ + width_min) / TILE_SIZE;

	y1 = (y_pos_ + y_val_ + 0.01) / TILE_SIZE;
	y2 = (y_pos_ + y_val_ + 56 - 0.01) / TILE_SIZE;
	

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
	{
		if (y_val_ > 0)
		{
			int val1 = map_data.tile[y2][x1];
			int val2 = map_data.tile[y2][x2];

			if (val1 == STATE_MONEY || val2 == STATE_MONEY)
			{
				map_data.tile[y2][x1] = 0;
				map_data.tile[y2][x2] = 0;
				IncreaseMoney();
			}
			else if ((val1 >= THREAT_TILE_MIN && val1 <= THREAT_TILE_MAX) || (val2 >= THREAT_TILE_MIN && val2 <= THREAT_TILE_MAX))
			{
				;
			}
			else if ((val1 >= JUMP_OR_UP_MIN && val1 <= JUMP_OR_UP_MAX) || (val2 >= JUMP_OR_UP_MIN && val2 <= JUMP_OR_UP_MAX))
			{
				if (jump_or_up_220_228) {
					jump_4_tile_202_203 = true;
				}
			}
			else if ((val1 >= SPACE_JUMP_MIN && val1 <= SPACE_JUMP_MAX) || (val2 >= SPACE_JUMP_MIN && val2 <= SPACE_JUMP_MAX))
			{
				if ((val1 >= 268 && val1 <= 270) || (val2 >= 268 && val2 <= 270))
				{
					space_jump_262_271 = true;
				}
			}
			else if ((val1 == JUMP_4_TILE_L || val1 == JUMP_4_TILE_R) || (val2 == JUMP_4_TILE_L || val2 == JUMP_4_TILE_R))
			{
				jump_4_tile_202_203 = true;
			}

			else if ((val1 == JUMP_6_TILE_L || val1 == JUMP_6_TILE_R) || (val2 == JUMP_6_TILE_L || val2 == JUMP_6_TILE_R))
			{
				jump_6_tile_200_201 = true;
			}
			else if ((val1 == JUMP_3_TILE_L || val1 == JUMP_3_TILE_R) || (val2 == JUMP_3_TILE_L || val2 == JUMP_3_TILE_R))
			{
				jump_3_tile_204_205 = true;
			}
			else if ((val1 == JUMP_3_TILE_UP_L || val1 == JUMP_3_TILE_UP_R) || (val2 == JUMP_3_TILE_UP_L || val2 == JUMP_3_TILE_UP_R))
			{
				jump_3_tile_up_206_207 = true;
			}
			else if ((val1 >= SPACE_PORTAL_ALL_TO_ROCKET_MIN && val1 <= SPACE_PORTAL_ALL_TO_ROCKET_MAX) || (val2 >= SPACE_PORTAL_ALL_TO_ROCKET_MIN && val2 <= SPACE_PORTAL_ALL_TO_ROCKET_MAX))
			{
				regime_type_.ROCKET_ = 1;
				regime_type_.ROUND_ = 0;
				regime_type_.FLAPPY_ = 0;
				regime_type_.NORMAL_ = 0;

			}
			else if ((val1 >= SPACE_PORTAL_ALL_TO_NORMAL_MIN && val1 <= SPACE_PORTAL_ALL_TO_NORMAL_MAX) || (val2 >= SPACE_PORTAL_ALL_TO_NORMAL_MIN && val2 <= SPACE_PORTAL_ALL_TO_NORMAL_MAX))
			{
				regime_type_.ROCKET_ = 0;
				regime_type_.ROUND_ = 0;
				regime_type_.FLAPPY_ = 0;
				regime_type_.NORMAL_ = 1;
			}
			else if ((val1 >= SPACE_PORTAL_ALL_TO_ROUND_MIN && val1 <= SPACE_PORTAL_ALL_TO_ROUND_MAX) || (val2 >= SPACE_PORTAL_ALL_TO_ROUND_MIN && val2 <= SPACE_PORTAL_ALL_TO_ROUND_MAX))
			{
				regime_type_.ROCKET_ = 0;
				regime_type_.ROUND_ = 1;
				regime_type_.FLAPPY_ = 0;
				regime_type_.NORMAL_ = 0;

			}
			else if ((val1 >= SPACE_PORTAL_ALL_TO_FLAPPY_MIN && val1 <= SPACE_PORTAL_ALL_TO_FLAPPY_MAX) || (val2 >= SPACE_PORTAL_ALL_TO_FLAPPY_MIN && val2 <= SPACE_PORTAL_ALL_TO_FLAPPY_MAX))
			{
				regime_type_.ROCKET_ = 0;
				regime_type_.ROUND_ = 0;
				regime_type_.FLAPPY_ = 1;
				regime_type_.NORMAL_ = 0;
			}
			else if (val1 == SPACE_PORTAL_NORMAL_TO_FLAPPY_UP || val2 == SPACE_PORTAL_NORMAL_TO_FLAPPY_UP || val1 == SPACE_PORTAL_NORMAL_TO_FLAPPY_DOWN || val2 == SPACE_PORTAL_NORMAL_TO_FLAPPY_DOWN)
			{
				regime_type_.FLAPPY_ = 1;
				regime_type_.NORMAL_ = 0;
			}
			else if (val1 == SPACE_PORTAL_FLAPPY_TO_NORMAL_UP || val2 == SPACE_PORTAL_FLAPPY_TO_NORMAL_UP || val1 == SPACE_PORTAL_FLAPPY_TO_NORMAL_DOWN || val2 == SPACE_PORTAL_FLAPPY_TO_NORMAL_DOWN)
			{
				regime_type_.FLAPPY_ = 0;
				regime_type_.NORMAL_ = 1;
			}
			else {
				if (val1 != BLANK_TILE || val2 != BLANK_TILE )
				{
					y_pos_ = y2 * TILE_SIZE;
					y_pos_ -= (height_frame_ + 0.01);
					y_val_ = 0;
					on_ground_ = true;
					if (status_ == WALK_NONE)
					{
						status_ = WALK_RIGHT;
					}

				}
			}
		}

		else if (y_val_ < 0)
		{
			int val1 = map_data.tile[y1][x1];
			int val2 = map_data.tile[y1][x2];

			if (val1 == STATE_MONEY || val2 == STATE_MONEY)
			{
				map_data.tile[y1][x1] = 0;
				map_data.tile[y1][x2] = 0;
				IncreaseMoney();
			}
			else if ((val1 >= THREAT_TILE_MIN && val1 <= THREAT_TILE_MAX) || (val2 >= THREAT_TILE_MIN && val2 <= THREAT_TILE_MAX))
			{
				;
			}
			else if ((val1 >= JUMP_OR_UP_MIN && val1 <= JUMP_OR_UP_MAX) || (val2 >= JUMP_OR_UP_MIN && val2 <= JUMP_OR_UP_MAX))
			{
				if (jump_or_up_220_228) {
					jump_4_tile_202_203 = true;
				}
			}
			else if ((val1 >= SPACE_JUMP_MIN && val1 <= SPACE_JUMP_MAX) || (val2 >= SPACE_JUMP_MIN && val2 <= SPACE_JUMP_MAX))
			{
				if ((val1 >= 268 && val1 <= 270) || (val2 >= 268 && val2 <= 270))
				{
					space_jump_262_271 = true;
				}
			}
			else if ((val1 == JUMP_4_TILE_L || val1 == JUMP_4_TILE_R) || (val2 == JUMP_4_TILE_L || val2 == JUMP_4_TILE_R))
			{
				jump_4_tile_202_203 = true;
			}

			else if ((val1 == JUMP_6_TILE_L || val1 == JUMP_6_TILE_R) || (val2 == JUMP_6_TILE_L || val2 == JUMP_6_TILE_R))
			{
				jump_6_tile_200_201 = true;
			}
			else if ((val1 == JUMP_3_TILE_L || val1 == JUMP_3_TILE_R) || (val2 == JUMP_3_TILE_L || val2 == JUMP_3_TILE_R))
			{
				jump_3_tile_204_205 = true;
			}
			else if ((val1 == JUMP_3_TILE_UP_L || val1 == JUMP_3_TILE_UP_R) || (val2 == JUMP_3_TILE_UP_L || val2 == JUMP_3_TILE_UP_R))
			{
				jump_3_tile_up_206_207 = true;
			}
			else if ((val1 >= SPACE_PORTAL_ALL_TO_ROCKET_MIN && val1 <= SPACE_PORTAL_ALL_TO_ROCKET_MAX) || (val2 >= SPACE_PORTAL_ALL_TO_ROCKET_MIN && val2 <= SPACE_PORTAL_ALL_TO_ROCKET_MAX))
			{
				regime_type_.ROCKET_ = 1;
				regime_type_.ROUND_ = 0;
				regime_type_.FLAPPY_ = 0;
				regime_type_.NORMAL_ = 0;

			}
			else if ((val1 >= SPACE_PORTAL_ALL_TO_NORMAL_MIN && val1 <= SPACE_PORTAL_ALL_TO_NORMAL_MAX) || (val2 >= SPACE_PORTAL_ALL_TO_NORMAL_MIN && val2 <= SPACE_PORTAL_ALL_TO_NORMAL_MAX))
			{
				regime_type_.ROCKET_ = 0;
				regime_type_.ROUND_ = 0;
				regime_type_.FLAPPY_ = 0;
				regime_type_.NORMAL_ = 1;
			}
			else if ((val1 >= SPACE_PORTAL_ALL_TO_ROUND_MIN && val1 <= SPACE_PORTAL_ALL_TO_ROUND_MAX) || (val2 >= SPACE_PORTAL_ALL_TO_ROUND_MIN && val2 <= SPACE_PORTAL_ALL_TO_ROUND_MAX))
			{
				regime_type_.ROCKET_ = 0;
				regime_type_.ROUND_ = 1;
				regime_type_.FLAPPY_ = 0;
				regime_type_.NORMAL_ = 0;

			}
			else if ((val1 >= SPACE_PORTAL_ALL_TO_FLAPPY_MIN && val1 <= SPACE_PORTAL_ALL_TO_FLAPPY_MAX) || (val2 >= SPACE_PORTAL_ALL_TO_FLAPPY_MIN && val2 <= SPACE_PORTAL_ALL_TO_FLAPPY_MAX))
			{
				regime_type_.ROCKET_ = 0;
				regime_type_.ROUND_ = 0;
				regime_type_.FLAPPY_ = 1;
				regime_type_.NORMAL_ = 0;
			}
			else if (val1 == SPACE_PORTAL_NORMAL_TO_FLAPPY_UP || val2 == SPACE_PORTAL_NORMAL_TO_FLAPPY_UP || val1 == SPACE_PORTAL_NORMAL_TO_FLAPPY_DOWN || val2 == SPACE_PORTAL_NORMAL_TO_FLAPPY_DOWN)
			{
				regime_type_.FLAPPY_ = 1;
				regime_type_.NORMAL_ = 0;
			}
			else if (val1 == SPACE_PORTAL_FLAPPY_TO_NORMAL_UP || val2 == SPACE_PORTAL_FLAPPY_TO_NORMAL_UP || val1 == SPACE_PORTAL_FLAPPY_TO_NORMAL_DOWN || val2 == SPACE_PORTAL_FLAPPY_TO_NORMAL_DOWN)
			{
				regime_type_.FLAPPY_ = 0;
				regime_type_.NORMAL_ = 1;
			}
			else {

				if (val1 != BLANK_TILE  || val2 != BLANK_TILE)
				{
					y_pos_ = 64;
					y_val_ = 0;
				}
			}
		}
	}

	
		// check threat 
		x1 = (x_pos_ + TILE_SIZE / 2) / TILE_SIZE;
		x2 = (x_pos_ + width_min) / TILE_SIZE;

		y1 = (y_pos_ + y_val_) / TILE_SIZE;
		y2 = (y_pos_ + y_val_ + height_frame_ - TILE_SIZE / 2) / TILE_SIZE;

		if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
		{
			if (y_val_ > 0)
			{
				int val1 = map_data.tile[y2][x1];
				int val2 = map_data.tile[y2][x2];

				if ((val1 >= THREAT_TILE_MIN && val1 <= THREAT_TILE_MAX) || (val2 >= THREAT_TILE_MIN && val2 <= THREAT_TILE_MAX))
				{
					va_cham_no = true;
					come_back_time_++;
				}

			}

			else if (y_val_ < 0)
			{
				int val1 = map_data.tile[y1][x1];
				int val2 = map_data.tile[y1][x2];

				if ((val1 >= THREAT_TILE_MIN && val1 <= THREAT_TILE_MAX) || (val2 >= THREAT_TILE_MIN && val2 <= THREAT_TILE_MAX))
				{
					va_cham_no = true;
					come_back_time_++;
				}

			}
		}
	

	


	x_pos_ += x_val_;
	y_pos_ += y_val_;

	if (x_pos_ < 0)
	{
		x_pos_ = 0;
	}
	else if (x_pos_ + width_frame_ > map_data.max_x_)
	{
		x_pos_ = map_data.max_x_ - width_frame_ - 1;
	}


	if (y_pos_ < 0)
	{
		y_pos_ = 0;
	}
	else if (y_pos_ + height_frame_ > map_data.max_y_)
	{
		y_pos_ = map_data.max_y_ - height_frame_ - 1;
	}

	std::cout << y_pos_ << "\n";
}

void MainObject::IncreaseMoney()
{
	money_count++;
}


void MainObject::UpdateImagePlayer(SDL_Renderer* des)
{
		if (regime_type_.NORMAL_ == 1)
		{
			LoadImg("img//lap_phuong_3.png", des);
		}
		else if (regime_type_.ROUND_ == 1)
		{
			LoadImg("img//khoi_tron_quay.png", des);
		}
		else if(regime_type_.FLAPPY_ == 1)
		{
			LoadImg("img//flappy_player.png", des);
		}
		else if (regime_type_.ROCKET_ == 1)
		{
			LoadImg("img//rocket_player.png", des);
		}
}


int MainObject::GetRegimeType() const {
	if (regime_type_.FLAPPY_ == 1) {
		return FLAPPY_MODE;
	}
	else
	{
		return NORMAL_MODE;
	}
}

int MainObject::GetX() {
	return (x_pos_ + 32 ) / 32;
}


