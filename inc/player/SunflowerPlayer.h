#ifndef _SUNFLOWER_PLAYER_H_
#define _SUNFLOWER_PLAYER_H_

#include "Atlas.h"
#include "Player.h"
#include "SunBullet.h"
#include "SunBulletEx.h"
#include "Vector2.h"

extern Atlas atlas_sunflower_idle_left;
extern Atlas atlas_sunflower_idle_right;
extern Atlas atlas_sunflower_run_left;
extern Atlas atlas_sunflower_run_right;
extern Atlas atlas_sunflower_attack_ex_left;
extern Atlas atlas_sunflower_attack_ex_right;
extern Atlas atlas_sun_text;
extern Atlas atlas_sunflower_die_left;
extern Atlas atlas_sunflower_die_right;

extern Player* player_1;
extern Player* player_2;

class SunflowerPlayer : public Player {
public:
    SunflowerPlayer() : Player(250) {
        animation_idle_left.set_atlas(&atlas_sunflower_idle_left);
        animation_idle_right.set_atlas(&atlas_sunflower_idle_right);
        animation_run_left.set_atlas(&atlas_sunflower_run_left);
        animation_run_right.set_atlas(&atlas_sunflower_run_right);
        animation_attack_ex_left.set_atlas(&atlas_sunflower_attack_ex_left);
        animation_attack_ex_right.set_atlas(&atlas_sunflower_attack_ex_right);
        animation_sun_text.set_atlas(&atlas_sun_text);
        animation_die_left.set_atlas(&atlas_sunflower_die_left);
        animation_die_right.set_atlas(&atlas_sunflower_die_right);

        animation_idle_left.set_interval(75);
        animation_idle_right.set_interval(75);
        animation_run_left.set_interval(75);
        animation_run_right.set_interval(75);
        animation_attack_ex_left.set_interval(100);
        animation_attack_ex_right.set_interval(100);
        animation_sun_text.set_interval(100);
        animation_die_left.set_interval(150);
        animation_die_right.set_interval(150);

        animation_die_left.set_loop(false);
        animation_die_right.set_loop(false);

        animation_attack_ex_left.set_loop(false);
        animation_attack_ex_right.set_loop(false);
        animation_sun_text.set_loop(false);

        animation_attack_ex_left.set_on_finish([&](){
            is_attacking_ex = false;
            is_sun_text_visible = false;
        });
        animation_attack_ex_right.set_on_finish([&](){
            is_attacking_ex = false;
            is_sun_text_visible = false;
        });

        size.x = 96, size.y = 96;
        collision_offset.x = 10;
        collision_offset.y = 10;
    }
    ~SunflowerPlayer() = default;

    void on_update(int delta) override {
        Player::on_update(delta);

        if(is_sun_text_visible){
            animation_sun_text.on_update(delta);
        }
    }

    void on_draw(const Camera& camera) override {
        Player::on_draw(camera);

        if(is_sun_text_visible){
            Vector2 text_position;
            IMAGE* frame = animation_sun_text.get_current_frame();
            text_position.x = position.x - (size.x - frame->getwidth()) / 2;
            text_position.y = position.y - frame->getheight();
            animation_sun_text.on_draw(camera, text_position.x, text_position.y);
        }
    }
    
    void on_attack() override {
        Bullet* bullet = new SunBullet();

        Vector2 bullet_position;
        const Vector2& bullet_size = bullet->get_size();
        bullet_position.x = position.x + (size.x - bullet_size.x) / 2;
        bullet_position.y = position.y - bullet_size.y;

        bullet->set_position(bullet_position);
        bullet->set_velocity(is_facing_right ? velocity_sun.x : -velocity_sun.x, velocity_sun.y);

        bullet->set_collide_target(id == PlayerId::P1 ? PlayerId::P2 : PlayerId::P1);

        bullet->set_callback([&](){
            mp += 25;
        });

        bullet_list.push_back(bullet);
    }

    void on_attack_ex() override {
        is_attacking_ex = true;
        is_sun_text_visible = true;

        animation_sun_text.reset();
        is_facing_right ? animation_attack_ex_right.reset() : animation_attack_ex_left.reset();

        Bullet* bullet = new SunBulletEx();
        Player* target_player = id == PlayerId::P1 ? player_2 : player_1;

        Vector2 bullet_position, bullet_velocity;
        auto& bullet_size = bullet->get_size();
        auto& target_size = target_player->get_size();
        auto& target_position = target_player->get_position();
        bullet_position.x = target_position.x + (target_size.x - bullet_size.x) / 2;
        bullet_position.y = -bullet_size.y * 0.75;
        bullet_velocity.x = 0;
        bullet_velocity.y = speed_sun_ex;

        bullet->set_position(bullet_position);
        bullet->set_velocity(bullet_velocity);

        bullet->set_collide_target(target_player->get_id());

        bullet->set_callback([&](){
            mp += 50;
        });

        bullet_list.push_back(bullet);

        mciSendString(_T("play sun_text from 0"), NULL, 0, NULL);
    }


private:
    const float speed_sun_ex = 0.30f;
    const Vector2 velocity_sun = {0.25f, -0.5f};

    Animation animation_sun_text;
    bool is_sun_text_visible = false;

};

#endif // _SUNFLOWER_PLAYER_H_