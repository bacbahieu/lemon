#ifndef LASER_H
#define LASER_H

#include <SDL.h>

class LaserEffect {
private:
    bool visible_; // Trạng thái hiện tại của laser
    Uint32 last_toggle_time_; // Thời gian cuối cùng laser được chuyển đổi
    Uint32 toggle_interval_; // Khoảng thời gian giữa các chuyển đổi
    Uint32 fade_duration_; // Thời gian làm dần dần hiển thị laser
    Uint32 start_fade_time_; // Thời gian bắt đầu làm dần dần hiển thị laser
public:
    LaserEffect(Uint32 toggle_interval, Uint32 fade_duration) :
        visible_(false), last_toggle_time_(0), toggle_interval_(toggle_interval),
        fade_duration_(fade_duration), start_fade_time_(0) {}

    void Update(SDL_Renderer* des); 
    bool IsVisible() const { return visible_; }
};

#endif // LASER_H
