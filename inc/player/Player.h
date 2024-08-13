#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <graphics.h>
#include <vector>

#include "Animation.h"
#include "Camera.h"
#include "Platform.h"
#include "PlayerId.h"
#include "Vector2.h"

extern std::vector<Platform> platform_list;

class Player {
public:
    Player() = default;
    ~Player() = default;

    virtual void on_update(int delta) {
        int direction = is_right_key_down - is_left_key_down;

        if(direction != 0){
            is_facing_right = direction > 0;
            current_animation = is_facing_right ? &animation_run_right : &animation_run_left;
            float distance = run_velocity * delta * direction;
            on_run(distance);
        }else {
            current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;
        }

        current_animation->on_update(delta);

        move_and_collide(delta);
    }

    virtual void on_run(float distance) {
        position.x += distance;
    }

    virtual void on_jump() {
        if(velocity.y == 0){
            velocity.y = jump_velocity;
        }else {
            return;
        }
    }

    virtual void on_draw(const Camera& camera) {
        current_animation->on_draw(camera, position.x, position.y);
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

protected:
    const float gravity = 1.6e-3f;
    const float run_velocity = 0.55f;
    const float jump_velocity = -0.85f;
    
    Vector2 position;
    Vector2 velocity;
    Vector2 size;

    Animation animation_idle_left;
    Animation animation_idle_right;
    Animation animation_run_left;
    Animation animation_run_right;

    Animation* current_animation = nullptr;

    PlayerId id = PlayerId::P1;

    bool is_left_key_down = false;
    bool is_right_key_down = false;

    bool is_facing_right = true;

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
    }
};


#endif // _PLAYER_H_