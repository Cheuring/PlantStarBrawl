#include "SunBullet.h"

SunBullet::SunBullet() {
    size.x = 96, size.y = 96;
    collision_offset.x = 25, collision_offset.y = 25;
    damage = 25;

    animation_idle.SetAtlas(&atlas_sun);
    animation_idle.SetInterval(50);

    animation_explode.SetAtlas(&atlas_sun_explode);
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

void SunBullet::OnCollide() {
    Bullet::OnCollide();

    main_camera.Shake(5, 200);

    mciSendString(_T("play sun_explode from 0"), NULL, 0, NULL);
}

void SunBullet::OnUpdate(int delta) {
    if(valid){
        velocity.y += gravity * delta;
        position += velocity * (float)delta;
        animation_idle.OnUpdate(delta);
    }else{
        animation_explode.OnUpdate(delta);
    }

    if(check_if_exceeds_screen()){
        can_remove = true;
    }
}

void SunBullet::OnDraw(const Camera& camera) const {
    if(valid){
        animation_idle.OnDraw(camera, (int)position.x, (int)position.y);
    }else{
        animation_explode.OnDraw(camera,
            (int)position.x + explode_render_offset.x,
            (int)position.y + explode_render_offset.y);
    }

    Bullet::OnDraw(camera);
}