#ifndef _PEASHOOTER_PLAYER_H_
#define _PEASHOOTER_PLAYER_H_

#include "Atlas.h"
#include "Camera.h"
#include "PeaBullet.h"
#include "Player.h"
#include "Timer.h"

extern Atlas atlas_peashooter_idle_left;
extern Atlas atlas_peashooter_idle_right;
extern Atlas atlas_peashooter_run_left;
extern Atlas atlas_peashooter_run_right;
extern Atlas atlas_peashooter_attack_ex_left;
extern Atlas atlas_peashooter_attack_ex_right;
extern Atlas atlas_peashooter_die_left;
extern Atlas atlas_peashooter_die_right;

extern Camera main_camera;


class PeashooterPlayer : public Player {
public:
    PeashooterPlayer() : Player() {
        animation_idle_left.set_atlas(&atlas_peashooter_idle_left);
        animation_idle_right.set_atlas(&atlas_peashooter_idle_right);
        animation_run_left.set_atlas(&atlas_peashooter_run_left);
        animation_run_right.set_atlas(&atlas_peashooter_run_right);
        animation_attack_ex_left.set_atlas(&atlas_peashooter_attack_ex_left);
        animation_attack_ex_right.set_atlas(&atlas_peashooter_attack_ex_right);
        animation_die_left.set_atlas(&atlas_peashooter_die_left);
        animation_die_right.set_atlas(&atlas_peashooter_die_right);

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

        size.x = 96, size.y = 96;
        collision_offset.x = 10;
        collision_offset.y = 15;

        timer_attck_ex.set_wait_time(attack_ex_duration);
        timer_attck_ex.set_one_shot(true);
        timer_attck_ex.set_callback([&](){
            is_attacking_ex = false;
        });

        timer_spwan_pea_ex.set_wait_time(100);
        timer_spwan_pea_ex.set_callback([&](){
            spwan_pea_bullet(speed_pea_ex);
        });
    }
    ~PeashooterPlayer() = default;

    void on_update(int delta) override {
        Player::on_update(delta);

        if(is_attacking_ex){
            main_camera.shake(5, 100);
            timer_attck_ex.on_update(delta);
            timer_spwan_pea_ex.on_update(delta);
        }
    }

    void on_attack() override {
        spwan_pea_bullet(speed_pea);

        switch (rand() % 2) {
            case 0:
                mciSendString(_T("play pea_shoot_1 from 0"), NULL, 0, NULL);
                break;
            case 1:
                mciSendString(_T("play pea_shoot_2 from 0"), NULL, 0, NULL);
                break;
        }
    }

    void on_attack_ex() override {
        is_attacking_ex = true;
        timer_attck_ex.restart();

        is_facing_right ? animation_attack_ex_right.reset() : animation_attack_ex_left.reset();

        mciSendString(_T("play pea_shoot_ex from 0"), NULL, 0, NULL);
    }

private:
    const float speed_pea = 0.75f;
    const float speed_pea_ex = 1.5f;
    const int attack_ex_duration = 2500;

    Timer timer_attck_ex;
    Timer timer_spwan_pea_ex;
    

private:
    void spwan_pea_bullet(float speed) {
        Bullet* bullet = new PeaBullet();

        Vector2 bullet_position, bullet_velocity;
        const Vector2& bullet_size = bullet->get_size();
        bullet_position.x = is_facing_right
            ? position.x + size.x - bullet_size.x / 2
            : position.x - bullet_size.x / 2;
        bullet_position.y = position.y;
        bullet_velocity.x = is_facing_right ? speed : -speed;
        bullet_velocity.y = 0;

        bullet->set_position(bullet_position);
        bullet->set_velocity(bullet_velocity);

        bullet->set_collide_target(id == PlayerId::P1 ? PlayerId::P2 : PlayerId::P1);

        bullet->set_callback([&](){
            mp += 15;
        });

        bullet_list.push_back(bullet);
    }
    
};

#endif // _PEASHOOTER_PLAYER_H_