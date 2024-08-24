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
    GloomShroomPlayer();
    ~GloomShroomPlayer() = default;

    void OnUpdate(int delta) override;
    void OnAttack() override;
    void OnAttackEx() override;

private:
    const int ex_attack_times = 5;
    const int ex_attack_interval = 75 * 7;

    Timer timer_attack_ex;
    int current_times = 1;

};




#endif // _GLOOM_SHROOM_PLAYER_H_