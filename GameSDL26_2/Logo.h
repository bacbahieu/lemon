#ifndef LOGO_H
#define LOGO_H

#include "CommonFunc.h"
#include "BaseObject.h"

BaseObject logo_UET_start;
int time_run_logo = 5000;

bool LoadLogoUET(SDL_Renderer* des) {
    bool ret = logo_UET_start.LoadImg("img//logo_UET.png", des);
    if (!ret) {
        return false;
    }

    logo_UET_start.SetRect((SCREEN_WIDTH - logo_UET_start.GetWidth()) / 2, (SCREEN_HEIGHT - logo_UET_start.GetHeight()) / 2);

    return true;
}

#endif // LOGO_H
