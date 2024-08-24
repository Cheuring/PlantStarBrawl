#include "MenuScene.h"

void MenuScene::OnEnter() {
    mciSendString(_T("play bgm_menu repeat from 0"), NULL, 0, NULL);
}

void MenuScene::OnDraw(const Camera& camera) {
    putimage(0, 0, &img_menu_background);
}

void MenuScene::OnInput(const ExMessage& msg) {
    if(msg.message == WM_KEYUP){
        mciSendString(_T("play ui_confirm from 0"), NULL, 0, NULL);
        scene_manager.SwitchTo(SceneManager::SceneType::Selector);
    }
}