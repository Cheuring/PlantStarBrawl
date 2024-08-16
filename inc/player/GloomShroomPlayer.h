#ifndef _GLOOM_SHROOM_PLAYER_H_
#define _GLOOM_SHROOM_PLAYER_H_

#include "Atlas.h"
#include "Camera.h"
#include "BubblesBullet.h"
#include "Player.h"
#include "Timer.h"

extern Atlas atlas_gloomshroom_idle_left;
extern Atlas atlas_gloomshroom_idle_right;
extern Atlas atlas_gloomshroom_run_left;
extern Atlas atlas_gloomshroom_run_right;
extern Atlas atlas_gloomshroom_attack_ex_left;
extern Atlas atlas_gloomshroom_attack_ex_right;
extern Atlas atlas_gloomshroom_die_left;
extern Atlas atlas_gloomshroom_die_right;

extern Camera main_camera;

class GloomShroomPlayer : public Player {
public:
    GloomShroomPlayer() : Player(700) {
        animation_idle_left.set_atlas(&atlas_gloomshroom_idle_left);
        animation_idle_right.set_atlas(&atlas_gloomshroom_idle_right);
        animation_run_left.set_atlas(&atlas_gloomshroom_run_left);
        animation_run_right.set_atlas(&atlas_gloomshroom_run_right);
        animation_attack_ex_left.set_atlas(&atlas_gloomshroom_attack_ex_left);
        animation_attack_ex_right.set_atlas(&atlas_gloomshroom_attack_ex_right);
        animation_die_left.set_atlas(&atlas_gloomshroom_die_left);
        animation_die_right.set_atlas(&atlas_gloomshroom_die_right);

        animation_idle_left.set_interval(75);
        animation_idle_right.set_interval(75);
        animation_run_left.set_interval(75);
        animation_run_right.set_interval(75);
        animation_attack_ex_left.set_interval(75);
        animation_attack_ex_right.set_interval(75);
        animation_die_left.set_interval(150);
        animation_die_right.set_interval(150);

        animation_die_left.set_loop(false);
        animation_die_right.set_loop(false);

        animation_attack_ex_left.set_loop(false);
        animation_attack_ex_right.set_loop(false);

        size.x = 96, size.y = 96;
        collision_offset.y = 15;

        timer_attack_ex.set_wait_time(ex_attack_interval);
        timer_attack_ex.set_one_shot(true);
        timer_attack_ex.set_callback([&](){
            if(current_times < ex_attack_times){
                current_times++;
                on_attack_ex();
            }else{
                is_attacking_ex = false;
                current_times = 1;
            }
        });
    }
    ~GloomShroomPlayer() = default;

    void on_update(int delta) override {
        Player::on_update(delta);

        if(is_attacking_ex){
            main_camera.shake(5, 100);
            timer_attack_ex.on_update(delta);
        }
    }

    void on_attack() override {
        mciSendString(_T("play bubbles_shot from 0"), NULL, 0, NULL);

        Bullet* bullet = new BubblesBullet();

        bullet->set_position(position + size * 0.5f);
        bullet->set_collide_target(id == PlayerId::P1 ? PlayerId::P2 : PlayerId::P1);
        bullet->set_callback([&](){
            mp += 25;
        });

        bullet_list.push_back(bullet);
    }

    void on_attack_ex() override {
        if(!is_attacking_ex){
            mciSendString(_T("play bubbles_shot_ex from 0"), NULL, 0, NULL);
            is_attacking_ex = true;
        }

        timer_attack_ex.restart();
        is_facing_right ? animation_attack_ex_right.reset() : animation_attack_ex_left.reset();

        Bullet* bullet = new BubblesBullet(true);
        bullet->set_position(position + size * 0.5f);
        bullet->set_collide_target(id == PlayerId::P1 ? PlayerId::P2 : PlayerId::P1);
        bullet->set_callback([&](){
            mp += 15;
        });

        bullet_list.push_back(bullet);
    }

private:
    const int ex_attack_times = 5;
    const int ex_attack_interval = 75 * 7;

    Timer timer_attack_ex;
    int current_times = 1;

};




#endif // _GLOOM_SHROOM_PLAYER_H_