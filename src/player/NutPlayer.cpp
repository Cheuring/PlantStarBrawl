#include "NutPlayer.h"

NutPlayer::NutPlayer() : Player(400) {
    animation_idle_left.SetAtlas(&atlas_nut_idle_left);
    animation_idle_right.SetAtlas(&atlas_nut_idle_right);
    animation_run_left.SetAtlas(&atlas_nut_run_left);
    animation_run_right.SetAtlas(&atlas_nut_run_right);
    animation_attack_ex_left.SetAtlas(&atlas_nut_attack_ex_left);
    animation_attack_ex_right.SetAtlas(&atlas_nut_attack_ex_right);
    animation_die_left.SetAtlas(&atlas_nut_die_left);
    animation_die_right.SetAtlas(&atlas_nut_die_right);

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

    animation_attack_ex_left.SetLoop(false);
    animation_attack_ex_right.SetLoop(false);

    animation_attack_ex_left.SetOnFinish([&](){
        hp -= 20;
        MakeInvulnerable();
        is_attacking_ex = false;
        NutExplode();
    });
    animation_attack_ex_right.SetOnFinish([&](){
        hp -= 20;
        MakeInvulnerable();
        is_attacking_ex = false;
        NutExplode();
    });

    timer_recovery.SetWaitTime(500);
    timer_recovery.SetCallback([&](){
        if(hp < 0) return;

        if(++hp > 100) hp = 100;
    });

    size.x = 96, size.y = 96;
    collision_offset.x = 10;
    collision_offset.y = 8;
    mp = 50;
}

void NutPlayer::OnUpdate(int delta) {
    int last_hp = hp;

    Player::OnUpdate(delta);

    if(hp < last_hp) {
        mp += (last_hp - hp) * 2;
        mp = std::min(mp, 100);
    }

    timer_recovery.OnUpdate(delta);
}

void NutPlayer::OnAttack() {
    is_facing_right ? position.x += dash_distance : position.x -= dash_distance;

    mciSendString(_T("play nut_dash from 0"), NULL, 0, NULL);
}

void NutPlayer::OnAttackEx() {
    is_attacking_ex = true;
    is_invulnerable = true;

    is_facing_right ? animation_attack_ex_right.Reset() : animation_attack_ex_left.Reset();
    
    mciSendString(_T("play nut_explode from 0"), NULL, 0, NULL);
}

void NutPlayer::NutExplode() {
    Bullet* bullet = new NutBullet();
    bullet->SetPosition(position + (size - bullet->GetSize()) * 0.5f);
    bullet->SetCollideTarget(id == PlayerId::P1 ? PlayerId::P2 : PlayerId::P1);
    bullet->SetCallback([&](){
        mp += 50;
    });
    
    bullet_list.push_back(bullet);

    main_camera.Shake(35, 350);
}