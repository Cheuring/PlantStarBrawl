#include "MySocket.h"
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
bool is_stop_menu_bgm = false;
bool is_start_menu_bgm = false;
bool is_stop_game_bgm = false;

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

    initgraph(1280, 720, SHOWCONSOLE);

    settextstyle(28, 0, _T("zpix"));
    setbkmode(TRANSPARENT);

    BeginBatchDraw();

    menu_scene = new MenuScene();
    // game_scene = new GameScene();
    selector_scene = new SelectorScene();

    scene_manager.SetCurrentScene(menu_scene);

    while(true){
        if(is_stop_menu_bgm){
            mciSendString(_T("stop bgm_menu"), NULL, 0, NULL);
            is_stop_menu_bgm = false;
        }
        if(is_start_menu_bgm){
            mciSendString(_T("play bgm_menu repeat from 0"), NULL, 0, NULL);
            is_start_menu_bgm = false;
        }
        if(is_stop_game_bgm){
            mciSendString(_T("stop bgm_game"), NULL, 0, NULL);
            is_stop_game_bgm = false;
        }

        DWORD frame_start_time = GetTickCount();

        while(peekmessage(&msg)){
            scene_manager.OnInput(msg, true);
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