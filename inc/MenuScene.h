#ifndef _MEMU_SCENE_H_
#define _MEMU_SCENE_H_

#include <iostream>
#include "Scene.h"

class MenuScene : public Scene
{
public:
    MenuScene() = default;
    ~MenuScene() = default;

    void on_enter() override {
        std::cout << "into Main Menu" << std::endl;
    }
    void on_update() override {
        std::cout << "Main Menu running" << std::endl;
    }
    void on_draw() override {
        outtextxy(10, 10, _T("Main Menu painting content"));
    }
    void on_input(const ExMessage& msg) override {};
    void on_exit() override {
        std::cout << "exit Main Menu" << std::endl;
    }

private:

};

#endif // _MEMU_SCENE_H_