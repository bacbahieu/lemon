#pragma once
#ifndef OP_CREDITS_H
#define OP_CREDITS_H

#include "CommonFunc.h"
#include "BaseObject.h"

BaseObject after_run_1;
BaseObject after_run_2;
BaseObject after_run_3;


BaseObject op_end_0;
BaseObject op_end_1;
BaseObject op_end_2;
BaseObject op_end_3;
BaseObject op_end_4;
BaseObject op_end_5;
BaseObject op_end_6;
BaseObject op_end_7;
BaseObject op_end_8;
BaseObject op_end_9;
BaseObject op_end_10;
BaseObject op_end_11;
BaseObject op_end_12;
BaseObject op_end_black;

int op_time = 14000;

bool Load_Op_0(SDL_Renderer* des) {
    bool ret = op_end_0.LoadImg("img//option_end.png", g_screen);
    if (!ret) {
        return false;
    }

    op_end_0.Render(des);
    return true;

}

bool Load_Op_1(SDL_Renderer* des) {
    bool ret = op_end_1.LoadImg("img//option_end_1.png", g_screen);
    if (!ret) {
        return false;
    }

    op_end_1.Render(des);
    return true;

}

bool Load_Op_2(SDL_Renderer* des) {
    bool ret = op_end_2.LoadImg("img//option_end_2.png", g_screen);
    if (!ret) {
        return false;
    }

    op_end_2.Render(des);
    return true;

}

bool Load_Op_3(SDL_Renderer* des) {
    bool ret = op_end_3.LoadImg("img//option_end_3.png", g_screen);
    if (!ret) {
        return false;
    }

    op_end_3.Render(des);
    return true;

}

bool Load_Op_4(SDL_Renderer* des) {
    bool ret = op_end_4.LoadImg("img//option_end_4.png", g_screen);
    if (!ret) {
        return false;
    }

    op_end_4.Render(des);
    return true;

}

bool Load_Op_5(SDL_Renderer* des) {
    bool ret = op_end_5.LoadImg("img//option_end_5.png", g_screen);
    if (!ret) {
        return false;
    }

    op_end_5.Render(des);
    return true;

}

bool Load_Op_6(SDL_Renderer* des) {
    bool ret = op_end_6.LoadImg("img//option_end_6.png", g_screen);
    if (!ret) {
        return false;
    }

    op_end_6.Render(des);
    return true;

}

bool Load_Op_7(SDL_Renderer* des) {
    bool ret = op_end_7.LoadImg("img//option_end_7.png", g_screen);
    if (!ret) {
        return false;
    }

    op_end_7.Render(des);
    return true;

}

bool Load_Op_8(SDL_Renderer* des) {
    bool ret = op_end_8.LoadImg("img//option_end_8.png", g_screen);
    if (!ret) {
        return false;
    }

    op_end_8.Render(des);
    return true;

}

bool Load_Op_9(SDL_Renderer* des) {
    bool ret = op_end_9.LoadImg("img//option_end_9.png", g_screen);
    if (!ret) {
        return false;
    }

    op_end_9.Render(des);
    return true;

}

bool Load_Op_10(SDL_Renderer* des) {
    bool ret = op_end_10.LoadImg("img//option_end_10.png", g_screen);
    if (!ret) {
        return false;
    }

    op_end_10.Render(des);
    return true;

}

bool Load_Op_11(SDL_Renderer* des) {
    bool ret = op_end_11.LoadImg("img//option_end_11.png", g_screen);
    if (!ret) {
        return false;
    }

    op_end_11.Render(des);
    return true;

}

bool Load_Op_12(SDL_Renderer* des) {
    bool ret = op_end_12.LoadImg("img//option_end_12.png", g_screen);
    if (!ret) {
        return false;
    }

    op_end_12.Render(des);
    return true;

}

bool Load_Op_black(SDL_Renderer* des) {
    bool ret = op_end_black.LoadImg("img//option_end_black.png", g_screen);
    if (!ret) {
        return false;
    }

    op_end_black.Render(des);
    return true;

}

bool Load_After_1(SDL_Renderer* des) {
    bool ret = after_run_1.LoadImg("img//end_img_1.png", g_screen);
    if (!ret) {
        std::cout << "Failed to load end_img_1!" << std::endl;
        return false;
    }

    after_run_1.Render(des);

    return true;

}

bool Load_After_2(SDL_Renderer* des) {
    bool ret = after_run_2.LoadImg("img//end_img_2.png", g_screen);
    if (!ret) {
        std::cout << "Failed to load end_img_2!" << std::endl;
        return false;
    }

    after_run_2.Render(des);
 

    return true;

}


bool Load_After_3(SDL_Renderer* des) {
    bool ret = after_run_3.LoadImg("img//end_img_3.png", g_screen);
    if (!ret) {
        std::cout << "Failed to load end_img_3!" << std::endl;
        return false;
    }

    after_run_3.Render(des);

    return true;

}


void Load_Img_After_Win(SDL_Renderer* des) {
    op_time -= 25;

    if (op_time > 15500) {
        Load_After_1(des);
        after_run_1.Render(des);
    }
    else if (op_time > 15000 && op_time <= 15500) {
        after_run_1.Free();
        Load_After_2(des);
        after_run_2.Render(des);
    }
    else if (op_time > 14500 && op_time <= 15000) {
        after_run_2.Free();
        Load_After_3(des);
        after_run_3.Render(des);
    }
    else if (op_time > 14000 && op_time <= 14500) {


        std::cout << "OPTION_______";
    }
    else if (op_time > 13000 && op_time <= 14000) {
        after_run_3.Free();
        Load_Op_0(des);
        op_end_0.Render(des);
    }
    else if (op_time > 12000 && op_time <= 13000) {
        op_end_0.Free();
        Load_Op_black(des);
        op_end_black.Render(des);
    }
    else if (op_time > 11800 && op_time <= 12000) {
        op_end_black.Free();
        Load_Op_1(des);
        op_end_1.Render(des);
    }
    else if (op_time > 11600 && op_time <= 11800) {
        op_end_1.Free();
        Load_Op_2(des);
        op_end_2.Render(des);
    }
    else if (op_time > 11400 && op_time <= 11600) {
        op_end_2.Free();
        Load_Op_3(des);
        op_end_3.Render(des);
    }
    else if (op_time > 11200 && op_time <= 11400) {
        op_end_3.Free();
        Load_Op_4(des);
        op_end_4.Render(des);
    }
    else if (op_time > 11000 && op_time <= 11200) {
        op_end_4.Free();
        Load_Op_5(des);
        op_end_5.Render(des);
    }
    else if (op_time > 10800 && op_time <= 11000) {
        op_end_5.Free();
        Load_Op_6(des);
        op_end_6.Render(des);
    }
    else if (op_time > 10600 && op_time <= 10800) {
        op_end_6.Free();
        Load_Op_7(des);
        op_end_7.Render(des);
    }
    else if (op_time > 9100 && op_time <= 10600) {
        op_end_7.Free();
        Load_Op_8(des);
        op_end_8.Render(des);
    }
    else if (op_time > 7600 && op_time <= 9100) {
        op_end_8.Free();
        Load_Op_9(des);
        op_end_9.Render(des);
    }
    else if (op_time > 6100 && op_time <= 7600) {
        op_end_9.Free();
        Load_Op_10(des);
        op_end_10.Render(des);
    }
    else if (op_time > 5500 && op_time <= 6100) {
        op_end_10.Free();
        Load_Op_black(des);
        op_end_black.Render(des);
    }
    else if (op_time > 4000 && op_time <= 5500) {
        op_end_black.Free();
        Load_Op_11(des);
        op_end_11.Render(des);
    }
    else if (op_time > 2500 && op_time <= 4000) {
        op_end_11.Free();
        Load_Op_12(des);
        op_end_12.Render(des);
    }
    else {
        op_end_12.Free();
    }
    std::cout << op_time << std::endl;
}

#endif // OP_CREDITS_H
