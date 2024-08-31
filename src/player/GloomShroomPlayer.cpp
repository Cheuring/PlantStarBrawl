#include "GloomShroomPlayer.h"

GloomShroomPlayer::GloomShroomPlayer() : Player(700) {
    animation_idle_left.SetAtlas(&atlas_gloomshroom_idle_left);
    animation_idle_right.SetAtlas(&atlas_gloomshroom_idle_right);
    animation_run_left.SetAtlas(&atlas_gloomshroom_run_left);
    animation_run_right.SetAtlas(&atlas_gloomshroom_run_right);
    animation_attack_ex_left.SetAtlas(&atlas_gloomshroom_attack_ex_left);
    animation_attack_ex_right.SetAtlas(&atlas_gloomshroom_attack_ex_right);
    animation_die_left.SetAtlas(&atlas_gloomshroom_die_left);
    animation_die_right.SetAtlas(&atlas_gloomshroom_die_right);

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

    size.x = 96, size.y = 96;
    collision_offset.y = 15;

    timer_attack_ex.SetWaitTime(ex_attack_interval);
    timer_attack_ex.SetOneShot(true);
    timer_attack_ex.SetCallback([&](){
        if(current_times < ex_attack_times){
            current_times++;
            OnAttackEx();
        }else{
            is_attacking_ex = false;
            current_times = 1;
        }
    });
}

void GloomShroomPlayer::OnUpdate(int delta) {
    Player::OnUpdate(delta);

    if(is_attacking_ex){
        main_camera.Shake(5, 100);
        timer_attack_ex.OnUpdate(delta);
    }
}

void GloomShroomPlayer::OnAttack() {
    mciSendString(_T("play bubbles_shot from 0"), NULL, 0, NULL);

    Bullet* bullet = new BubblesBullet();

    bullet->SetPosition(position + size / 2);
    bullet->SetCollideTarget(id == PlayerId::P1 ? PlayerId::P2 : PlayerId::P1);
    bullet->SetCallback([&](){
        mp += 25;
    });

    bullet_list.push_back(bullet);
}

void GloomShroomPlayer::OnAttackEx() {
    if(!is_attacking_ex){
        mciSendString(_T("play bubbles_shot_ex from 0"), NULL, 0, NULL);
        is_attacking_ex = true;
    }

    timer_attack_ex.Restart();
    is_facing_right ? animation_attack_ex_right.Reset() : animation_attack_ex_left.Reset();

    Bullet* bullet = new BubblesBullet(true);
    bullet->SetPosition(position + size / 2);
    bullet->SetCollideTarget(id == PlayerId::P1 ? PlayerId::P2 : PlayerId::P1);
    bullet->SetCallback([&](){
        mp += 15;
    });

    bullet_list.push_back(bullet);
}