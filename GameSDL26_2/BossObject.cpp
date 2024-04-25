#include "BossObject.h"
#include "SDL_image.h" 

void LaserEffect::Update(SDL_Renderer* des) {
    Uint32 current_time = SDL_GetTicks();

    if (current_time - last_toggle_time_ >= toggle_interval_) {
        visible_ = !visible_;
        last_toggle_time_ = current_time;
        start_fade_time_ = current_time;
    }
    if (visible_ && current_time - start_fade_time_ >= fade_duration_) {
        visible_ = false;
    }
}



BossObject::BossObject():
laser_effect_(1000, 2000)
{
    frame_ = 0;
    x_pos_ = 1060;
    y_pos_ = 200;
    x_val_ = 0;
    y_val_ = 0;
    width_frame_ = 0;
    height_frame_ = 0;
    map_x_ = 0;
    map_y_ = 0;
    last_update_time_ = SDL_GetTicks();
    move_timer_ = 0;
    is_active_ = false;
    MOVE_DOWN = true;
    MOVE_UP = false;
    Time_of_appearance = false;
    
}


BossObject::~BossObject() {}


bool BossObject::LoadImg(std::string path, SDL_Renderer* screen)
{
    bool ret = BaseObject::LoadImg(path, screen);
    if (ret) {
        width_frame_ = rect_.w;
        height_frame_ = rect_.h;
    }
    return ret;
}

SDL_Rect BossObject::GetRectFrame()
{
    SDL_Rect rect;
    rect.x = rect_.x;
    rect.y = rect_.y;
    rect.w = width_frame_;
    rect.h = height_frame_;
    return rect;
}

void BossObject::set_clips()
{
    if (width_frame_ > 0 && height_frame_ > 0) {
        for (int i = 0; i < 5; ++i) {
            frame_clip_[i].x = i * width_frame_;
            frame_clip_[i].y = 0;
            frame_clip_[i].w = width_frame_;
            frame_clip_[i].h = height_frame_;
        }
    }
}

void BossObject::Show(SDL_Renderer* des)
{
    UpdateImageBoss(des);

    /*rect_.x = x_pos_ - map_x_;
    rect_.y = y_pos_ - map_y_;

    SDL_Rect* current_clip = &frame_clip_[frame_];
    SDL_Rect renderQuad = { rect_.x, rect_.y,width_frame_,height_frame_ };

    SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);*/

    rect_.x = x_pos_ - map_x_;
    rect_.y = y_pos_ - map_y_;

    SDL_Rect* current_clip = nullptr;
    SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };

    SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);

    // Update và vẽ laser
    UpdateLaserEffect(des);

    if (laser_effect_.IsVisible()) {
        RenderBlueTrail(des);
    }
}


void BossObject::DoBoss(Map& map_data)
{
    int current_time = SDL_GetTicks();


    move_timer_ += current_time - last_update_time_;

    if (move_timer_ >= MOVE_UP_TIME && MOVE_DOWN) {
        // Move up
        y_pos_ -= 200;
        MOVE_DOWN = false;
        MOVE_UP = true; 
        move_timer_ = 0; 
    }
    else if (move_timer_ >= MOVE_DOWN_TIME && MOVE_UP) {
        // Move down
        y_pos_ += 200; 
        MOVE_UP = false;
        MOVE_DOWN = true; 
        move_timer_ = 0; 
    }

    last_update_time_ = current_time;
}

void BossObject::RenderBlueTrail(SDL_Renderer* des) {

    int trailStartX = x_pos_ + width_frame_ - 360; 
    int trailStartY = y_pos_ + 5 * height_frame_ / 8; 

    int trailLength = 1000; 
    int trailWidth = 96; 

    int red = 64; 
    int green = 224; 
    int blue = 208; 
    int alpha = 255;

    int pulsationPeriod = 50; 
    int pulsationAmplitude = 50; 

    int pulsatingGreen = green + pulsationAmplitude * std::sin(SDL_GetTicks() * 2 * M_PI / pulsationPeriod);

    for (int i = 0; i < trailLength; ++i) {
        alpha = 255 - (255 * i / trailLength);
        SDL_SetRenderDrawColor(des, red, pulsatingGreen, blue, alpha);
        SDL_Rect trailRect = { trailStartX - i, trailStartY - trailWidth / 2, trailWidth , trailWidth };
        SDL_RenderFillRect(des, &trailRect);
    }
    trailRectquad = { trailStartX - 1000, trailStartY - 96, trailWidth, trailWidth };

}


void BossObject::UpdateImageBoss(SDL_Renderer* des)
{
    LoadImg("img//boss_1.png", des);
}

void BossObject::UpdateLaserEffect(SDL_Renderer* des)
{
    laser_effect_.Update(des); 
}

SDL_Rect BossObject::GetRectlaser()
{
    std::cout << trailRectquad.x << "\\\\\\\\" << trailRectquad.y << "\n";
    return trailRectquad;
}