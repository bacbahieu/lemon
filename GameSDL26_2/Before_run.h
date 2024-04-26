#ifndef BEFORE_RUN_H
#define BEFORE_RUN_H

#include "CommonFunc.h"
#include "BaseObject.h"

BaseObject before_run_3mi;
BaseObject before_run_2mi;
BaseObject before_run_1mi;

bool bf_run_3mi = false;
bool bf_run_2mi = false;
bool bf_run_1mi = false;

int bf_run_timer = 3000; // time_count_down

bool Load_Before_3(SDL_Renderer* des) {
    bool ret = before_run_3mi.LoadImg("img//Before_Run_3mi.png", des);
    if (!ret) {
        std::cout << "Failed to load bf_run 3!" << std::endl;
        return false;
    }

    before_run_3mi.Render(des);

    bf_run_3mi = true;

    return true;

}

bool Load_Before_2(SDL_Renderer* des) {
    bool ret = before_run_2mi.LoadImg("img//Before_Run_2mi.png", des);
    if (!ret) {
        std::cout << "Failed to load bf_run 2!" << std::endl;
        return false;
    }

    before_run_2mi.Render(des);

    bf_run_2mi = true;

    return true;

}

bool Load_Before_1(SDL_Renderer* des) {
    bool ret = before_run_1mi.LoadImg("img//Before_Run_1mi.png", des);
    if (!ret) {
        std::cout << "Failed to load bf_run 1!" << std::endl;
        return false;
    }

    before_run_1mi.Render(des);
    std::cout << "RUN 1111111" << std::endl;
    bf_run_1mi = true;

    return true;

}

void Load_Bf_RUN(SDL_Renderer* des) {
    bf_run_timer -= 100;
    std::cout << bf_run_timer << std::endl;
    if (bf_run_timer > 2000) {
        Load_Before_3(des);
        before_run_3mi.Render(des);
    }
    else if (bf_run_timer <= 2000 && bf_run_timer > 1000) {
        before_run_3mi.Free();
        Load_Before_2(des);
        before_run_2mi.Render(des);
    }
    else if (bf_run_timer <= 1000 && bf_run_timer > 0) {
        before_run_2mi.Free();
        Load_Before_1(des);
        before_run_1mi.Render(des);
    }
    else if (bf_run_timer <= 0) {
        before_run_1mi.Free();
        game_state = PLAYING;
    }
}

#endif // BEFORE_RUN_H
