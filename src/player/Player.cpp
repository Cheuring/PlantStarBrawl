#include "Player.h"
#include <iostream>

Player::Player(int attack_cd) : attack_cd(attack_cd) {
    current_animation = &animation_idle_right;

    timer_attack_cd.SetWaitTime(attack_cd);
    timer_attack_cd.SetOneShot(true);
    timer_attack_cd.SetCallback([&](){
        can_attack = true;
    });

    timer_invulnerable.SetWaitTime(750);
    timer_invulnerable.SetOneShot(true);
    timer_invulnerable.SetCallback([&](){
        is_invulnerable = false;
    });

    timer_invulnerable_blink.SetWaitTime(75);
    timer_invulnerable_blink.SetCallback([&](){
        is_showing_sketch_frame = !is_showing_sketch_frame;
    });

    timer_run_effect_generation.SetWaitTime(75);
    timer_run_effect_generation.SetCallback([&](){
        if(velocity.y != 0){
            return;
        }
        Vector2 particle_position;
        IMAGE* frame = atlas_run_effect.GetImage(0);
        particle_position.x = position.x + (size.x - frame->getwidth()) / 2;
        particle_position.y = position.y + size.y - frame->getheight();
        particle_list.emplace_back(particle_position, 45, &atlas_run_effect);
    });

    timer_die_effect_generation.SetWaitTime(35);
    timer_die_effect_generation.SetCallback([&](){
        Vector2 particle_position;
        IMAGE* frame = atlas_run_effect.GetImage(0);
        particle_position.x = position.x + (size.x - frame->getwidth()) / 2;
        particle_position.y = position.y + size.y - frame->getheight();
        particle_list.emplace_back(particle_position, 150, &atlas_run_effect);
    });

    animation_jump_effect.SetAtlas(&atlas_jump_effect);
    animation_jump_effect.SetInterval(25);
    animation_jump_effect.SetLoop(false);
    animation_jump_effect.SetOnFinish([&](){
        is_jump_effect_visible = false;
    });

    animation_land_effect.SetAtlas(&atlas_land_effect);
    animation_land_effect.SetInterval(50);
    animation_land_effect.SetLoop(false);
    animation_land_effect.SetOnFinish([&](){
        is_land_effect_visible = false;
    });

    timer_cursor_visible.SetWaitTime(2500);
    timer_cursor_visible.SetOneShot(true);
    timer_cursor_visible.SetCallback([&](){
        is_cursor_visible = false;
    });

    timer_buff_hurry.SetWaitTime(4000);
    timer_buff_hurry.SetOneShot(true);
    timer_buff_hurry.SetCallback([&](){
        DeleteBuff(BuffId::HURRY);
    });

    timer_buff_invisible.SetWaitTime(8000);
    timer_buff_invisible.SetOneShot(true);
    timer_buff_invisible.SetCallback([&](){
        DeleteBuff(BuffId::INVISIBLE);
    });

    timer_buff_recover_hp.SetWaitTime(5000);
    timer_buff_recover_hp.SetOneShot(true);
    timer_buff_recover_hp.SetCallback([&](){
        DeleteBuff(BuffId::RECOVER_HP);
    });

    timer_buff_recover_hp_helper.SetWaitTime(1000);
    timer_buff_recover_hp_helper.SetOneShot(false);
    timer_buff_recover_hp_helper.SetCallback([&](){
        hp += 5;
        hp = std::min(hp, 100);
    });

    timer_buff_recover_mp.SetWaitTime(5000);
    timer_buff_recover_mp.SetOneShot(true);
    timer_buff_recover_mp.SetCallback([&](){
        DeleteBuff(BuffId::RECOVER_MP);
    });
    timer_buff_recover_mp_helper.Pause();

    timer_buff_recover_mp_helper.SetWaitTime(1000);
    timer_buff_recover_mp_helper.SetOneShot(false);
    timer_buff_recover_mp_helper.SetCallback([&](){
        mp += 5;
        mp = std::min(mp, 100);
    });
    timer_buff_recover_hp_helper.Pause();
}

void Player::OnUpdate(int delta) {
    int direction = is_right_key_down - is_left_key_down;

    if(direction != 0){
        if(!is_attacking_ex)
            is_facing_right = direction > 0;

        current_animation = is_facing_right ? &animation_run_right : &animation_run_left;
        float distance = run_velocity * delta * direction;
        OnRun(distance);
    }else {
        current_animation = is_facing_right ? &animation_idle_right : &animation_idle_left;
        timer_run_effect_generation.Pause();
    }

    if(is_attacking_ex){
        current_animation = is_facing_right ? &animation_attack_ex_right : &animation_attack_ex_left;
    }

    current_animation->OnUpdate(delta);
    animation_jump_effect.OnUpdate(delta);
    animation_land_effect.OnUpdate(delta);

    timer_attack_cd.OnUpdate(delta);
    timer_invulnerable.OnUpdate(delta);
    timer_invulnerable_blink.OnUpdate(delta);
    timer_run_effect_generation.OnUpdate(delta);
    timer_cursor_visible.OnUpdate(delta);

    timer_buff_hurry.OnUpdate(delta);
    timer_buff_invisible.OnUpdate(delta);
    timer_buff_recover_hp.OnUpdate(delta);
    timer_buff_recover_mp.OnUpdate(delta);
    timer_buff_recover_hp_helper.OnUpdate(delta);
    timer_buff_recover_mp_helper.OnUpdate(delta);

    if(hp <= 0){
        current_animation = last_hurt_direction.x < 0 ? &animation_die_left : &animation_die_right;
    }

    if(hp <= 0){
        timer_die_effect_generation.OnUpdate(delta);
    }

    particle_list.erase(std::remove_if(particle_list.begin(), particle_list.end(), [&](Particle& particle){
        return !particle.CheckValid();
    }), particle_list.end());

    for(auto& particle : particle_list){
        particle.OnUpdate(delta);
    }

    if(is_showing_sketch_frame){
        SketchImage(current_animation->GetCurrentFrame(), &img_sketch);
    }

    MoveAndCollide(delta);
}

void Player::OnRun(float distance) {
    if(is_attacking_ex){
        return;
    }

    position.x += distance;
    timer_run_effect_generation.Resume();
}

void Player::OnJump() {
    if(velocity.y != 0 || is_attacking_ex){
        return;
    }

    velocity.y = jump_velocity;
    is_jump_effect_visible = true;
    animation_jump_effect.Reset();

    IMAGE* effect_frame = animation_jump_effect.GetCurrentFrame();
    position_jump_effect.x = position.x + (size.x - effect_frame->getwidth()) / 2;
    position_jump_effect.y = position.y + size.y - effect_frame->getheight();
}

void Player::OnLand() {
    is_land_effect_visible = true;
    animation_land_effect.Reset();

    IMAGE* effect_frame = animation_land_effect.GetCurrentFrame();
    position_land_effect.x = position.x + (size.x - effect_frame->getwidth()) / 2;
    position_land_effect.y = position.y + size.y - effect_frame->getheight();
}

void Player::OnDrop() {
    if(velocity.y == 0){
        if(position.y + size.y == platform_list[0].shape.y){
            return;
        }
        position.y += 1;
        velocity.y = 0.1f;
    }
}

void Player::OnDraw(const Camera& camera) {
    if(is_jump_effect_visible){
        animation_jump_effect.OnDraw(camera, position_jump_effect.x, position_jump_effect.y);
    }

    if(is_land_effect_visible){
        animation_land_effect.OnDraw(camera, position_land_effect.x, position_land_effect.y);
    }

    for(const auto& particle : particle_list){
        particle.OnDraw(camera);
    }

    if(!is_invisible){
        if(hp > 0 && is_invulnerable && is_showing_sketch_frame){
            PutImageAlpha(camera, position.x, position.y, &img_sketch);
        }else{
            current_animation->OnDraw(camera, position.x, position.y);
        }
    }

    if(is_cursor_visible){
        switch (id) {
            case PlayerId::P1:
                PutImageAlpha(camera, position.x + (size.x - img_1P_cursor.getwidth()) / 2,
                    position.y - img_1P_cursor.getheight() , &img_1P_cursor);
                break;
            case PlayerId::P2:
                PutImageAlpha(camera, position.x + (size.x - img_2P_cursor.getwidth()) / 2,
                    position.y - img_2P_cursor.getheight() , &img_2P_cursor);
                break;
        }
    }

    if(is_debug || (is_invisible && is_local)){
        setfillcolor(RGB(0, 0, 255));
        solidcircle(position.x, position.y, 3);
        
        setlinecolor(RGB(0, 125, 125));
        rectangle(position.x + collision_offset.x, position.y + collision_offset.y, position.x + size.x - collision_offset.x, position.y + size.y);
    }
}

void Player::OnInput(const ExMessage& msg) {
    switch (msg.message){
        case WM_KEYDOWN:
            switch (msg.vkcode) {
                case 'A':
                case 'a':
                    is_left_key_down = true;
                    std::cout << (id == PlayerId::P1 ? "P1" : "P2") << " left key down" << std::endl;
                    break;
                case 'D':
                case 'd':
                    is_right_key_down = true;
                    std::cout << (id == PlayerId::P1 ? "P1" : "P2") << " right key down" << std::endl;
                    break;
                case 'W':
                case 'w':
                    OnJump();
                    break;
                case 'S':
                case 's':
                    OnDrop();
                    break;
                case 'J':
                case 'j':
                    if(can_attack){
                        OnAttack();
                        can_attack = false;
                        timer_attack_cd.Restart();
                    }
                    break;
                case 'K':
                case 'k':
                    if(mp >= 100){
                        OnAttackEx();

                        if(!is_debug)
                            mp = 0;
                    }
                    break;
                default:
                    break;
            }
            break;
        case WM_KEYUP:
            switch(msg.vkcode){
                case 'A':
                case 'a':
                    is_left_key_down = false;
                    std::cout << (id == PlayerId::P1 ? "P1" : "P2") << " left key up" << std::endl;
                    break;
                case 'D':
                case 'd':
                    is_right_key_down = false;
                    std::cout << (id == PlayerId::P1 ? "P1" : "P2") << " right key up" << std::endl;
                    break;
                default:
                    break;
            }
    }
}

void Player::OnInputLocal(const ExMessage& msg) {
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
                            OnJump();
                            break;
                        case 'S':
                        case 's':
                            OnDrop();
                            break;
                        case 'F':
                        case 'f':
                            if(can_attack){
                                OnAttack();
                                can_attack = false;
                                timer_attack_cd.Restart();
                            }
                            break;
                        case 'G':
                        case 'g':
                            if(mp >= 100){
                                OnAttackEx();

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
                            OnJump();
                            break;
                        case VK_DOWN:
                            OnDrop();
                            break;
                            //  . key
                        case VK_OEM_PERIOD:
                            if(can_attack){
                                OnAttack();
                                can_attack = false;
                                timer_attack_cd.Restart();
                            }
                            break;
                            //  / key
                        case VK_OEM_2:
                            if(mp >= 100){
                                OnAttackEx();

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

void Player::SetId(PlayerId Id) {
    if(Id == PlayerId::P2) {
        is_facing_right = false;
    }
    this->id = Id;
}

void Player::SetPosition(float x, float y) {
    position.x = x;
    position.y = y;
}

auto Player::GetPosition() const -> const Vector2& {
    return position;
}

auto Player::GetSize() const -> const Vector2& {
    return size;
}

auto Player::GetId() const -> PlayerId {
    return id;
}

auto Player::GetHp() const -> int {
    return hp;
}

void Player::SetHp(int hp) {
    this->hp = hp;
}

auto Player::GetMp() const -> int {
    return mp;
}

void Player::MakeInvulnerable() {
    is_invulnerable = true;
    timer_invulnerable.Restart();
    timer_invulnerable_blink.Restart();
}

auto Player::GetBuffList() const -> const std::list<BuffId>& {
    return buff_list;
}

void Player::SetLocal(bool is_local) {
    this->is_local = is_local;
}

void Player::AddBuff(BuffId buffid) {
    switch(buffid){
        case BuffId::RECOVER_HP:
            if(buffid_map.find(BuffId::RECOVER_HP) == buffid_map.end()){
                buff_list.push_back(BuffId::RECOVER_HP);
                buffid_map[BuffId::RECOVER_HP] = std::prev(buff_list.end());
                timer_buff_recover_hp.Restart();
                timer_buff_recover_hp_helper.Restart();
                timer_buff_recover_hp_helper.Resume();
            }else{
                buff_list.splice(buff_list.end(), buff_list, buffid_map[BuffId::RECOVER_HP]);
                timer_buff_recover_hp.Restart();
                timer_buff_recover_hp_helper.Restart();
            }
            break;
        case BuffId::RECOVER_MP:
            if(buffid_map.find(BuffId::RECOVER_MP) == buffid_map.end()){
                buff_list.push_back(BuffId::RECOVER_MP);
                buffid_map[BuffId::RECOVER_MP] = std::prev(buff_list.end());
                timer_buff_recover_mp.Restart();
                timer_buff_recover_mp_helper.Restart();
                timer_buff_recover_mp_helper.Resume();
            }else{
                buff_list.splice(buff_list.end(), buff_list, buffid_map[BuffId::RECOVER_MP]);
                timer_buff_recover_mp.Restart();
                timer_buff_recover_mp_helper.Restart();
            }
            break;
        case BuffId::HURRY:
            if(buffid_map.find(BuffId::HURRY) == buffid_map.end()){
                buff_list.push_back(BuffId::HURRY);
                buffid_map[BuffId::HURRY] = std::prev(buff_list.end());
                run_velocity *= 2;
                timer_buff_hurry.Restart();
            }else{
                buff_list.splice(buff_list.end(), buff_list, buffid_map[BuffId::HURRY]);
                timer_buff_hurry.Restart();
            }
            break;
        case BuffId::INVISIBLE:
            if(buffid_map.find(BuffId::INVISIBLE) == buffid_map.end()){
                buff_list.push_back(BuffId::INVISIBLE);
                buffid_map[BuffId::INVISIBLE] = std::prev(buff_list.end());
                is_invisible = true;
                timer_buff_invisible.Restart();
            }else{
                buff_list.splice(buff_list.end(), buff_list, buffid_map[BuffId::INVISIBLE]);
                timer_buff_invisible.Restart();
            }
            break;
        default:
            break;
    }
}

void Player::DeleteBuff(BuffId buffid) {
    switch(buffid) {
        case BuffId::RECOVER_HP:
            if(buffid_map.find(BuffId::RECOVER_HP) != buffid_map.end()){
                buff_list.erase(buffid_map[BuffId::RECOVER_HP]);
                buffid_map.erase(BuffId::RECOVER_HP);
                timer_buff_recover_hp_helper.Pause();
            }
            break;
        case BuffId::RECOVER_MP:
            if(buffid_map.find(BuffId::RECOVER_MP) != buffid_map.end()){
                buff_list.erase(buffid_map[BuffId::RECOVER_MP]);
                buffid_map.erase(BuffId::RECOVER_MP);
                timer_buff_recover_mp_helper.Pause();
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

void Player::MoveAndCollide(int delta) {
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
                        OnLand();
                    }
                    break;
                }
            }
        }

    }

    if(!is_invulnerable){
        for(Bullet* bullet : bullet_list){
            if(bullet->GetCollideTarget() != id || !bullet->GetValid()){
                continue;
            }

            if(bullet->CheckCollision(position + collision_offset, {size.x - collision_offset.x * 2, size.y - collision_offset.y})){
                if(!is_debug)
                    hp -= bullet->GetDamage();

                MakeInvulnerable();
                last_hurt_direction = bullet->GetPosition() - position;

                bullet->OnCollide();
                bullet->SetValid(false);

                if(hp <= 0) {
                    velocity.x = last_hurt_direction.x < 0 ? 0.35f : -0.35f;
                    velocity.y = -1.f;
                }
            }
        }
    }

    for(BuffBullet* buff : buff_bullet_list){
        if(!buff->GetValid()){
            continue;
        }

        if(buff->CheckCollision(position + collision_offset, {size.x - collision_offset.x * 2, size.y - collision_offset.y})){
            AddBuff(buff->GetBuffId());
            buff->OnCollide();
            buff->SetValid(false);
        }
    }
}