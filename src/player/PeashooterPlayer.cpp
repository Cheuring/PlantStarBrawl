#include "PeashooterPlayer.h"

PeashooterPlayer::PeashooterPlayer() : Player() {
    animation_idle_left.SetAtlas(&atlas_peashooter_idle_left);
    animation_idle_right.SetAtlas(&atlas_peashooter_idle_right);
    animation_run_left.SetAtlas(&atlas_peashooter_run_left);
    animation_run_right.SetAtlas(&atlas_peashooter_run_right);
    animation_attack_ex_left.SetAtlas(&atlas_peashooter_attack_ex_left);
    animation_attack_ex_right.SetAtlas(&atlas_peashooter_attack_ex_right);
    animation_die_left.SetAtlas(&atlas_peashooter_die_left);
    animation_die_right.SetAtlas(&atlas_peashooter_die_right);

    animation_idle_left.SetInterval(75);
    animation_idle_right.SetInterval(75);
    animation_run_left.SetInterval(75);
    animation_run_right.SetInterval(75);
    animation_attack_ex_left.SetInterval(75);
    animation_attack_ex_right.SetInterval(75);
    animation_die_left.SetInterval(150);
    animation_die_right.SetInterval(150);

    animation_die_left.SetLoop(false);
    animation_die_right.SetLoop(false);

    size.x = 96, size.y = 96;
    collision_offset.x = 10;
    collision_offset.y = 15;

    timer_attck_ex.SetWaitTime(attack_ex_duration);
    timer_attck_ex.SetOneShot(true);
    timer_attck_ex.SetCallback([&](){
        is_attacking_ex = false;
    });

    timer_spwan_pea_ex.SetWaitTime(100);
    timer_spwan_pea_ex.SetCallback([&](){
        SpwanPeaBullet(speed_pea_ex);
    });
}

void PeashooterPlayer::OnUpdate(int delta) {
    Player::OnUpdate(delta);

    if(is_attacking_ex){
        main_camera.Shake(5, 100);
        timer_attck_ex.OnUpdate(delta);
        timer_spwan_pea_ex.OnUpdate(delta);
    }
}

void PeashooterPlayer::OnAttack() {
    SpwanPeaBullet(speed_pea);

    switch (rand() % 2) {
        case 0:
            mciSendString(_T("play pea_shoot_1 from 0"), NULL, 0, NULL);
            break;
        case 1:
            mciSendString(_T("play pea_shoot_2 from 0"), NULL, 0, NULL);
            break;
    }
}

void PeashooterPlayer::OnAttackEx() {
    is_attacking_ex = true;
    timer_attck_ex.Restart();

    is_facing_right ? animation_attack_ex_right.Reset() : animation_attack_ex_left.Reset();

    mciSendString(_T("play pea_shoot_ex from 0"), NULL, 0, NULL);
}

void PeashooterPlayer::SpwanPeaBullet(float speed) {
    Bullet* bullet = new PeaBullet();

    Vector2 bullet_position, bullet_velocity;
    const Vector2& bullet_size = bullet->GetSize();
    bullet_position.x = is_facing_right
        ? position.x + size.x - bullet_size.x / 2
        : position.x - bullet_size.x / 2;
    bullet_position.y = position.y;
    bullet_velocity.x = is_facing_right ? speed : -speed;
    bullet_velocity.y = 0;

    bullet->SetPosition(bullet_position);
    bullet->SetVelocity(bullet_velocity);

    bullet->SetCollideTarget(id == PlayerId::P1 ? PlayerId::P2 : PlayerId::P1);

    bullet->SetCallback([&](){
        mp += 15;
    });

    bullet_list.push_back(bullet);
}