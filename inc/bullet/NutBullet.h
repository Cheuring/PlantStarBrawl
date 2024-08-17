#ifndef _NUT_BULLET_H_
#define _NUT_BULLET_H_

#include "Animation.h"
#include "Bullet.h"
#include "Camera.h"
#include "Vector2.h"

extern Atlas atlas_nut_explode;

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
        });

        timer_valid.set_wait_time(100 * 1.5);
        timer_valid.set_one_shot(true);
        timer_valid.set_callback([&](){
            valid = true;
        });
    }
    ~NutBullet() = default;

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