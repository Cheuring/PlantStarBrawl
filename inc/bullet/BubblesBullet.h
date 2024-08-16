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
    BubblesBullet(bool is_ex = false) : Bullet() {
        this->is_ex = is_ex;
        valid = false;

        if(is_ex){
            animation.set_atlas(&atlas_bubbles_ex);
            size.x = 384, size.y = 384;
            radius = 140;
            sin45_r = 100;
            damage = 20;
            bullet_size *= 2;
        }else{
            animation.set_atlas(&atlas_bubbles);
            size.x = 192, size.y = 192;
        }

        animation.set_interval(100);
        animation.set_loop(false);
        animation.set_on_finish([&](){
            can_remove = true;
            valid = false;
        });

        timer_valid.set_wait_time(100 * 3);
        timer_valid.set_one_shot(true);
        timer_valid.set_callback([&](){
            valid = true;
        });
    }
    ~BubblesBullet() = default;

    int get_damage() const override {
        return damage * collision_count;
    }

    void set_position(const Vector2& position) override {
        this->position = position;

        pos[0] = {position.x - radius - bullet_size.x / 2, position.y - bullet_size.y / 2};
        pos[1] = {position.x - sin45_r - bullet_size.x / 2, position.y - sin45_r - bullet_size.y / 2};
        pos[2] = {position.x - bullet_size.x / 2, position.y - radius - bullet_size.y / 2};
        pos[3] = {position.x + sin45_r - bullet_size.x / 2, position.y - sin45_r - bullet_size.y / 2};
        pos[4] = {position.x + radius - bullet_size.x / 2, position.y - bullet_size.y / 2};
        pos[5] = {position.x + sin45_r - bullet_size.x / 2, position.y + sin45_r - bullet_size.y / 2};
        pos[6] = {position.x - bullet_size.x / 2, position.y + radius - bullet_size.y / 2};
        pos[7] = {position.x - sin45_r - bullet_size.x / 2, position.y + sin45_r - bullet_size.y / 2};
    }

    void on_collide() override {
        Bullet::on_collide();
        collision_count = 0;
    }

    bool check_collision(const Vector2& position, const Vector2& size) override {
        if(!valid) return false;

        for(int i = 0; i < 8; ++i) {
            if(check_collision_helper(pos[i], position, size)) {
                collision_count++;
            }
        }

        return collision_count > 0;
    }

    void on_update(int delta) override {
        animation.on_update(delta);
        timer_valid.on_update(delta);
    }

    void on_draw(const Camera& camera) const override {
        animation.on_draw(camera, (int)position.x - size.x / 2, (int)position.y - size.y / 2);

        if(is_debug && valid) {
            setfillcolor(RGB(255, 255, 255));
            setlinecolor(RGB(255, 255, 255));
            for(int i = 0; i < 8; ++i) {
                rectangle(pos[i].x + collision_offset.x, pos[i].y + collision_offset.y,
                    pos[i].x + bullet_size.x - collision_offset.x, pos[i].y + bullet_size.y - collision_offset.y);
                solidcircle(pos[i].x + collision_offset.x + bullet_size.x / 2, pos[i].y + collision_offset.y + bullet_size.y / 2, 5);
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
    Vector2 bullet_size = {25, 25};

private:
    bool check_collision_helper(const Vector2& position_bullet, const Vector2& position_target, const Vector2& size) const {
        bool is_collide_x = (std::max(position_bullet.x + this->bullet_size.x, position_target.x + size.x)
            - std::min(position_bullet.x, position_target.x)) <= (this->bullet_size.x + size.x);
        bool is_collide_y = (std::max(position_bullet.y + this->bullet_size.y, position_target.y + size.y)
            - std::min(position_bullet.y, position_target.y)) <= (this->bullet_size.y + size.y);

        return is_collide_x && is_collide_y;
    }
};

#endif // _BUBBLES_BULLET_H_