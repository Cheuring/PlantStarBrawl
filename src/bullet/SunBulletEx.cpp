#include "SunBulletEx.h"

SunBulletEx::SunBulletEx(){
    size.x = 288, size.y = 288;
    collision_offset.x = 65, collision_offset.y = 65;
    damage = 35;

    animation_idle.SetAtlas(&atlas_sun_ex);
    animation_idle.SetInterval(50);

    animation_explode.SetAtlas(&atlas_sun_ex_explode);
    animation_explode.SetInterval(50);
    animation_explode.SetLoop(false);
    animation_explode.SetOnFinish([&](){
        can_remove = true;
    });

    IMAGE* frame_idle = animation_idle.GetCurrentFrame();
    IMAGE* frame_explode = animation_explode.GetCurrentFrame();
    explode_render_offset.x = (frame_idle->getwidth() - frame_explode->getwidth()) / 2.0f;
    explode_render_offset.y = (frame_idle->getheight() - frame_explode->getheight()) / 2.0f;
}

void SunBulletEx::OnCollide() {
    Bullet::OnCollide();

    main_camera.Shake(20, 350);

    mciSendString(_T("play sun_explode_ex from 0"), NULL, 0, NULL);
}

void SunBulletEx::OnUpdate(int delta) {
    if(valid){
        position += velocity * (float)delta;
        animation_idle.OnUpdate(delta);
    }else{
        animation_explode.OnUpdate(delta);
    }

    if(check_if_exceeds_screen()){
        can_remove = true;
    }
}

void SunBulletEx::OnDraw(const Camera& camera) const {
    if(valid){
        animation_idle.OnDraw(camera, (int)position.x, (int)position.y);
    }else{
        animation_explode.OnDraw(camera,
            (int)position.x + explode_render_offset.x,
            (int)position.y + explode_render_offset.y);
    }

    Bullet::OnDraw(camera);
}