#include "PeaBullet.h"

PeaBullet::PeaBullet() {
    size.x = 64, size.y = 64;
    collision_offset.x = 25, collision_offset.y = 25;
    damage = 20;

    animation_break.SetAtlas(&atlas_pea_break);
    animation_break.SetInterval(100);
    animation_break.SetLoop(false);
    animation_break.SetOnFinish([&](){
        can_remove = true;
    });
}

void PeaBullet::OnCollide() {
    Bullet::OnCollide();

    switch( rand() % 3 ) {
        case 0:
            mciSendString(_T("play pea_break_1 from 0"), NULL, 0, NULL);
            break;
        case 1:
            mciSendString(_T("play pea_break_2 from 0"), NULL, 0, NULL);
            break;
        case 2:
            mciSendString(_T("play pea_break_3 from 0"), NULL, 0, NULL);
            break;
    }
}

void PeaBullet::OnUpdate(int delta) {
    position += velocity * (float)delta;

    if(!valid){
        animation_break.OnUpdate(delta);
    }

    if(check_if_exceeds_screen()){
        can_remove = true;
    }
}

void PeaBullet::OnDraw(const Camera& camera) const {
    if(valid){
        PutImageAlpha(camera, (int)position.x, (int)position.y, &img_pea);
    }else{
        animation_break.OnDraw(camera, (int)position.x, (int)position.y);
    }

    Bullet::OnDraw(camera);
}