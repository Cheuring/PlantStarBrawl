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

MySocket client(false);
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
    ExMessage msg;
    while(true) {
        while(peekmessage(&msg)){
            if(msg.message == WM_KEYDOWN){
                sendBuf.push_back(msg.vkcode);
                sendBuf.push_back('0');
                scene_manager.OnInput(msg, false);
            }else if(msg.message == WM_KEYUP){
                sendBuf.push_back(msg.vkcode);
                sendBuf.push_back('1');
                scene_manager.OnInput(msg, false);
            }
        }
        if(!sendBuf.empty()){
            client.sendMsg(sendBuf);
            std::cout << "Client sendBuf: " << sendBuf << std::endl;
            sendBuf.clear();
        }
    }
}

void HandleRecv() {
    ExMessage msg;
    while(true) {
        client.recvMsg(recvBuf);
        if(!recvBuf.empty()){
            int len = recvBuf.size();
            for(int i = 0; i + 1 < len; i += 2){
                if(recvBuf[i+1] == '1'){
                    msg.message = WM_KEYUP;
                }else{
                    msg.message = WM_KEYDOWN;
                }
                msg.vkcode = recvBuf[i];
                scene_manager.OnInput(msg, true);
            }
            std::cout << "Client recvBuf: " << recvBuf << std::endl;
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