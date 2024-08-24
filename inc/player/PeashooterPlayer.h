#ifndef _PEASHOOTER_PLAYER_H_
#define _PEASHOOTER_PLAYER_H_

#include "Atlas.h"
#include "Camera.h"
#include "PeaBullet.h"
#include "Player.h"
#include "Timer.h"

extern Atlas atlas_peashooter_idle_left;
extern Atlas atlas_peashooter_idle_right;
extern Atlas atlas_peashooter_run_left;
extern Atlas atlas_peashooter_run_right;
extern Atlas atlas_peashooter_attack_ex_left;
extern Atlas atlas_peashooter_attack_ex_right;
extern Atlas atlas_peashooter_die_left;
extern Atlas atlas_peashooter_die_right;

extern Camera main_camera;


class PeashooterPlayer : public Player {
public:
    PeashooterPlayer();
    ~PeashooterPlayer() = default;

    void OnUpdate(int delta) override;
    void OnAttack() override;
    void OnAttackEx() override;

private:
    const float speed_pea = 0.75f;
    const float speed_pea_ex = 1.5f;
    const int attack_ex_duration = 2500;

    Timer timer_attck_ex;
    Timer timer_spwan_pea_ex;
    

private:
    void SpwanPeaBullet(float speed);
};

#endif // _PEASHOOTER_PLAYER_H_