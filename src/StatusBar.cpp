#include "StatusBar.h"

void StatusBar::SetAvatar(IMAGE* img_avatar) {
    this->img_avatar = img_avatar;
}

void StatusBar::SetPosition(int x, int y) {
    position.x = x;
    position.y = y;
}

void StatusBar::SetHp(int hp) {
    this->hp = hp;
}

void StatusBar::SetMp(int mp) {
    this->mp = mp;
}

void StatusBar::OnDraw() {
    PutImageAlpha(position.x, position.y, img_avatar);

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
                PutImageAlpha(position.x + 100 + buff_cnt * BUFF_SIZE, position.y + 10 - BUFF_SIZE, &img_buff_hurry);
                break;
            case BuffId::INVISIBLE:
                PutImageAlpha(position.x + 100 + buff_cnt * BUFF_SIZE, position.y + 10 - BUFF_SIZE, &img_buff_invisible);
                break;
            case BuffId::RECOVER_HP:
                PutImageAlpha(position.x + 100 + buff_cnt * BUFF_SIZE, position.y + 10 - BUFF_SIZE, &img_buff_recover_hp);
                break;
            case BuffId::RECOVER_MP:
                PutImageAlpha(position.x + 100 + buff_cnt * BUFF_SIZE, position.y + 10 - BUFF_SIZE, &img_buff_recover_mp);
                break;
            default:
                break;
        }
        ++buff_cnt;
    }
}
