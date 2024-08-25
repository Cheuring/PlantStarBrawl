#include <graphics.h>
#include <windows.h>
#include <vector>

#include "Atlas.h"
#include "BuffBullet.h"
#include "Bullet.h"
#include "Camera.h"
#include "GameScene.h"
#include "MediaSource.h"
#include "MenuScene.h"
#include "Platform.h"
#include "Player.h"
#include "Scene.h"
#include "SelectorScene.h"
#include "SceneManager.h"
#include "util.h"

bool is_debug = false;

Scene* menu_scene = nullptr;
Scene* game_scene = nullptr;
Scene* selector_scene = nullptr;

Camera main_camera;
SceneManager scene_manager;

std::vector<Bullet *> bullet_list;
std::vector<BuffBullet *> buff_bullet_list;
std::vector<Platform> platform_list;

Player* player_1 = nullptr;
Player* player_2 = nullptr;

IMAGE* img_player_1_avatar = nullptr;
IMAGE* img_player_2_avatar = nullptr;

int main(){
    ExMessage msg;

    LoadGameResources();

    initgraph(1280, 720);

    settextstyle(28, 0, _T("zpix"));
    setbkmode(TRANSPARENT);

    BeginBatchDraw();

    menu_scene = new MenuScene();
    selector_scene = new SelectorScene();

    scene_manager.SetCurrentScene(menu_scene);

    while(true){
        DWORD frame_start_time = GetTickCount();

        while(peekmessage(&msg)){
            scene_manager.OnInput(msg);
        }

        static DWORD last_tick_time = GetTickCount();
        DWORD current_tick_time = GetTickCount();
        DWORD delta = current_tick_time - last_tick_time;
        scene_manager.OnUpdate(delta);
        last_tick_time = current_tick_time;

        cleardevice();
        scene_manager.OnDraw(main_camera);
        FlushBatchDraw();

        DWORD frame_end_time = GetTickCount();
        DWORD frame_duration = frame_end_time - frame_start_time;
        if(frame_duration < 1000 / FPS){
            Sleep(1000 / FPS - frame_duration);
        }
    }

    // scene->OnExit();
    // delete scene;

    EndBatchDraw();
    closegraph();
    
}