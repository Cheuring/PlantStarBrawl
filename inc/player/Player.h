#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <graphics.h>
#include <vector>
#include <list>

#include "Animation.h"
#include "BuffId.h"
#include "Bullet.h"
#include "BuffBullet.h"
#include "Camera.h"
#include "Timer.h"
#include "Particle.h"
#include "Platform.h"
#include "PlayerId.h"
#include "util.h"
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
    Player(int attack_cd = 350);
    ~Player() = default;

    virtual void OnUpdate(int delta);
    virtual void OnRun(int distance);
    virtual void OnJump();
    virtual void OnLand();
    virtual void OnDrop();
    virtual void OnDraw(const Camera& camera);
    virtual void OnInput(const ExMessage& msg);
    virtual void OnInputLocal(const ExMessage& msg);

    void SetId(PlayerId ID);
    void SetPosition(int x, int y);
    auto GetPosition() const -> const Vector2<int>&;
    auto GetSize() const -> const Vector2<int>&;
    auto GetId() const -> PlayerId;
    auto GetHp() const -> int;
    void SetHp(int hp);
    auto GetMp() const -> int;
    void MakeInvulnerable();
    auto GetBuffList() const -> const std::list<BuffId>&;
    void SetLocal(bool is_local);

    virtual void OnAttack() = 0;
    virtual void OnAttackEx() = 0;

protected:
    const float gravity = 1.6e-3f;
    const float jump_velocity = -0.85f;
    float run_velocity = 0.55f;

    int mp = 0;     //  Magic Point
    int hp = 100;   //  Health Point
    
    Vector2<int> position;
    Vector2<float> velocity;
    Vector2<int> size;
    Vector2<int> collision_offset;

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

    Vector2<int> position_jump_effect;
    Vector2<int> position_land_effect;

    Animation* current_animation = nullptr;

    PlayerId id = PlayerId::P1;

    bool is_left_key_down = false;
    bool is_right_key_down = false;

    bool is_facing_right = true;

    int attack_cd;
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

    Vector2<int> last_hurt_direction;

    std::list<BuffId> buff_list;
    std::unordered_map<BuffId, decltype(buff_list.begin())> buffid_map;
    Timer timer_buff_recover_hp;
    Timer timer_buff_recover_hp_helper;
    Timer timer_buff_recover_mp;
    Timer timer_buff_recover_mp_helper;
    Timer timer_buff_hurry;
    Timer timer_buff_invisible;

    bool is_invisible = false;
    bool is_local;

protected:
    void AddBuff(BuffId buffid);
    void DeleteBuff(BuffId buffid);
    void MoveAndCollide(int delta);
};


#endif // _PLAYER_H_