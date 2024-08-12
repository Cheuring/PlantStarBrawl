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

    void on_enter() override {
        mciSendString(_T("play bgm_menu repeat from 0"), NULL, 0, NULL);
    }
    void on_update(int delta) override {

    }
    void on_draw(const Camera& camera) override {
        putimage(0, 0, &img_menu_background);
    }
    void on_input(const ExMessage& msg) override {
        if(msg.message == WM_KEYUP){
            mciSendString(_T("play ui_confirm from 0"), NULL, 0, NULL);
            scene_manager.switch_to(SceneManager::SceneType::Selector);
        }
    };

private:
};

#endif // _MEMU_SCENE_H_