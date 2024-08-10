#include <graphics.h>

#include "Scene.h"
#include "MenuScene.h"
#include "GameScene.h"
#include "SelectorScene.h"
#include "SceneManager.h"

Scene* menu_scene = nullptr;
Scene* game_scene = nullptr;
Scene* selector_scene = nullptr;

SceneManager scene_manager;

int main(){
    ExMessage msg;
    const int FPS = 60;

    initgraph(1280, 720, EW_SHOWCONSOLE);

    BeginBatchDraw();

    menu_scene = new MenuScene();
    game_scene = new GameScene();
    selector_scene = new SelectorScene();

    scene_manager.set_current_scene(menu_scene);

    while(true){
        DWORD frame_start_time = GetTickCount();

        while(peekmessage(&msg)){
            scene_manager.on_input(msg);
        }

        scene_manager.on_update();

        cleardevice();
        scene_manager.on_draw();
        FlushBatchDraw();

        DWORD frame_end_time = GetTickCount();
        DWORD frame_duration = frame_end_time - frame_start_time;
        if(frame_duration < 1000 / FPS){
            Sleep(1000 / FPS - frame_duration);
        }
    }

    // scene->on_exit();
    // delete scene;

    EndBatchDraw();
    closegraph();
    
}