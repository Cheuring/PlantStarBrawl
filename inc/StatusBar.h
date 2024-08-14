#ifndef _STATUS_BAR_H_
#define _STATUS_BAR_H_

#include <graphics.h>

#include "util.h"


class StatusBar {
public:
    StatusBar() = default;
    ~StatusBar() = default;

    void set_avatar(IMAGE* img_avatar) {
        this->img_avatar = img_avatar;
    }

    void set_position(int x, int y) {
        position.x = x;
        position.y = y;
    }

    void set_hp(int hp) {
        this->hp = hp;
    }

    void set_mp(int mp) {
        this->mp = mp;
    }

    void on_draw(){
        put_image_alpha(position.x, position.y, img_avatar);

        setfillcolor(RGB(5, 5, 5));
        solidroundrect(position.x + 100, position.y + 10, position.x + 100 + width + 3 * 2, position.y + 36, 8, 8);
        solidroundrect(position.x + 100, position.y + 45, position.x + 100 + width + 3 * 2, position.y + 71, 8, 8);
        setfillcolor(RGB(67, 47, 47));
        solidroundrect(position.x + 100, position.y + 10, position.x + 100 + 3 + width, position.y + 33, 8, 8);
        solidroundrect(position.x + 100, position.y + 45, position.x + 100 + 3 + width, position.y + 68, 8, 8);

        float hp_bar_width = width * std::max(0, hp) / 100.f;
        float mp_bar_width = width * std::min(100, mp) / 100.f;

        setfillcolor(RGB(197, 61, 67));
        solidroundrect(position.x + 100, position.y + 10, position.x + 100 + 3 + hp_bar_width, position.y + 33, 8, 8);
        setfillcolor(RGB(83, 131, 195));
        solidroundrect(position.x + 100, position.y + 45, position.x + 100 + 3 + mp_bar_width, position.y + 68, 8, 8);
    }


private:
    const int width = 275;

    int hp = 0;
    int mp = 0;
    POINT position = {0, 0};
    IMAGE* img_avatar = nullptr;
};

#endif // _STATUS_BAR_H_