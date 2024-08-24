#include "BubblesBullet.h"

BubblesBullet::BubblesBullet(bool is_ex) {
    this->is_ex = is_ex;
    valid = false;

    if(is_ex){
        animation.SetAtlas(&atlas_bubbles_ex);
        size.x = 384, size.y = 384;
        radius = 140;
        sin45_r = 100;
        damage = 20;
        bullet_size *= 2;
    }else{
        animation.SetAtlas(&atlas_bubbles);
        size.x = 192, size.y = 192;
    }

    animation.SetInterval(100);
    animation.SetLoop(false);
    animation.SetOnFinish([&](){
        can_remove = true;
        valid = false;
    });

    timer_valid.SetWaitTime(100 * 3);
    timer_valid.SetOneShot(true);
    timer_valid.SetCallback([&](){
        valid = true;
    });
}

auto BubblesBullet::GetDamage() const -> int {
    return damage * collision_count;
}

void BubblesBullet::SetPosition(const Vector2& position) {
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

void BubblesBullet::OnCollide() {
    Bullet::OnCollide();
    collision_count = 0;
}

auto BubblesBullet::CheckCollision(const Vector2& position, const Vector2& size) -> bool {
    if(!valid) return false;

    for(int i = 0; i < 8; ++i) {
        if(check_collision_helper(pos[i], position, size)) {
            collision_count++;
        }
    }

    return collision_count > 0;
}

void BubblesBullet::OnUpdate(int delta) {
    animation.OnUpdate(delta);
    timer_valid.OnUpdate(delta);
}

void BubblesBullet::OnDraw(const Camera& camera) const {
    animation.OnDraw(camera, (int)position.x - size.x / 2, (int)position.y - size.y / 2);

    if(is_debug && valid) {
        setfillcolor(RGB(0, 0, 255));
        solidcircle(position.x, position.y, 3);
        
        setlinecolor(RGB(255, 255, 255));
        for(int i = 0; i < 8; ++i) {
            rectangle(pos[i].x + collision_offset.x, pos[i].y + collision_offset.y,
                pos[i].x + bullet_size.x - collision_offset.x, pos[i].y + bullet_size.y - collision_offset.y);
        }
    }
}

auto BubblesBullet::check_collision_helper(const Vector2& position_bullet, const Vector2& position_target, const Vector2& size) const -> bool {
    bool is_collide_x = (std::max(position_bullet.x + bullet_size.x, position_target.x + size.x)
        - std::min(position_bullet.x, position_target.x)) <= (bullet_size.x + size.x);
    bool is_collide_y = (std::max(position_bullet.y + bullet_size.y, position_target.y + size.y)
        - std::min(position_bullet.y, position_target.y)) <= (bullet_size.y + size.y);

    return is_collide_x && is_collide_y;
}