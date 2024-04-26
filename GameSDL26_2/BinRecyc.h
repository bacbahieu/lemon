//#pragma once
//if (come_map) {
//    LoadMap_1();
//    if (is_map_1_loaded) {
//
//        if (x >= 1195 && x <= 1255 && y >= 268 && y <= 380) {
//            is_map_1_loaded = false;
//            map_1_option.Free();
//            LoadMap_2();
//            std::cout << "RUN Map 2" << std::endl;
//        }
//        else if (x >= RETURN_BUTTON_X && x <= RETURN_BUTTON_X + RETURN_BUTTON_WIDTH &&
//            y >= RETURN_BUTTON_Y && y <= RETURN_BUTTON_Y + RETURN_BUTTON_HEIGHT) {
//            map_1_option.Free();
//            menu_image.Render(renderer);
//            in_first_menu = true;
//            come_map = false; // Reset come_map when returning to the first menu
//        }
//    }
//
//    if (is_map_2_loaded) {
//        if (x >= 1195 && x <= 1255 && y >= 268 && y <= 380) {
//            is_map_2_loaded = false;
//            map_2_option.Free();
//            LoadMap_3();
//            std::cout << "RUN Map 3" << std::endl;
//        }
//        else if (x >= 30 && x <= 90 && y >= 268 && y <= 380) {
//            is_map_2_loaded = false;
//            map_2_option.Free();
//            LoadMap_1();
//        }
//        else if (x >= RETURN_BUTTON_X && x <= RETURN_BUTTON_X + RETURN_BUTTON_WIDTH &&
//            y >= RETURN_BUTTON_Y && y <= RETURN_BUTTON_Y + RETURN_BUTTON_HEIGHT) {
//            map_2_option.Free();
//            menu_image.Render(renderer);
//            in_first_menu = true;
//            come_map = false; // Reset come_map when returning to the first menu
//        }
//    }
//
//    if (is_map_3_loaded) {
//        if (x >= 30 && x <= 90 && y >= 268 && y <= 380) {
//            is_map_3_loaded = false;
//            map_3_option.Free();
//            LoadMap_2();
//        }
//        else if (x >= RETURN_BUTTON_X && x <= RETURN_BUTTON_X + RETURN_BUTTON_WIDTH &&
//            y >= RETURN_BUTTON_Y && y <= RETURN_BUTTON_Y + RETURN_BUTTON_HEIGHT) {
//            map_3_option.Free();
//            menu_image.Render(renderer);
//            in_first_menu = true;
//            come_map = false; // Reset come_map when returning to the first menu
//        }
//        else if (x >= 255 && x <= 1020 && y >= 105 && y <= 300)
//        {
//
//            is_map_3_loaded = false;
//            map_3_option.Free();
//            menu_image.Render(renderer);
//            game_state = BEFORE_RUN;
//        }
//    }
//}