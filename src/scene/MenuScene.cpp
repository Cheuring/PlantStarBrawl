#include "MenuScene.h"
#include <iostream>

void MenuScene::OnEnter() {
    mciSendString(_T("play bgm_menu repeat from 0"), NULL, 0, NULL);
    // if(game_scene != nullptr){
    //     delete game_scene;
    //     game_scene = nullptr;
    // }
}

void MenuScene::OnDraw(const Camera& camera) {
    putimage(0, 0, &img_menu_background);
}

void MenuScene::OnInput(const ExMessage& msg, bool is_1P) {
    if(msg.message == WM_KEYUP){
        mciSendString(_T("play ui_confirm from 0"), NULL, 0, NULL);
        // std::cout<< "start playing ui_confirm" << std::endl;
        scene_manager.SwitchTo(SceneManager::SceneType::Selector);
    }
}

void MenuScene::OnInput(const ExMessage& msg) {
    if(msg.message == WM_KEYUP){
        mciSendString(_T("play ui_confirm from 0"), NULL, 0, NULL);
        // std::cout<< "start playing ui_confirm" << std::endl;
        scene_manager.SwitchTo(SceneManager::SceneType::Selector);
    }
}