#ifndef _NUT_BULLET_H_
#define _NUT_BULLET_H_

#include "Animation.h"
#include "Bullet.h"
#include "Camera.h"
#include "Vector2.h"

extern Atlas atlas_nut_explode;

extern Camera main_camera;

class NutBullet : public Bullet {
public:
    NutBullet() {
        size.x = 288, size.y = 288;
        damage = 40;
        valid = false;

        animation_explode.set_atlas(&atlas_nut_explode);
        animation_explode.set_interval(100);
        animation_explode.set_loop(false);
        animation_explode.set_on_finish([&](){
            can_remove = true;
            main_camera.shake(20, 400);
            mciSendString(_T("play nut_explode from 0"), NULL, 0, NULL);
        });

        timer_valid.set_wait_time(150);
        timer_valid.set_one_shot(true);
        timer_valid.set_callback([&](){
            valid = true;
        });
    }
    ~NutBullet() = default;

    bool check_collision(const Vector2& position, const Vector2& size) override {
        bool is_collide_x = (std::max(this->position.x + this->size.x - this->collision_offset.x, position.x + size.x)
            - std::min(this->position.x + this->collision_offset.x, position.x)) <= (this->size.x + size.x - this->collision_offset.x * 2);
        bool is_collide_y = (std::max(this->position.y + this->size.y - this->collision_offset.y, position.y + size.y)
            - std::min(this->position.y + this->collision_offset.y, position.y)) <= (this->size.y + size.y - this->collision_offset.y * 2);

        return is_collide_x && is_collide_y;
    }

    void on_update(int delta) override {
        animation_explode.on_update(delta);
        timer_valid.on_update(delta);
    }

    void on_draw(const Camera& camera) const override {
        animation_explode.on_draw(camera, (int)position.x, (int)position.y);

        if(valid){
            Bullet::on_draw(camera);
        }
    }
private:
    Animation animation_explode;
    Timer timer_valid;
};


#endif // _NUT_BULLET_H_