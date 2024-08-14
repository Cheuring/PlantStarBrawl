#ifndef _BUlLET_H_
#define _BUlLET_H_

#include <functional>
#include <graphics.h>

#include "Camera.h"
#include "PlayerId.h"
#include "Vector2.h"

extern bool is_debug;

class Bullet {
public:
    Bullet() = default;
    ~Bullet() = default;

    void set_damage(int damage) {
        this->damage = damage;
    }

    int get_damage() const {
        return damage;
    }

    void set_position(float x, float y) {
        position.x = x;
        position.y = y;
    }
    void set_position(const Vector2& position) {
        this->position = position;
    }

    const Vector2& get_position() const {
        return position;
    }

    const Vector2& get_size() const {
        return size;
    }

    void set_velocity(float x, float y) {
        velocity.x = x;
        velocity.y = y;
    }
    void set_velocity(const Vector2& velocity) {
        this->velocity = velocity;
    }

    void set_collide_target(PlayerId target_id) {
        this->target_id = target_id;
    }

    PlayerId get_collide_target() const {
        return target_id;
    }

    void set_callback(std::function<void()> callback) {
        this->callback = callback;
    }

    void set_valid(bool valid) {
        this->valid = valid;
    }

    bool get_valid() const {
        return valid;
    }

    bool check_can_remove() const {
        return can_remove;
    }

    virtual void on_collide(){
        if(callback) callback();
    }

    virtual bool check_collision(const Vector2& position, const Vector2& size) {
        return this->position.x + this->size.x / 2 >= position.x
            && this->position.x + this->size.x / 2 <= position.x + size.x
            && this->position.y + this->size.y / 2 >= position.y
            && this->position.y + this->size.y / 2 <= position.y + size.y;
    }


    virtual void on_update(int delta) = 0;
    virtual void on_draw(const Camera& camera) const {
        if(is_debug){
            setfillcolor(RGB(255, 255, 255));
            setlinecolor(RGB(255, 255, 255));
            rectangle(position.x, position.y, position.x + size.x, position.y + size.y);
            solidcircle(position.x + size.x / 2, position.y + size.y / 2, 5);
        }
    }


protected:
    Vector2 size;
    Vector2 position;
    Vector2 velocity;
    int damage = 10;
    bool valid = true;
    bool can_remove = false;
    std::function<void()> callback;
    PlayerId target_id = PlayerId::P1;

protected:
    bool check_if_exceeds_screen() {
        return (position.x + size.x <= 0 || position.x - size.x >= getwidth()
            || position.y + size.y <= 0 || position.y - size.y >= getheight());
    }
};

#endif // _BUlLET_H_