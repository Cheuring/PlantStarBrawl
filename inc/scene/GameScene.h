#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include <iostream>

#include "Scene.h"
#include "SceneManager.h"

extern SceneManager scene_manager;

class GameScene : public Scene {
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
        if(msg.message == WM_KEYDOWN){
            scene_manager.switch_to(SceneManager::SceneType::Menu);
        }
    }
    void on_update(int delta) override {
        std::cout << "GameScene on_update" << std::endl;
    }
    void on_draw(const Camera& camera) override {
        outtextxy(10, 10, _T("GameScene on_draw"));
    }

private:

};

#endif // _GAME_SCENE_H_