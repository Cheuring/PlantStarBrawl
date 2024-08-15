#ifndef _BUBBLES_BULLET_H_
#define _BUBBLES_BULLET_H_

#include <graphics.h>

#include "Animation.h"
#include "Bullet.h"
#include "Timer.h"
#include "Vector2.h"

extern Atlas atlas_bubbles;
extern Atlas atlas_bubbles_ex;

class BubblesBullet : public Bullet {
public:
    BubblesBullet(const Vector2& position, bool is_ex = false) : Bullet() {
        this->position = position;
        this->is_ex = is_ex;
        valid = false;

        if(is_ex){
            animation.set_atlas(&atlas_bubbles_ex);
            size.x = 50, size.y = 50;
            radius = 140;
            sin45_r = 100;
        }else{
            animation.set_atlas(&atlas_bubbles);
            size.x = 25, size.y = 25;
        }

        animation.set_interval(50);
        animation.set_loop(false);
        animation.set_on_finish([&](){
            can_remove = true;
            valid = false;
        });

        timer_valid.set_wait_time(150);
        timer_valid.set_one_shot(true);
        timer_valid.set_callback([&](){
            valid = true;
        });

        pos[0] = {position.x - radius - size.x / 2, position.y - size.y / 2};
        pos[1] = {position.x - sin45_r - size.x / 2, position.y - sin45_r - size.y / 2};
        pos[2] = {position.x - size.x / 2, position.y - radius - size.y / 2};
        pos[3] = {position.x + sin45_r - size.x / 2, position.y - sin45_r - size.y / 2};
        pos[4] = {position.x + radius - size.x / 2, position.y - size.y / 2};
        pos[5] = {position.x + sin45_r - size.x / 2, position.y + sin45_r - size.y / 2};
        pos[6] = {position.x - size.x / 2, position.y + radius - size.y / 2};
        pos[7] = {position.x - sin45_r - size.x / 2, position.y + sin45_r - size.y / 2};
    }
    ~BubblesBullet() = default;

    void on_collide() override {
        if(callback == nullptr) return;

        for(int i = 0; i < collision_count; ++i) {
            callback();
        }

        collision_count = 0;
    }

    bool check_collision(const Vector2& position, const Vector2& size) override {
        if(!valid) return false;

        for(int i = 0; i < 8; ++i) {
            if(check_collision_helper(pos[i], position, size)) {
                collision_count++;
            }
        }
    }

    void on_update(int delta) override {
        animation.on_update(delta);
        timer_valid.on_update(delta);
    }

    void on_draw(const Camera& camera) const override {
        animation.on_draw(camera, (int)position.x, (int)position.y);

        if(is_debug && valid) {
            setfillcolor(RGB(255, 255, 255));
            setlinecolor(RGB(255, 255, 255));
            for(int i = 0; i < 8; ++i) {
                rectangle(pos[i].x + collision_offset.x, pos[i].y + collision_offset.y,
                    pos[i].x + size.x - collision_offset.x, pos[i].y + size.y - collision_offset.y);
                solidcircle(pos[i].x + collision_offset.x + size.x / 2, pos[i].y + collision_offset.y + size.y / 2, 5);
            }
        }
    }
    
private:
    int radius = 70;
    int sin45_r = 50;

    bool is_ex = false;
    int collision_count = 0;

    Animation animation;
    Timer timer_valid;

    Vector2 pos[8];

private:
    bool check_collision_helper(const Vector2& position_bullet, const Vector2& position_target, const Vector2& size) const {
        bool is_collide_x = (std::max(position_bullet.x + this->size.x, position_target.x + size.x)
            - std::min(position_bullet.x, position_target.x)) <= (this->size.x + size.x);
        bool is_collide_y = (std::max(position_bullet.y + this->size.y, position_target.y + size.y)
            - std::min(position_bullet.y, position_target.y)) <= (this->size.y + size.y);

        return is_collide_x && is_collide_y;
    }
};

#endif // _BUBBLES_BULLET_H_