#ifndef _SCENE_MANAGER_H
#define _SCENE_MANAGER_H

#include "Scene.h"

extern Scene* menu_scene;
extern Scene* game_scene;
extern Scene* selector_scene;

class SceneManager{
public:
    enum class SceneType{
        Menu,
        Game,
        Selector,
    };

    SceneManager() = default;
    ~SceneManager() = default;

    void SetCurrentScene(Scene* scene);
    void SwitchTo(SceneType type);
    void OnInput(const ExMessage& msg, bool is_1P);
    void OnInput(const ExMessage& msg);
    void OnUpdate(int delta);
    void OnDraw(const Camera& camera);


private:
    Scene* current_scene = nullptr;
};


#endif // _SCENE_MANAGER_H