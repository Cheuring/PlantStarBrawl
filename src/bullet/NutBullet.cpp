#include "NutBullet.h"

NutBullet::NutBullet() {
    size.x = 288, size.y = 288;
    damage = 40;
    valid = false;

    animation_explode.SetAtlas(&atlas_nut_explode);
    animation_explode.SetInterval(100);
    animation_explode.SetLoop(false);
    animation_explode.SetOnFinish([&](){
        can_remove = true;
    });

    timer_valid.SetWaitTime(100 * 1.5);
    timer_valid.SetOneShot(true);
    timer_valid.SetCallback([&](){
        valid = true;
    });
}

void NutBullet::OnUpdate(int delta) {
    animation_explode.OnUpdate(delta);
    timer_valid.OnUpdate(delta);
}

void NutBullet::OnDraw(const Camera& camera) const {
    animation_explode.OnDraw(camera, (int)position.x, (int)position.y);

    if(valid){
        Bullet::OnDraw(camera);
    }
}