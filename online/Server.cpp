#include "MySocket.h"
#include <graphics.h>
#include <thread>
#include <windows.h>
#include <vector>
#include <iostream>

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

MySocket server(true);
std::string sendBuf;
std::string recvBuf;

void GameCircle() {
    while(true) {
        DWORD frame_start_time = GetTickCount();

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
}

void LocalInput() {
    mciSendString(_T("open " MEDIA_PATH_PREFIX "bgm_game.mp3 alias bgm_game"), NULL, 0, NULL);
    // mciSendString(_T("open " MEDIA_PATH_PREFIX "bgm_menu.mp3 alias bgm_menu"), NULL, 0, NULL);
    // mciSendString(_T("open " MEDIA_PATH_PREFIX "pea_break_1.mp3 alias pea_break_1"), NULL, 0, NULL);
    // mciSendString(_T("open " MEDIA_PATH_PREFIX "pea_break_2.mp3 alias pea_break_2"), NULL, 0, NULL);
    // mciSendString(_T("open " MEDIA_PATH_PREFIX "pea_break_3.mp3 alias pea_break_3"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "pea_shoot_1.mp3 alias pea_shoot_1"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "pea_shoot_2.mp3 alias pea_shoot_2"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "pea_shoot_ex.mp3 alias pea_shoot_ex"), NULL, 0, NULL);
    // mciSendString(_T("open " MEDIA_PATH_PREFIX "sun_explode.mp3 alias sun_explode"), NULL, 0, NULL);
    // mciSendString(_T("open " MEDIA_PATH_PREFIX "sun_explode_ex.mp3 alias sun_explode_ex"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "sun_text.mp3 alias sun_text"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "bubbles_shot.mp3 alias bubbles_shot"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "bubbles_shot_ex.mp3 alias bubbles_shot_ex"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "nut_explode.mp3 alias nut_explode"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "nut_dash.wav alias nut_dash"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "ui_confirm.wav alias ui_confirm"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "ui_switch.wav alias ui_switch"), NULL, 0, NULL);
    // mciSendString(_T("open " MEDIA_PATH_PREFIX "ui_win.wav alias ui_win"), NULL, 0, NULL);
    ExMessage msg;
    while(true) {
        if(is_stop_game_bgm){
            mciSendString(_T("stop bgm_game"), NULL, 0, NULL);
            is_stop_game_bgm = false;
        }

        while(peekmessage(&msg)){
            if(msg.message == WM_KEYDOWN){
                sendBuf.push_back(msg.vkcode);
                sendBuf.push_back('0');
                scene_manager.OnInput(msg, true);
            }else if(msg.message == WM_KEYUP){
                sendBuf.push_back(msg.vkcode);
                sendBuf.push_back('1');
                scene_manager.OnInput(msg, true);
            }
        }
        if(!sendBuf.empty()){
            server.sendMsg(sendBuf);
            std::cout << "Server sendBuf: " << sendBuf << std::endl;
            sendBuf.clear();
        }
    }
}

void HandleRecv() {
    // mciSendString(_T("open " MEDIA_PATH_PREFIX "bgm_game.mp3 alias bgm_game"), NULL, 0, NULL);
    // mciSendString(_T("open " MEDIA_PATH_PREFIX "bgm_menu.mp3 alias bgm_menu"), NULL, 0, NULL);
    // mciSendString(_T("open " MEDIA_PATH_PREFIX "pea_break_1.mp3 alias pea_break_1"), NULL, 0, NULL);
    // mciSendString(_T("open " MEDIA_PATH_PREFIX "pea_break_2.mp3 alias pea_break_2"), NULL, 0, NULL);
    // mciSendString(_T("open " MEDIA_PATH_PREFIX "pea_break_3.mp3 alias pea_break_3"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "pea_shoot_1.mp3 alias pea_shoot_1"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "pea_shoot_2.mp3 alias pea_shoot_2"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "pea_shoot_ex.mp3 alias pea_shoot_ex"), NULL, 0, NULL);
    // mciSendString(_T("open " MEDIA_PATH_PREFIX "sun_explode.mp3 alias sun_explode"), NULL, 0, NULL);
    // mciSendString(_T("open " MEDIA_PATH_PREFIX "sun_explode_ex.mp3 alias sun_explode_ex"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "sun_text.mp3 alias sun_text"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "bubbles_shot.mp3 alias bubbles_shot"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "bubbles_shot_ex.mp3 alias bubbles_shot_ex"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "nut_explode.mp3 alias nut_explode"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "nut_dash.wav alias nut_dash"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "ui_confirm.wav alias ui_confirm"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "ui_switch.wav alias ui_switch"), NULL, 0, NULL);
    // mciSendString(_T("open " MEDIA_PATH_PREFIX "ui_win.wav alias ui_win"), NULL, 0, NULL);
    ExMessage msg;
    while(true) {
        server.recvMsg(recvBuf);
        if(!recvBuf.empty()){
            int len = recvBuf.size();
            for(int i = 0; i + 1 < len; i += 2){
                if(recvBuf[i+1] == '1'){
                    msg.message = WM_KEYUP;
                }else{
                    msg.message = WM_KEYDOWN;
                }
                msg.vkcode = recvBuf[i];
                scene_manager.OnInput(msg, false);
            }
            std::cout << "Server recvBuf: " << recvBuf << std::endl;
            recvBuf.clear();
        }
    }
}

int main(){
    LoadGameResources();

    initgraph(1280, 720, EW_SHOWCONSOLE);

    settextstyle(28, 0, _T("zpix"));
    setbkmode(TRANSPARENT);

    BeginBatchDraw();

    menu_scene = new MenuScene();
    // game_scene = new GameScene();
    selector_scene = new SelectorScene();

    scene_manager.SetCurrentScene(menu_scene);

    std::thread thread_send(LocalInput);
    thread_send.detach();

    std::thread thread_recv(HandleRecv);
    thread_recv.detach();

    while(true) {
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


    EndBatchDraw();
    closegraph();
}