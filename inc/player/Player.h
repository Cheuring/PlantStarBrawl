#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <graphics.h>
#include <vector>
#include <list>

#include "Animation.h"
#include "Bullet.h"
#include "Camera.h"
#include "Timer.h"
#include "Particle.h"
#include "Platform.h"
#include "PlayerId.h"
#include "Vector2.h"

extern bool is_debug;

extern std::vector<Bullet *> bullet_list;
extern std::vector<BuffBullet *> buff_bullet_list;
extern std::vector<Platform> platform_list;

extern Atlas atlas_run_effect;
extern Atlas atlas_jump_effect;
extern Atlas atlas_land_effect;

extern IMAGE img_1P_cursor;
extern IMAGE img_2P_cursor;

class Player {
public:
    Player(int attack_cd = 350) : attack_cd(attack_cd) {
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

        timer_run_effect_generation.set_wait_time(75);
        timer_run_effect_generation.set_callback([&](){
            if(velocity.y != 0){
                return;
            }
            Vector2 particle_position;
            IMAGE* frame = atlas_run_effect.get_image(0);
            particle_position.x = position.x + (size.x - frame->getwidth()) / 2;
            particle_position.y = position.y + size.y - frame->getheight();
            particle_list.emplace_back(particle_position, 45, &atlas_run_effect);
        });

        timer_die_effect_generation.set_wait_time(35);
        timer_die_effect_generation.set_callback([&](){
            Vector2 particle_position;
            IMAGE* frame = atlas_run_effect.get_image(0);
            particle_position.x = position.x + (size.x - frame->getwidth()) / 2;
            particle_position.y = position.y + size.y - frame->getheight();
            particle_list.emplace_back(particle_position, 150, &atlas_run_effect);
        });

        animation_jump_effect.set_atlas(&atlas_jump_effect);
        animation_jump_effect.set_interval(25);
        animation_jump_effect.set_loop(false);
        animation_jump_effect.set_on_finish([&](){
            is_jump_effect_visible = false;
        });

        animation_land_effect.set_atlas(&atlas_land_effect);
        animation_land_effect.set_interval(50);
        animation_land_effect.set_loop(false);
        animation_land_effect.set_on_finish([&](){
            is_land_effect_visible = false;
        });

        timer_cursor_visible.set_wait_time(2500);
        timer_cursor_visible.set_one_shot(true);
        timer_cursor_visible.set_callback([&](){
            is_cursor_visible = false;
        });

        timer_buff_hurry.set_wait_time(4000);
        timer_buff_hurry.set_one_shot(true);
        timer_buff_hurry.set_callback([&](){
            deleteBuff(BuffId::HURRY);
        });

        timer_buff_invisible.set_wait_time(8000);
        timer_buff_invisible.set_one_shot(true);
        timer_buff_invisible.set_callback([&](){
            deleteBuff(BuffId::INVISIBLE);
        });

        timer_buff_recover_hp.set_wait_time(5000);
        timer_buff_recover_hp.set_one_shot(true);
        timer_buff_recover_hp.set_callback([&](){
            deleteBuff(BuffId::RECOVER_HP);
        });

        timer_buff_recover_hp_helper.set_wait_time(1000);
        timer_buff_recover_hp_helper.set_one_shot(false);
        timer_buff_recover_hp_helper.set_callback([&](){
            hp += 5;
            hp = std::min(hp, 100);
        });

        timer_buff_recover_mp.set_wait_time(5000);
        timer_buff_recover_mp.set_one_shot(true);
        timer_buff_recover_mp.set_callback([&](){
            deleteBuff(BuffId::RECOVER_MP);
        });
        timer_buff_recover_mp_helper.pause();

        timer_buff_recover_mp_helper.set_wait_time(1000);
        timer_buff_recover_mp_helper.set_one_shot(false);
        timer_buff_recover_mp_helper.set_callback([&](){
            mp += 5;
            mp = std::min(mp, 100);
        });
        timer_buff_recover_hp_helper.pause();
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
            timer_run_effect_generation.pause();
        }

        if(is_attacking_ex){
            current_animation = is_facing_right ? &animation_attack_ex_right : &animation_attack_ex_left;
        }

        current_animation->on_update(delta);
        animation_jump_effect.on_update(delta);
        animation_land_effect.on_update(delta);

        timer_attack_cd.on_update(delta);
        timer_invulnerable.on_update(delta);
        timer_invulnerable_blink.on_update(delta);
        timer_run_effect_generation.on_update(delta);
        timer_cursor_visible.on_update(delta);

        timer_buff_hurry.on_update(delta);
        timer_buff_invisible.on_update(delta);
        timer_buff_recover_hp.on_update(delta);
        timer_buff_recover_mp.on_update(delta);
        timer_buff_recover_hp_helper.on_update(delta);
        timer_buff_recover_mp_helper.on_update(delta);

        if(hp <= 0){
            current_animation = last_hurt_direction.x < 0 ? &animation_die_left : &animation_die_right;
        }

        if(hp <= 0){
            timer_die_effect_generation.on_update(delta);
        }

        particle_list.erase(std::remove_if(particle_list.begin(), particle_list.end(), [&](Particle& particle){
            return !particle.check_valid();
        }), particle_list.end());

        for(auto& particle : particle_list){
            particle.on_update(delta);
        }

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
        timer_run_effect_generation.resume();
    }

    virtual void on_jump() {
        if(velocity.y != 0 || is_attacking_ex){
            return;
        }

        velocity.y = jump_velocity;
        is_jump_effect_visible = true;
        animation_jump_effect.reset();

        IMAGE* effect_frame = animation_jump_effect.get_current_frame();
        position_jump_effect.x = position.x + (size.x - effect_frame->getwidth()) / 2;
        position_jump_effect.y = position.y + size.y - effect_frame->getheight();
    }

    virtual void on_land() {
        is_land_effect_visible = true;
        animation_land_effect.reset();

        IMAGE* effect_frame = animation_land_effect.get_current_frame();
        position_land_effect.x = position.x + (size.x - effect_frame->getwidth()) / 2;
        position_land_effect.y = position.y + size.y - effect_frame->getheight();
    }

    virtual void on_drop() {
        if(velocity.y == 0){
            if(position.y + size.y == platform_list[0].shape.y){
                return;
            }
            position.y += 1;
            velocity.y = 0.1f;
        }
    }

    virtual void on_draw(const Camera& camera) {
        if(is_jump_effect_visible){
            animation_jump_effect.on_draw(camera, position_jump_effect.x, position_jump_effect.y);
        }

        if(is_land_effect_visible){
            animation_land_effect.on_draw(camera, position_land_effect.x, position_land_effect.y);
        }

        for(const auto& particle : particle_list){
            particle.on_draw(camera);
        }

        if(!is_invisible){
            if(hp > 0 && is_invulnerable && is_showing_sketch_frame){
                put_image_alpha(camera, position.x, position.y, &img_sketch);
            }else{
                current_animation->on_draw(camera, position.x, position.y);
            }
        }

        if(is_cursor_visible){
            switch (id) {
                case PlayerId::P1:
                    put_image_alpha(camera, position.x + (size.x - img_1P_cursor.getwidth()) / 2,
                        position.y - img_1P_cursor.getheight() , &img_1P_cursor);
                    break;
                case PlayerId::P2:
                    put_image_alpha(camera, position.x + (size.x - img_2P_cursor.getwidth()) / 2,
                        position.y - img_2P_cursor.getheight() , &img_2P_cursor);
                    break;
            }
        }

        if(is_debug){
            setfillcolor(RGB(0, 0, 255));
            solidcircle(position.x, position.y, 3);
            
            setlinecolor(RGB(0, 125, 125));
            rectangle(position.x + collision_offset.x, position.y + collision_offset.y, position.x + size.x - collision_offset.x, position.y + size.y);
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
                            case 'S':
                            case 's':
                                on_drop();
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

                                    if(!is_debug)
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
                            case VK_DOWN:
                                on_drop();
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

                                    if(!is_debug)
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

    int get_hp() const {
        return hp;
    }

    void set_hp(int hp) {
        this->hp = hp;
    }

    int get_mp() const {
        return mp;
    }

    void make_invulnerable() {
        is_invulnerable = true;
        timer_invulnerable.restart();
    }

    const std::list<BuffId>& getBuffList(){
        return buff_list;
    }

    virtual void on_attack() = 0;
    virtual void on_attack_ex() = 0;

protected:
    const float gravity = 1.6e-3f;
    const float jump_velocity = -0.85f;
    float run_velocity = 0.55f;

    int mp = 0;     //  Magic Point
    int hp = 100;   //  Health Point
    
    Vector2 position;
    Vector2 velocity;
    Vector2 size;
    Vector2 collision_offset;

    Animation animation_idle_left;
    Animation animation_idle_right;
    Animation animation_run_left;
    Animation animation_run_right;
    Animation animation_attack_ex_left;
    Animation animation_attack_ex_right;
    Animation animation_jump_effect;
    Animation animation_land_effect;
    Animation animation_die_left;
    Animation animation_die_right;

    bool is_jump_effect_visible = false;
    bool is_land_effect_visible = false;

    Vector2 position_jump_effect;
    Vector2 position_land_effect;

    Animation* current_animation = nullptr;

    PlayerId id = PlayerId::P1;

    bool is_left_key_down = false;
    bool is_right_key_down = false;

    bool is_facing_right = true;

    int attack_cd = 350;
    bool can_attack = true;
    Timer timer_attack_cd;

    bool is_attacking_ex = false;

    IMAGE img_sketch;
    bool is_invulnerable = false;
    bool is_showing_sketch_frame = false;
    Timer timer_invulnerable;
    Timer timer_invulnerable_blink;

    std::vector<Particle> particle_list;

    Timer timer_run_effect_generation;
    Timer timer_die_effect_generation;

    bool is_cursor_visible = true;
    Timer timer_cursor_visible;

    Vector2 last_hurt_direction;

    std::list<BuffId> buff_list;
    std::unordered_map<BuffId, decltype(buff_list.begin())> buffid_map;
    Timer timer_buff_recover_hp;
    Timer timer_buff_recover_hp_helper;
    Timer timer_buff_recover_mp;
    Timer timer_buff_recover_mp_helper;
    Timer timer_buff_hurry;
    Timer timer_buff_invisible;

    bool is_invisible = false;

protected:
    void addBuff(BuffId buffid) {
        switch(buffid){
            case BuffId::RECOVER_HP:
                if(buffid_map.find(BuffId::RECOVER_HP) == buffid_map.end()){
                    buff_list.push_back(BuffId::RECOVER_HP);
                    buffid_map[BuffId::RECOVER_HP] = std::prev(buff_list.end());
                    timer_buff_recover_hp.restart();
                    timer_buff_recover_hp_helper.restart();
                    timer_buff_recover_hp_helper.resume();
                }else{
                    buff_list.splice(buff_list.end(), buff_list, buffid_map[BuffId::RECOVER_HP]);
                    timer_buff_recover_hp.restart();
                    timer_buff_recover_hp_helper.restart();
                }
                break;
            case BuffId::RECOVER_MP:
                if(buffid_map.find(BuffId::RECOVER_MP) == buffid_map.end()){
                    buff_list.push_back(BuffId::RECOVER_MP);
                    buffid_map[BuffId::RECOVER_MP] = std::prev(buff_list.end());
                    timer_buff_recover_mp.restart();
                    timer_buff_recover_mp_helper.restart();
                    timer_buff_recover_mp_helper.resume();
                }else{
                    buff_list.splice(buff_list.end(), buff_list, buffid_map[BuffId::RECOVER_MP]);
                    timer_buff_recover_mp.restart();
                    timer_buff_recover_mp_helper.restart();
                }
                break;
            case BuffId::HURRY:
                if(buffid_map.find(BuffId::HURRY) == buffid_map.end()){
                    buff_list.push_back(BuffId::HURRY);
                    buffid_map[BuffId::HURRY] = std::prev(buff_list.end());
                    run_velocity *= 2;
                    timer_buff_hurry.restart();
                }else{
                    buff_list.splice(buff_list.end(), buff_list, buffid_map[BuffId::HURRY]);
                    timer_buff_hurry.restart();
                }
                break;
            case BuffId::INVISIBLE:
                if(buffid_map.find(BuffId::INVISIBLE) == buffid_map.end()){
                    buff_list.push_back(BuffId::INVISIBLE);
                    buffid_map[BuffId::INVISIBLE] = std::prev(buff_list.end());
                    is_invisible = true;
                    timer_buff_invisible.restart();
                }else{
                    buff_list.splice(buff_list.end(), buff_list, buffid_map[BuffId::INVISIBLE]);
                    timer_buff_invisible.restart();
                }
                break;
            default:
                break;
        }
    }

    void deleteBuff(BuffId buffid) {
        switch(buffid) {
            case BuffId::RECOVER_HP:
                if(buffid_map.find(BuffId::RECOVER_HP) != buffid_map.end()){
                    buff_list.erase(buffid_map[BuffId::RECOVER_HP]);
                    buffid_map.erase(BuffId::RECOVER_HP);
                    timer_buff_recover_hp_helper.pause();
                }
                break;
            case BuffId::RECOVER_MP:
                if(buffid_map.find(BuffId::RECOVER_MP) != buffid_map.end()){
                    buff_list.erase(buffid_map[BuffId::RECOVER_MP]);
                    buffid_map.erase(BuffId::RECOVER_MP);
                    timer_buff_recover_mp_helper.pause();
                }
                break;
            case BuffId::HURRY:
                if(buffid_map.find(BuffId::HURRY) != buffid_map.end()){
                    buff_list.erase(buffid_map[BuffId::HURRY]);
                    buffid_map.erase(BuffId::HURRY);
                    run_velocity = 0.55f;
                }
                break;
            case BuffId::INVISIBLE:
                if(buffid_map.find(BuffId::INVISIBLE) != buffid_map.end()){
                    buff_list.erase(buffid_map[BuffId::INVISIBLE]);
                    buffid_map.erase(BuffId::INVISIBLE);
                    is_invisible = false;
                }
                break;
            default:
                break;
        }
    }

    void move_and_collide(int delta) {
        float last_velocity_y = velocity.y;

        velocity.y += gravity * delta;
        position += velocity * (float)delta;

        if(hp <= 0) {
            return;
        }

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

                        if(last_velocity_y != 0){
                            on_land();
                        }
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

                if(bullet->check_collision(position + collision_offset, {size.x - collision_offset.x * 2, size.y - collision_offset.y})){
                    if(!is_debug)
                        hp -= bullet->get_damage();

                    make_invulnerable();
                    last_hurt_direction = bullet->get_position() - position;

                    bullet->on_collide();
                    bullet->set_valid(false);

                    if(hp <= 0) {
                        velocity.x = last_hurt_direction.x < 0 ? 0.35f : -0.35f;
                        velocity.y = -1.f;
                    }
                }
            }
        }

        for(BuffBullet* buff : buff_bullet_list){
            if(!buff->get_valid()){
                continue;
            }

            if(buff->check_collision(position + collision_offset, {size.x - collision_offset.x * 2, size.y - collision_offset.y})){
                addBuff(buff->getBuffId());
                buff->on_collide();
                buff->set_valid(false);
            }
        }
    }
};


#endif // _PLAYER_H_