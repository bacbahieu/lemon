
#include "MainObject.h"



MainObject::MainObject()
{
	frame_ = 0;
	x_pos_ = 64; // 11776
	y_pos_ = 960; // 192

	pre_y_pos_ = 960;
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

	on_ground_ = false;
	roi_tudo = false;
	va_cham_no = false;

	map_x_ = 0;
	map_y_ = 0;


	currentTileX = 0;
	previous_start_y = 0;
	come_back_time_ = 0;

	is_player_at_start_position = false;

	money_count = 0;

	explosion_texture_ = nullptr;
}

MainObject::~MainObject()
{
	if (explosion_texture_ != nullptr)
	{
		SDL_DestroyTexture(explosion_texture_);
		explosion_texture_ = nullptr;
	}
}

bool MainObject::LoadImg(std::string path, SDL_Renderer* screen)
{

	bool ret = BaseObject::LoadImg(path, screen);
	if (ret == true)
	{
		width_frame_ = rect_.w / 8;
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


void MainObject::Show(SDL_Renderer* des)
{
	
		UpdateImagePlayer(des);
		if (va_cham_no) {
			ShowExplosion(des);
		}
		else
		{
			if (input_type_.right_ == 1)
			{
				frame_++;
			}
			else
			{
				frame_ = 0;
			}


			if (frame_ >= 8)
			{
				frame_ = 2;
			}

			if (come_back_time_ == 0)
			{
				rect_.x = x_pos_ - map_x_;
				rect_.y = y_pos_ - map_y_;

				SDL_Rect* current_clip = &frame_clip_[frame_];
				SDL_Rect renderQuad = { rect_.x, rect_.y,width_frame_,height_frame_ };

				SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
			}
		}
		
	

}

void MainObject::HandelInputAction(SDL_Event events, SDL_Renderer* screen)
{
	if (regime_type_.NORMAL_ == 1) {
		
		status_ = WALK_RIGHT;
		input_type_.right_ = 1;
		UpdateImagePlayer(screen);


		if (events.type == SDL_KEYDOWN)
		{
			if (events.key.keysym.sym == SDLK_UP || events.key.keysym.sym == SDLK_SPACE)
			{
				if (on_ground_)
				{
					input_type_.jump_ = 1;
				}

			}

		}
		 else if (events.type == SDL_MOUSEBUTTONDOWN)
		{
			if (events.button.button == SDL_BUTTON_LEFT)
			{
				
				if (on_ground_)
				{
					input_type_.jump_ = 1;
				}
			}
		}
		
	}
	else
	{
		if (events.type == SDL_KEYDOWN)
		{
			if (events.key.keysym.sym == SDLK_UP)
			{
				if (on_ground_)
				{
					input_type_.jump_ = 1;
				}

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

		 if (input_type_.right_ == 1)
		{
			x_val_ += PLAYER_SPEED;
		}
		
		 if (regime_type_.NORMAL_ == 1) {
			 y_val_ += 8;
			 if (y_val_ >= MAX_FALL_SPEED) y_val_ = MAX_FALL_SPEED;
			 
			 if (input_type_.jump_ == 1)
			 {
				 if (on_ground_ == true )
				 {
					 y_val_ = -PLAYER_JUMP_VAL;
					 
				 }
				 on_ground_ = false;
				 input_type_.jump_ = 0;
			 }
			
			 
		 }
		 else
		 {
			 y_val_ += 6;
			 if (y_val_ >= MAX_FALL_SPEED_FLAPPY) { y_val_ = MAX_FALL_SPEED_FLAPPY; }

			 if (input_type_.jump_ == 1)
			 {
			     y_val_ = - 36;
				 input_type_.jump_ = 0;
			 }
		 }

		 currentTileX += y_val_;


		CheckToMap(map_data);

		if (va_cham_no) { std::cout << "CA_1 "; }

		CenterEntityOnMap(map_data);

		

		if (on_ground_)
		{
			y_val_ = 0;
		}
		

	}


	if (come_back_time_ > 0)
	{
		va_cham_no = false;
		is_player_at_start_position = true;
		SDL_Delay(500); // Delay for 2000 milliseconds (2 seconds)
		
		come_back_time_--;
		if (come_back_time_ == 0)// Reset again
		{
			x_pos_ = 64; // 64
			y_pos_ = 960;
			x_val_ = 0;
			y_val_ = 0;
			
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


	// map theo chieu doc

	 

		map_data.start_y_ = y_pos_ - (SCREEN_HEIGHT / 2);

		if (map_data.start_y_ < 0)
		{
			map_data.start_y_ = 0;
		}
		else if (map_data.start_y_ + SCREEN_HEIGHT >= map_data.max_y_)
		{
			map_data.start_y_ = map_data.max_y_ - SCREEN_HEIGHT;
		}
		else if(previous_start_y + 128 > y_pos_)
		{
			map_data.start_y_ -= 64;
		}
		else if (previous_start_y + SCREEN_HEIGHT - y_pos_ - TILE_SIZE < 96 )
		{
			map_data.start_y_ += 64;
		}
		else {
			map_data.start_y_ = previous_start_y;
		}

		previous_start_y = map_data.start_y_;
	

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
	x2 = (x_pos_ + x_val_ + width_frame_ - 0.01) / TILE_SIZE;

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
			else if ((val1 >= 5 && val1 <= 10) || (val2 >= 5 && val2 <= 10))
			{
				;
			}
			else 
			{
				if ((val1 != BLANK_TILE && val1 != SPACE_PORTAL_NORMAL_TO_FLAPPY  && val1 != SPACE_PORTAL_FLAPPY_TO_NORMAL)||( val2 != BLANK_TILE && val2 != SPACE_PORTAL_NORMAL_TO_FLAPPY && val2 != SPACE_PORTAL_FLAPPY_TO_NORMAL))
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


	x1 = (x_pos_ + x_val_+32) / TILE_SIZE;
	x2 = (x_pos_ + x_val_ + width_frame_ - 32) / TILE_SIZE;

	y1 = (y_pos_) / TILE_SIZE;
	y2 = (y_pos_ + height_min - 32) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 <= MAX_MAP_Y)
	{
		if (x_val_ > 0) //main object is moving to right
		{

			int val1 = map_data.tile[y1][x2];
			int val2 = map_data.tile[y2][x2];

			 if ((val1 >= 5 && val1 <= 10) || (val2 >= 5 && val2 <= 10))
			 {
				 va_cham_no = true;
				 come_back_time_++;
			 }
			 else if (val1 == SPACE_PORTAL_NORMAL_TO_FLAPPY || val2 == SPACE_PORTAL_NORMAL_TO_FLAPPY)
			 {
				 regime_type_.FLAPPY_ = 1;
				 regime_type_.NORMAL_ = 0;
			 }
			 else if (val1 == SPACE_PORTAL_FLAPPY_TO_NORMAL || val2 == SPACE_PORTAL_FLAPPY_TO_NORMAL)
			 {
				 regime_type_.FLAPPY_ = 0;
				 regime_type_.NORMAL_ = 1;
			 }
		}

	}


	//check vertical

	int width_min = min(width_frame_, TILE_SIZE);
	x1 = (x_pos_) / TILE_SIZE;
	x2 = (x_pos_ + width_min) / TILE_SIZE;

	y1 = (y_pos_ + y_val_) / TILE_SIZE;
	y2 = (y_pos_ + y_val_ + height_frame_ - 0.01) / TILE_SIZE;

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
			else if ((val1 >= 6 && val1 <= 10) || (val2 >= 6 && val2 <= 10))
			{
				;
			}
			else {
				if ((val1 != BLANK_TILE && val1 != SPACE_PORTAL_NORMAL_TO_FLAPPY) || (val2 != BLANK_TILE && val2 != SPACE_PORTAL_NORMAL_TO_FLAPPY))
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
			else if ((val1 >= 6 && val1 <= 10) || (val2 >= 6 && val2 <= 10))
			{
				;
			}
			else {

				if ((val1 != BLANK_TILE && val1 != SPACE_PORTAL_NORMAL_TO_FLAPPY && val1 != SPACE_PORTAL_FLAPPY_TO_NORMAL) || (val2 != BLANK_TILE && val2 != SPACE_PORTAL_NORMAL_TO_FLAPPY && val2 != SPACE_PORTAL_FLAPPY_TO_NORMAL))
				{
					y_pos_ = (y1 + 0.01) * TILE_SIZE;
					y_val_ = 0;
				}
			}
		}
	}


	// check threat 
	x1 = (x_pos_+32) / TILE_SIZE;
	x2 = (x_pos_ + width_min) / TILE_SIZE;

	y1 = (y_pos_ + y_val_) / TILE_SIZE;
	y2 = (y_pos_ + y_val_ + height_frame_ - 32) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
	{
		if (y_val_ > 0)
		{
			int val1 = map_data.tile[y2][x1];
			int val2 = map_data.tile[y2][x2];

			if ((val1 >= 6 && val1 <= 10) || (val2 >= 6 && val2 <= 10))
			 {
				va_cham_no = true;
				come_back_time_++;
			 }
			
		}

		else if (y_val_ < 0)
		{
			int val1 = map_data.tile[y1][x1];
			int val2 = map_data.tile[y1][x2];

		 if ((val1 >= 6 && val1 <= 10) || (val2 >= 6 && val2 <= 10))
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

}

void MainObject::IncreaseMoney()
{
	money_count++;
}


void MainObject::UpdateImagePlayer(SDL_Renderer* des)
{
	if (!va_cham_no) {
		if (regime_type_.NORMAL_ == 1)
		{
			if (roi_tudo)
			{
				LoadImg("img//jump_frame_update.png", des);

			}
			else {
				if (on_ground_)
				{
					LoadImg("img//frame_dichuyen_2.png", des);

				}
				else
				{
					LoadImg("img//jump_frame_update.png", des);

				}
			}

		}
		else
		{
			LoadImg("img//frame_duthuyen_8frame.png", des);
		}
	}
	else 
	{
		LoadImg("img//exp3.png", des);
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