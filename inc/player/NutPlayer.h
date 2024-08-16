#ifndef _NUT_PLAYER_H_
#define _NUT_PLAYER_H_

#include "Player.h"

class NutPlayer : public Player {
public:
    NutPlayer() = default;
    ~NutPlayer() = default;

    void on_attack() override { }
    void on_attack_ex() override { }

private:

};


#endif // _NUT_PLAYER_H_