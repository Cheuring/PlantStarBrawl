#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <graphics.h>

#include "Animation.h"
#include "Camera.h"
#include "PlayerId.h"
#include "Vector2.h"

class Player {
public:
    Player() = default;
    ~Player() = default;

    virtual void on_update(int delta) {
        int direction = is_right_key_down - is_left_key_down;

        if(direction != 0){
            is_facing_right = direction > 0;
            current_animation = is_facing_right ? &animation_run_right : &animation_run_left;
        }else {
            current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;
        }

        current_animation->on_update(delta);
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
    Vector2 position;

    Animation animation_idle_left;
    Animation animation_idle_right;
    Animation animation_run_left;
    Animation animation_run_right;

    Animation* current_animation = nullptr;

    PlayerId id = PlayerId::P1;

    bool is_left_key_down = false;
    bool is_right_key_down = false;

    bool is_facing_right = true;
};


#endif // _PLAYER_H_