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
    NutPlayer();
    ~NutPlayer() = default;

    void OnUpdate(int delta) override;
    void OnAttack() override;
    void OnAttackEx() override;
    void NutExplode();

private:
    const int dash_distance = 150;

    Timer timer_recovery;
};


#endif // _NUT_PLAYER_H_