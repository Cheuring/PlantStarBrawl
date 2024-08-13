#ifndef _PEASHOOTER_PLAYER_H_
#define _PEASHOOTER_PLAYER_H_

#include <iostream>

#include "Atlas.h"
#include "MediaSource.h"
#include "Player.h"

extern Atlas atlas_peashooter_idle_left;
extern Atlas atlas_peashooter_idle_right;
extern Atlas atlas_peashooter_run_left;
extern Atlas atlas_peashooter_run_right;


class PeashooterPlayer : public Player {
public:
    PeashooterPlayer() : Player() {
        animation_idle_left.set_atlas(&atlas_peashooter_idle_left);
        animation_idle_right.set_atlas(&atlas_peashooter_idle_right);
        animation_run_left.set_atlas(&atlas_peashooter_run_left);
        animation_run_right.set_atlas(&atlas_peashooter_run_right);

        animation_idle_left.set_interval(75);
        animation_idle_right.set_interval(75);
        animation_run_left.set_interval(75);
        animation_run_right.set_interval(75);

        size.x = 96, size.y = 96;
    }

    void on_update(int delta) override {
        Player::on_update(delta);
        std::cout << "PeashooterPlayer::on_update" << std::endl;
    }

    ~PeashooterPlayer() = default;


private:
    
};

#endif // _PEASHOOTER_PLAYER_H_