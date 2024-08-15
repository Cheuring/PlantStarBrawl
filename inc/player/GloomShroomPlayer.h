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
    GloomShroomPlayer() : Player() {
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

        size.x = 96, size.y = 96;

    }
    ~GloomShroomPlayer() = default;

    void on_attack() override {

    }

    void on_attack_ex() override {

    }

private:


};




#endif // _GLOOM_SHROOM_PLAYER_H_