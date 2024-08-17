#ifndef _NUT_PLAYER_H_
#define _NUT_PLAYER_H_

#include "Atlas.h"
#include "Camera.h"
#include "Player.h"
#include "NutBullet.h"

extern Atlas atlas_nut_idle_left;
extern Atlas atlas_nut_idle_right;
extern Atlas atlas_nut_run_left;
extern Atlas atlas_nut_run_right;
extern Atlas atlas_nut_attack_ex_left;
extern Atlas atlas_nut_attack_ex_right;
extern Atlas atlas_nut_die_left;
extern Atlas atlas_nut_die_right;

extern Camera main_camera;

class NutPlayer : public Player {
public:
    NutPlayer() : Player(400) {
        animation_idle_left.set_atlas(&atlas_nut_idle_left);
        animation_idle_right.set_atlas(&atlas_nut_idle_right);
        animation_run_left.set_atlas(&atlas_nut_run_left);
        animation_run_right.set_atlas(&atlas_nut_run_right);
        animation_attack_ex_left.set_atlas(&atlas_nut_attack_ex_left);
        animation_attack_ex_right.set_atlas(&atlas_nut_attack_ex_right);
        animation_die_left.set_atlas(&atlas_nut_die_left);
        animation_die_right.set_atlas(&atlas_nut_die_right);

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

        animation_attack_ex_left.set_on_finish([&](){
            hp -= 20;
            make_invulnerable();
            is_attacking_ex = false;
            nut_explode();
        });
        animation_attack_ex_right.set_on_finish([&](){
            hp -= 20;
            make_invulnerable();
            is_attacking_ex = false;
            nut_explode();
        });

        timer_recovery.set_wait_time(500);
        timer_recovery.set_callback([&](){
            if(hp < 0) return;

            if(++hp > 100) hp = 100;
        });

        size.x = 96, size.y = 96;
        collision_offset.x = 10;
        collision_offset.y = 8;
        mp = 50;
    }
    ~NutPlayer() = default;

    void on_update(int delta) override {
        int last_hp = hp;

        Player::on_update(delta);

        if(hp < last_hp) {
            mp += (last_hp - hp) * 2;
            mp = std::min(mp, 100);
        }

        timer_recovery.on_update(delta);
    }

    void on_attack() override {
        is_facing_right ? position.x += dash_distance : position.x -= dash_distance;

        mciSendString(_T("play nut_dash from 0"), NULL, 0, NULL);
    }
    void on_attack_ex() override {
        is_attacking_ex = true;
        is_invulnerable = true;

        is_facing_right ? animation_attack_ex_right.reset() : animation_attack_ex_left.reset();
        
        mciSendString(_T("play nut_explode from 0"), NULL, 0, NULL);
    }

    void nut_explode() {
        Bullet* bullet = new NutBullet();
        bullet->set_position(position + (size - bullet->get_size()) * 0.5f);
        bullet->set_collide_target(id == PlayerId::P1 ? PlayerId::P2 : PlayerId::P1);
        bullet->set_callback([&](){
            mp += 50;
        });
        
        bullet_list.push_back(bullet);

        main_camera.shake(35, 350);
    }

private:
    const int dash_distance = 150;

    Timer timer_recovery;
};


#endif // _NUT_PLAYER_H_