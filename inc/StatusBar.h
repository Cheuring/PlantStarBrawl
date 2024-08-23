#ifndef _STATUS_BAR_H_
#define _STATUS_BAR_H_

#include <graphics.h>
#include <list>
#include <unordered_map>

#include "BuffId.h"
#include "util.h"

#define BUFF_SIZE 80

extern IMAGE img_buff_hurry;
extern IMAGE img_buff_invisible;
extern IMAGE img_buff_recover_hp;
extern IMAGE img_buff_recover_mp;

class StatusBar {
public:
    StatusBar(const std::list<BuffId>& buff_list) : buff_list(buff_list) {};
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

        int buff_cnt = 0;
        for(auto& buff_id: buff_list){
            switch(buff_id) {
                case BuffId::HURRY:
                    put_image_alpha(position.x + 100 + buff_cnt * BUFF_SIZE, position.y + 10 - BUFF_SIZE, &img_buff_hurry);
                    break;
                case BuffId::INVISIBLE:
                    put_image_alpha(position.x + 100 + buff_cnt * BUFF_SIZE, position.y + 10 - BUFF_SIZE, &img_buff_invisible);
                    break;
                case BuffId::RECOVER_HP:
                    put_image_alpha(position.x + 100 + buff_cnt * BUFF_SIZE, position.y + 10 - BUFF_SIZE, &img_buff_recover_hp);
                    break;
                case BuffId::RECOVER_MP:
                    put_image_alpha(position.x + 100 + buff_cnt * BUFF_SIZE, position.y + 10 - BUFF_SIZE, &img_buff_recover_mp);
                    break;
                default:
                    break;
            }
            ++buff_cnt;
        }
    }


private:
    const int width = 275;

    int hp = 0;
    int mp = 0;
    POINT position = {0, 0};
    IMAGE* img_avatar = nullptr;

    const std::list<BuffId>& buff_list;
};

#endif // _STATUS_BAR_H_