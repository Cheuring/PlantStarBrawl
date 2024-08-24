#ifndef _MEMU_SCENE_H_
#define _MEMU_SCENE_H_

#include "Scene.h"
#include "SceneManager.h"

extern IMAGE img_menu_background;
extern SceneManager scene_manager;

class MenuScene : public Scene {
public:
    MenuScene() = default;
    ~MenuScene() = default;

    void OnEnter() override;
    void OnUpdate(int delta) override {}
    void OnDraw(const Camera& camera) override;
    void OnInput(const ExMessage& msg) override;
    void OnExit() override {}

private:
};

#endif // _MEMU_SCENE_H_