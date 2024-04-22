#ifndef BOSS_OBJECT_H_
#define BOSS_OBJECT_H_

#include "BaseObject.h"
#include "Laser.h"
#include "SDL.h" 

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

class BossObject : public BaseObject {
private:
    int frame_;
    int x_pos_;
    int y_pos_;
    int x_val_;
    int y_val_;
    int width_frame_;
    int height_frame_;
    int map_x_;
    int map_y_;
    SDL_Rect frame_clip_[5];
    Uint32 last_update_time_;
    Uint32 move_timer_;
    bool is_active_;
    bool MOVE_DOWN;
    bool MOVE_UP;
    bool Time_of_appearance;
    
    LaserEffect laser_effect_; // Thêm laser effect vào BossObject

public:
    static const int MOVE_DOWN_TIME = 5000; // Thời gian để di chuyển xuống (5 giây)
    static const int MOVE_UP_TIME = 5000;
    BossObject();
    ~BossObject();

    bool LoadImg(std::string path, SDL_Renderer* screen);
    SDL_Rect GetRectFrame();
    void set_clips();
    void Show(SDL_Renderer* des);
    void DoBoss(Map& map_data);
    void RenderBlueTrail(SDL_Renderer* des); // Render_laser
    void UpdateImageBoss(SDL_Renderer* des);
    void UpdateLaserEffect(SDL_Renderer* des); // Thêm hàm cập nhật laser effect

    int GetX() const { return x_pos_; } 
    int GetY() const { return y_pos_; } 
    int get_frame_width() const { return width_frame_; } 
    int get_frame_height() const { return height_frame_; } 
};

#endif  // BOSS_OBJECT_H_
