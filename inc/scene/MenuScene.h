#ifndef _MEMU_SCENE_H_
#define _MEMU_SCENE_H_

#include <iostream>

#include "Atlas.h"
#include "Animation.h"
#include "Camera.h"
#include "Scene.h"
#include "SceneManager.h"

extern Atlas atlas_peashooter_run_right;
extern SceneManager scene_manager;

class MenuScene : public Scene {
public:
    MenuScene() = default;
    ~MenuScene() = default;

    void on_enter() override {
        anim_peashooter_run_right.set_atlas(&atlas_peashooter_run_right);
        anim_peashooter_run_right.set_interval(75);
        anim_peashooter_run_right.set_loop(true);
    }
    void on_update(int delta) override {
        camera.on_update(delta);
        anim_peashooter_run_right.on_update(delta);
    }
    void on_draw() override {
        const Vector2& pos = camera.get_position();
        anim_peashooter_run_right.on_draw((int)(100 - pos.x), (int)(100 - pos.y));
    }
    void on_input(const ExMessage& msg) override {
        if(msg.message == WM_KEYDOWN){
            scene_manager.switch_to(SceneManager::SceneType::Game);
        }
    };
    void on_exit() override {
        std::cout << "exit Main Menu" << std::endl;
    }

private:
    Camera camera;
    Animation anim_peashooter_run_right;
};

#endif // _MEMU_SCENE_H_