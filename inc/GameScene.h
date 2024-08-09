#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include <iostream>
#include "Scene.h"

class GameScene : public Scene{
public:
    GameScene() = default;
    ~GameScene() = default;

    void on_enter() override {
        std::cout << "GameScene on_enter" << std::endl;
    }
    void on_exit() override {
        std::cout << "GameScene on_exit" << std::endl;
    }
    void on_input(const ExMessage& msg) override {
        std::cout << "GameScene on_input" << std::endl;
    }
    void on_update() override {
        std::cout << "GameScene on_update" << std::endl;
    }
    void on_draw() override {
        std::cout << "GameScene on_draw" << std::endl;
    }

private:

};

#endif // _GAME_SCENE_H_