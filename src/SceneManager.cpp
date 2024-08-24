#include "SceneManager.h"

void SceneManager::SetCurrentScene(Scene* scene){
    current_scene = scene;
    current_scene->OnEnter();
}

void SceneManager::SwitchTo(SceneType type){
    current_scene->OnExit();
    switch (type)
    {
    case SceneType::Menu:
        current_scene = menu_scene;
        break;
    case SceneType::Game:
        current_scene = game_scene;
        break;
    case SceneType::Selector:
        current_scene = selector_scene;
        break;
    default:
        break;
    }

    current_scene->OnEnter();
}

void SceneManager::OnInput(const ExMessage& msg){
    current_scene->OnInput(msg);
}

void SceneManager::OnUpdate(int delta){
    current_scene->OnUpdate(delta);
}

void SceneManager::OnDraw(const Camera& camera){
    current_scene->OnDraw(camera);
}