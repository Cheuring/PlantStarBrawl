#ifndef _SELECTOR_SCENE_H_
#define _SELECTOR_SCENE_H_

#include <iostream>

#include "Scene.h"
#include "SceneManager.h"

extern SceneManager scene_manager;

class SelectorScene : public Scene {
public:
    SelectorScene() = default;
    ~SelectorScene() = default;

    void on_enter() override {
        std::cout << "into Selector Scene" << std::endl;
    }
    void on_update(int delta) override {
        std::cout << "Selector Scene running" << std::endl;
    }
    void on_draw() override {
        outtextxy(10, 10, _T("Selector Scene painting content"));
    }
    void on_input(const ExMessage& msg) override {
        if(msg.message == WM_KEYDOWN){
            scene_manager.switch_to(SceneManager::SceneType::Game);
        }
    };
    void on_exit() override {
        std::cout << "exit Selector Scene" << std::endl;
    }

private:

};

#endif // _SELECTOR_SCENE_H_