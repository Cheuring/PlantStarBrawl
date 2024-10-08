#ifndef _SCENE_MANAGER_H
#define _SCENE_MANAGER_H

#include "GameType.h"
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

    SceneManager(GameType type);
    ~SceneManager() = default;

    void SetCurrentScene(Scene* scene);
    void SwitchTo(SceneType type);
    void OnInput(const ExMessage& msg, bool is_1P);
    void OnInput(const ExMessage& msg);
    void OnUpdate(int delta);
    void OnDraw(const Camera& camera);
    auto GetGameType() const -> GameType { return game_type; }
    void SetGameType(const GameType& type) { game_type = type; }


private:
    Scene* current_scene = nullptr;
    GameType game_type;
};


#endif // _SCENE_MANAGER_H