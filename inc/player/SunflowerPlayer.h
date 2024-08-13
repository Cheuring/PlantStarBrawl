#ifndef _SUNFLOWER_PLAYER_H_
#define _SUNFLOWER_PLAYER_H_

#include <iostream>

#include "Atlas.h"
#include "MediaSource.h"
#include "Player.h"

extern Atlas atlas_sunflower_idle_left;
extern Atlas atlas_sunflower_idle_right;
extern Atlas atlas_sunflower_run_left;
extern Atlas atlas_sunflower_run_right;

class SunflowerPlayer : public Player {
public:
    SunflowerPlayer() : Player() {
        animation_idle_left.set_atlas(&atlas_sunflower_idle_left);
        animation_idle_right.set_atlas(&atlas_sunflower_idle_right);
        animation_run_left.set_atlas(&atlas_sunflower_run_left);
        animation_run_right.set_atlas(&atlas_sunflower_run_right);

        animation_idle_left.set_interval(75);
        animation_idle_right.set_interval(75);
        animation_run_left.set_interval(75);
        animation_run_right.set_interval(75);

        size.x = 96, size.y = 96;
    }

    void on_update(int delta) override {
        Player::on_update(delta);
        std::cout << "SunflowerPlayer::on_update" << std::endl;
    }
    
    ~SunflowerPlayer() = default;



private:

};

#endif // _SUNFLOWER_PLAYER_H_