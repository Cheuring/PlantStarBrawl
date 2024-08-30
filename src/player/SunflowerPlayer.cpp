#include "SunflowerPlayer.h"

SunflowerPlayer::SunflowerPlayer() : Player(400) {
    animation_idle_left.SetAtlas(&atlas_sunflower_idle_left);
    animation_idle_right.SetAtlas(&atlas_sunflower_idle_right);
    animation_run_left.SetAtlas(&atlas_sunflower_run_left);
    animation_run_right.SetAtlas(&atlas_sunflower_run_right);
    animation_attack_ex_left.SetAtlas(&atlas_sunflower_attack_ex_left);
    animation_attack_ex_right.SetAtlas(&atlas_sunflower_attack_ex_right);
    animation_sun_text.SetAtlas(&atlas_sun_text);
    animation_die_left.SetAtlas(&atlas_sunflower_die_left);
    animation_die_right.SetAtlas(&atlas_sunflower_die_right);

    animation_idle_left.SetInterval(75);
    animation_idle_right.SetInterval(75);
    animation_run_left.SetInterval(75);
    animation_run_right.SetInterval(75);
    animation_attack_ex_left.SetInterval(100);
    animation_attack_ex_right.SetInterval(100);
    animation_sun_text.SetInterval(100);
    animation_die_left.SetInterval(150);
    animation_die_right.SetInterval(150);

    animation_die_left.SetLoop(false);
    animation_die_right.SetLoop(false);

    animation_attack_ex_left.SetLoop(false);
    animation_attack_ex_right.SetLoop(false);
    animation_sun_text.SetLoop(false);

    animation_attack_ex_left.SetOnFinish([&](){
        is_attacking_ex = false;
        is_sun_text_visible = false;
    });
    animation_attack_ex_right.SetOnFinish([&](){
        is_attacking_ex = false;
        is_sun_text_visible = false;
    });

    size.x = 96, size.y = 96;
    collision_offset.x = 10;
    collision_offset.y = 10;
}

void SunflowerPlayer::OnUpdate(int delta) {
    Player::OnUpdate(delta);

    if(is_sun_text_visible){
        animation_sun_text.OnUpdate(delta);
    }
}

void SunflowerPlayer::OnDraw(const Camera& camera)  {
    Player::OnDraw(camera);

    if(is_sun_text_visible){
        Vector2<int> text_position;
        IMAGE* frame = animation_sun_text.GetCurrentFrame();
        text_position.x = position.x - (size.x - frame->getwidth()) / 2;
        text_position.y = position.y - frame->getheight();
        animation_sun_text.OnDraw(camera, text_position.x, text_position.y);
    }
}

void SunflowerPlayer::OnAttack() {
    Bullet* bullet = new SunBullet();

    Vector2<int> bullet_position;
    const auto& bullet_size = bullet->GetSize();
    bullet_position.x = position.x + (size.x - bullet_size.x) / 2;
    bullet_position.y = position.y - bullet_size.y;

    bullet->SetPosition(bullet_position);
    bullet->SetVelocity(is_facing_right ? velocity_sun.x : -velocity_sun.x, velocity_sun.y);

    bullet->SetCollideTarget(id == PlayerId::P1 ? PlayerId::P2 : PlayerId::P1);

    bullet->SetCallback([&](){
        mp += 25;
    });

    bullet_list.push_back(bullet);
}

void SunflowerPlayer::OnAttackEx() {
    is_attacking_ex = true;
    is_sun_text_visible = true;

    animation_sun_text.Reset();
    is_facing_right ? animation_attack_ex_right.Reset() : animation_attack_ex_left.Reset();

    Bullet* bullet = new SunBulletEx();
    Player* target_player = id == PlayerId::P1 ? player_2 : player_1;

    Vector2<int> bullet_position;
    Vector2<float> bullet_velocity;
    auto& bullet_size = bullet->GetSize();
    auto& target_size = target_player->GetSize();
    auto& target_position = target_player->GetPosition();
    bullet_position.x = target_position.x + (target_size.x - bullet_size.x) / 2;
    bullet_position.y = -bullet_size.y * 0.75;
    bullet_velocity.x = 0;
    bullet_velocity.y = speed_sun_ex;

    bullet->SetPosition(bullet_position);
    bullet->SetVelocity(bullet_velocity);

    bullet->SetCollideTarget(target_player->GetId());

    bullet->SetCallback([&](){
        mp += 50;
    });

    bullet_list.push_back(bullet);

    mciSendString(_T("play sun_text from 0"), NULL, 0, NULL);
}