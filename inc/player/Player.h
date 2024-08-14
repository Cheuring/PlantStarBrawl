#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <graphics.h>
#include <vector>

#include "Animation.h"
#include "Bullet.h"
#include "Camera.h"
#include "Timer.h"
#include "Platform.h"
#include "PlayerId.h"
#include "Vector2.h"

extern bool is_debug;

extern std::vector<Bullet *> bullet_list;
extern std::vector<Platform> platform_list;

class Player {
public:
    Player() {
        current_animation = &animation_idle_right;

        timer_attack_cd.set_wait_time(attack_cd);
        timer_attack_cd.set_one_shot(true);
        timer_attack_cd.set_callback([&](){
            can_attack = true;
        });

        timer_invulnerable.set_wait_time(750);
        timer_invulnerable.set_one_shot(true);
        timer_invulnerable.set_callback([&](){
            is_invulnerable = false;
        });

        timer_invulnerable_blink.set_wait_time(75);
        timer_invulnerable_blink.set_callback([&](){
            is_showing_sketch_frame = !is_showing_sketch_frame;
        });
    }
    ~Player() = default;

    virtual void on_update(int delta) {
        int direction = is_right_key_down - is_left_key_down;

        if(direction != 0){
            if(!is_attacking_ex)
                is_facing_right = direction > 0;

            current_animation = is_facing_right ? &animation_run_right : &animation_run_left;
            float distance = run_velocity * delta * direction;
            on_run(distance);
        }else {
            current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;
        }

        if(is_attacking_ex){
            current_animation = is_facing_right ? &animation_attack_ex_right : &animation_attack_ex_left;
        }

        current_animation->on_update(delta);

        timer_attack_cd.on_update(delta);
        timer_invulnerable.on_update(delta);
        timer_invulnerable_blink.on_update(delta);

        if(is_showing_sketch_frame){
            sketch_image(current_animation->get_current_frame(), &img_sketch);
        }

        move_and_collide(delta);
    }

    virtual void on_run(float distance) {
        if(is_attacking_ex){
            return;
        }

        position.x += distance;
    }

    virtual void on_jump() {
        if(velocity.y != 0 || is_attacking_ex){
            return;
        }

        velocity.y = jump_velocity;
    }

    virtual void on_draw(const Camera& camera) {
        if(hp > 0 && is_invulnerable && is_showing_sketch_frame){
            put_image_alpha(camera, position.x, position.y, &img_sketch);
        }else{
            current_animation->on_draw(camera, position.x, position.y);
        }

        if(is_debug){
            setlinecolor(RGB(0, 125, 125));
            rectangle(position.x, position.y, position.x + size.x, position.y + size.y);
        }
    }

    virtual void on_input(const ExMessage& msg) {
        switch (msg.message){
            case WM_KEYDOWN:
                switch (id) {
                    case PlayerId::P1:
                        switch (msg.vkcode) {
                            case 'A':
                            case 'a':
                                is_left_key_down = true;
                                break;
                            case 'D':
                            case 'd':
                                is_right_key_down = true;
                                break;
                            case 'W':
                            case 'w':
                                on_jump();
                                break;
                            case 'F':
                            case 'f':
                                if(can_attack){
                                    on_attack();
                                    can_attack = false;
                                    timer_attack_cd.restart();
                                }
                                break;
                            case 'G':
                            case 'g':
                                if(mp >= 100){
                                    on_attack_ex();
                                    mp = 0;
                                }
                                break;
                        }
                        break;
                    case PlayerId::P2:
                        switch (msg.vkcode) {
                            case VK_LEFT:
                                is_left_key_down = true;
                                break;
                            case VK_RIGHT:
                                is_right_key_down = true;
                                break;
                            case VK_UP:
                                on_jump();
                                break;
                                //  . key
                            case VK_OEM_PERIOD:
                                if(can_attack){
                                    on_attack();
                                    can_attack = false;
                                    timer_attack_cd.restart();
                                }
                                break;
                                //  / key
                            case VK_OEM_2:
                                if(mp >= 100){
                                    on_attack_ex();
                                    mp = 0;
                                }
                                break;
                        }
                        break;
                }
                break;
            case WM_KEYUP:
                switch (id) {
                    case PlayerId::P1:
                        switch (msg.vkcode) {
                            case 'A':
                            case 'a':
                                is_left_key_down = false;
                                break;
                            case 'D':
                            case 'd':
                                is_right_key_down = false;
                                break;
                        }
                        break;
                    case PlayerId::P2:
                        switch (msg.vkcode) {
                            case VK_LEFT:
                                is_left_key_down = false;
                                break;
                            case VK_RIGHT:
                                is_right_key_down = false;
                                break;
                        }
                        break;
                }
                break;
            default:
                break;
        }
    }

    void set_id(PlayerId ID){
        if(ID == PlayerId::P2) {
            is_facing_right = false;
        }
        this->id = ID;
    }

    void set_position(float x, float y){
        position.x = x;
        position.y = y;
    }

    const Vector2& get_position() const {
        return position;
    }

    const Vector2& get_size() const {
        return size;
    }

    PlayerId get_id() const {
        return id;
    }

    void make_invulnerable() {
        is_invulnerable = true;
        timer_invulnerable.restart();
    }

    virtual void on_attack() = 0;
    virtual void on_attack_ex() = 0;

protected:
    const float gravity = 1.6e-3f;
    const float run_velocity = 0.55f;
    const float jump_velocity = -0.85f;

    int mp = 0;     //  Magic Point
    int hp = 100;   //  Health Point
    
    Vector2 position;
    Vector2 velocity;
    Vector2 size;

    Animation animation_idle_left;
    Animation animation_idle_right;
    Animation animation_run_left;
    Animation animation_run_right;
    Animation animation_attack_ex_left;
    Animation animation_attack_ex_right;

    Animation* current_animation = nullptr;

    PlayerId id = PlayerId::P1;

    bool is_left_key_down = false;
    bool is_right_key_down = false;

    bool is_facing_right = true;

    int attack_cd = 500;
    bool can_attack = true;
    Timer timer_attack_cd;

    bool is_attacking_ex = false;

    IMAGE img_sketch;
    bool is_invulnerable = false;
    bool is_showing_sketch_frame = false;
    Timer timer_invulnerable;
    Timer timer_invulnerable_blink;

protected:
    void move_and_collide(int delta) {
        velocity.y += gravity * delta;
        position.y += velocity.y * delta;

        if(velocity.y > 0){
            for(const auto& platform : platform_list){
                const auto& shape = platform.shape;
                bool is_collide_x = (std::max(position.x + size.x, shape.right) - std::min(position.x, shape.left)
                                    <= size.x + (shape.right - shape.left));
                bool is_collide_y = (shape.y >= position.y && shape.y <= position.y + size.y);

                if(is_collide_x && is_collide_y){
                    float delta_pos_y = velocity.y * delta;
                    float last_tick_foot_pos_y = position.y + size.y - delta_pos_y;
                    if(last_tick_foot_pos_y <= shape.y){
                        position.y = shape.y - size.y;
                        velocity.y = 0;

                        break;
                    }
                }
            }

        }

        if(!is_invulnerable){
            for(Bullet* bullet : bullet_list){
                if(bullet->get_collide_target() != id || !bullet->get_valid()){
                    continue;
                }

                if(bullet->check_collision(position, size)){
                    make_invulnerable();
                    bullet->on_collide();
                    bullet->set_valid(false);
                    hp -= bullet->get_damage();
                }
            }
        }
    }
};


#endif // _PLAYER_H_