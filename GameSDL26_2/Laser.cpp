#include "Laser.h"


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
