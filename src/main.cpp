#include <graphics.h>

#include "Atlas.h"
#include "Camera.h"
#include "GameScene.h"
#include "MediaSource.h"
#include "MenuScene.h"
#include "Scene.h"
#include "SelectorScene.h"
#include "SceneManager.h"
#include "util.h"

Scene* menu_scene = nullptr;
Scene* game_scene = nullptr;
Scene* selector_scene = nullptr;

Camera main_camera;
SceneManager scene_manager;

int main(){
    ExMessage msg;

    load_game_resources();

    initgraph(1280, 720, EW_SHOWCONSOLE);

    settextstyle(28, 0, _T("zpix"));
    setbkmode(TRANSPARENT);

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

        static DWORD last_tick_time = GetTickCount();
        DWORD current_tick_time = GetTickCount();
        DWORD delta = current_tick_time - last_tick_time;
        scene_manager.on_update(delta);
        last_tick_time = current_tick_time;

        cleardevice();
        scene_manager.on_draw(main_camera);
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