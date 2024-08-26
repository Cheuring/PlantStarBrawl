#ifndef _MEMU_SCENE_H_
#define _MEMU_SCENE_H_

#include "GameType.h"
#include "Scene.h"
#include "SceneManager.h"

extern IMAGE img_menu_background;
extern SceneManager scene_manager;
extern Scene* game_scene;

class MenuScene : public Scene {
public:
    MenuScene() = default;
    ~MenuScene() = default;

    void OnEnter() override;
    void OnUpdate(int delta) override {}
    void OnDraw(const Camera& camera) override;
    void OnInput(const ExMessage& msg, bool is_1P) override;
    void OnInput(const ExMessage& msg) override;
    void OnExit(GameType type) override {}

private:
};

#endif // _MEMU_SCENE_H_