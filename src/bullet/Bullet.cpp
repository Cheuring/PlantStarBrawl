#include "Bullet.h"

void Bullet::SetDamage(int damage) {
    this->damage = damage;
}

auto Bullet::GetDamage() const -> int {
    return damage;
}

void Bullet::SetPosition(int x, int y) {
    position.x = x;
    position.y = y;
}

void Bullet::SetPosition(const Vector2<int>& position) {
    this->position = position;
}

auto Bullet::GetPosition() const -> const Vector2<int>& {
    return position;
}

auto Bullet::GetSize() const -> const Vector2<int>& {
    return size;
}

void Bullet::SetVelocity(float x, float y) {
    velocity.x = x;
    velocity.y = y;
}

void Bullet::SetVelocity(const Vector2<float>& velocity) {
    this->velocity = velocity;
}

void Bullet::SetCollideTarget(PlayerId target_id) {
    this->target_id = target_id;
}

auto Bullet::GetCollideTarget() const -> PlayerId {
    return target_id;
}

void Bullet::SetCallback(std::function<void()> callback) {
    this->callback = callback;
}

void Bullet::SetValid(bool valid) {
    this->valid = valid;
}

auto Bullet::GetValid() const -> bool {
    return valid;
}

auto Bullet::CheckCanRemove() const -> bool {
    return can_remove;
}

void Bullet::OnCollide() {
    if(callback) callback();
}

auto Bullet::CheckCollision(const Vector2<int>& position, const Vector2<int>& size) -> bool {
    bool is_collide_x = (std::max(this->position.x + this->size.x - this->collision_offset.x, position.x + size.x)
        - std::min(this->position.x + this->collision_offset.x, position.x)) < (this->size.x + size.x - this->collision_offset.x * 2);
    bool is_collide_y = (std::max(this->position.y + this->size.y - this->collision_offset.y, position.y + size.y)
        - std::min(this->position.y + this->collision_offset.y, position.y)) < (this->size.y + size.y - this->collision_offset.y * 2);

    return is_collide_x && is_collide_y;
}

void Bullet::OnDraw(const Camera& camera) const {
    if(is_debug){
        setfillcolor(RGB(0, 0, 255));
        solidcircle(position.x, position.y, 3);

        setlinecolor(RGB(128, 0, 128));
        rectangle(position.x + collision_offset.x, position.y + collision_offset.y,
            position.x + size.x - collision_offset.x, position.y + size.y - collision_offset.y);
    }
}

auto Bullet::check_if_exceeds_screen() const -> bool {
    return (position.x + size.x < 0 || position.x - size.x > getwidth() || position.y - size.y > getheight());
}