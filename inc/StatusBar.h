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

    void SetAvatar(IMAGE* img_avatar);
    void SetPosition(int x, int y);
    void SetHp(int hp);
    void SetMp(int mp);
    void OnDraw();

private:
    const int width = 275;

    int hp = 0;
    int mp = 0;
    POINT position = {0, 0};
    IMAGE* img_avatar = nullptr;

    const std::list<BuffId>& buff_list;
};

#endif // _STATUS_BAR_H_