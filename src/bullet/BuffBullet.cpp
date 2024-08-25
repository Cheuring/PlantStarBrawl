#include "BuffBullet.h"

BuffBullet::BuffBullet(int buffNum, int rand_num){
    size.x = 80, size.y = 80;
    collision_offset.x = 14, collision_offset.y = 14;

    velocity.y = 0.1f;
    position.y = -size.y / 2;
    position.x = rand_num % (getwidth() - (int)size.x);

    switch(buffNum){
        case 0:
            setAtlas(&atlas_buffbox_blue);
            buff_id = BuffId::RECOVER_MP;
            break;
        case 1:
            setAtlas(&atlas_buffbox_pink);
            buff_id = BuffId::RECOVER_HP;
            break;
        case 2:
            setAtlas(&atlas_buffbox_yellow);
            buff_id = BuffId::INVISIBLE;
            break;
        case 3:
            setAtlas(&atlas_buffbox_yellow);
            buff_id = BuffId::HURRY;
            break;
        default:
            throw std::runtime_error("Invalid buff number");
            break;
    }
}

auto BuffBullet::GetBuffId() const -> BuffId {
    return buff_id;
}

void BuffBullet::OnCollide() {
    can_remove = true;

    mciSendString(_T("play ui_confirm from 0"), NULL, 0, NULL);
}

void BuffBullet::OnUpdate(int delta) {
    if(valid){
        position += velocity * (float)delta;
        animation_buff.OnUpdate(delta);

        if(check_if_exceeds_screen()){
            can_remove = true;
        }
    }
}

void BuffBullet::OnDraw(const Camera& camera) const {
    if(valid){
        animation_buff.OnDraw(camera, (int)position.x, (int)position.y);
    }

    Bullet::OnDraw(camera);
}