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

    head_texture_ = nullptr;
    jaw_texture_ = nullptr;

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

    time_begin_boss_ = 0;

    head_x_pos_ = 1020;
    head_y_pos_ = 200;
    head_width_ = 276;
    head_height_ = 260;

    jaw_x_pos_ = 1000;
    jaw_y_pos_ = 476;
    jaw_width_ = 276;
    jaw_height_ = 123;

    head_rotation_angle_ = 0.0;
    jaw_rotation_angle_ = 0.0;
}


BossObject::~BossObject() {
}


void BossObject::SplitImage(SDL_Renderer* des) {
    // Tạo đối tượng SDL_Surface để tải ảnh đầu
    SDL_Surface* head_surface = IMG_Load("img//boss_tren.png");
    if (!head_surface) {
        printf("Failed to load head surface: %s\n", IMG_GetError());
        return;
    }

    // Đặt màu chính trong ảnh là màu trong suốt
    SDL_SetColorKey(head_surface, SDL_TRUE, SDL_MapRGB(head_surface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));

    // Tạo texture từ surface của ảnh đầu
    head_texture_ = SDL_CreateTextureFromSurface(des, head_surface);
    if (!head_texture_) {
        printf("Failed to create texture for head surface: %s\n", SDL_GetError());
        SDL_FreeSurface(head_surface);
        return;
    }

    // Giải phóng surface sau khi tạo texture
    SDL_FreeSurface(head_surface);

    // Tạo đối tượng SDL_Surface để tải ảnh cằm
    SDL_Surface* jaw_surface = IMG_Load("img//boss_duoi.png");
    if (!jaw_surface) {
        printf("Failed to load jaw surface: %s\n", IMG_GetError());
        // Giải phóng surface đã tải nếu có lỗi
        if (head_texture_) {
            SDL_DestroyTexture(head_texture_);
            head_texture_ = NULL;
        }
        return;
    }

    // Đặt màu chính trong ảnh là màu trong suốt
    SDL_SetColorKey(jaw_surface, SDL_TRUE, SDL_MapRGB(jaw_surface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));

    // Tạo texture từ surface của ảnh cằm
    jaw_texture_ = SDL_CreateTextureFromSurface(des, jaw_surface);
    if (!jaw_texture_) {
        printf("Failed to create texture for jaw surface: %s\n", SDL_GetError());
        SDL_FreeSurface(jaw_surface);
        // Giải phóng surface đã tải nếu có lỗi
        if (head_texture_) {
            SDL_DestroyTexture(head_texture_);
            head_texture_ = NULL;
        }
        return;
    }

    // Giải phóng surface sau khi tạo texture
    SDL_FreeSurface(jaw_surface);
}





void BossObject::UpdateRotation() {
    // Cập nhật góc quay cho đầu theo chiều kim đồng hồ
    head_rotation_angle_ += rotation_speed_;
    if (head_rotation_angle_ >= max_rotation_angle_) {
        head_rotation_angle_ = max_rotation_angle_;
    }

    // Cập nhật góc quay cho cằm ngược chiều kim đồng hồ
    jaw_rotation_angle_ -= rotation_speed_;
    if (jaw_rotation_angle_ <= -max_rotation_angle_) {
        jaw_rotation_angle_ = -max_rotation_angle_;
    }



}

void BossObject::Show(SDL_Renderer* des) {
    UpdateRotation();
    // Update và vẽ laser
    UpdateLaserEffect(des);

    if (laser_effect_.IsVisible()) {
        RenderBlueTrail(des);
    }
    // Hiển thị phần đầu với góc quay theo hướng kim đồng hồ
    SDL_Rect head_renderQuad = { head_x_pos_, head_y_pos_, head_width_, head_height_ };
    SDL_RenderCopyEx(des, head_texture_, nullptr, &head_renderQuad, head_rotation_angle_, nullptr, SDL_FLIP_NONE);

    // Hiển thị phần cằm với góc quay ngược kim đồng hồ
    SDL_Rect jaw_renderQuad = { jaw_x_pos_, jaw_y_pos_, jaw_width_, jaw_height_ };
    SDL_RenderCopyEx(des, jaw_texture_, nullptr, &jaw_renderQuad, jaw_rotation_angle_, nullptr, SDL_FLIP_NONE);


    

    // Các hàm khác để vẽ laser, hiện giữ nguyên
}



void BossObject::DoBoss(Map& map_data)
{


    int current_time = SDL_GetTicks();


    move_timer_ += current_time - last_update_time_ + 50;

    if (move_timer_ >= MOVE_UP_TIME && MOVE_DOWN) {
        // Move up
        head_y_pos_ -= 200;
        jaw_y_pos_ -= 200;
        MOVE_DOWN = false;
        MOVE_UP = true; 
        move_timer_ = 0; 
        time_begin_boss_ = 0;
        head_rotation_angle_ = 0.0;
        jaw_rotation_angle_ =0.0;
    }
    else if (move_timer_ >= MOVE_DOWN_TIME && MOVE_UP) {
        // Move down
        head_y_pos_ += 200;
        jaw_y_pos_ += 200;
        MOVE_UP = false;
        MOVE_DOWN = true; 
        move_timer_ = 0; 
        time_begin_boss_ = 0;
        head_rotation_angle_ = 0.0;
        jaw_rotation_angle_ = 0.0;
    }

    last_update_time_ = current_time;
}

void BossObject::RenderBlueTrail(SDL_Renderer* des) {

    int trailStartX = jaw_x_pos_ + width_frame_  + 100;
    int trailStartY = jaw_y_pos_ + 5 * height_frame_ / 8;

    int trailLength = 1200; 
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
    trailRectquad = { 0, trailStartY , trailLength, trailWidth };

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