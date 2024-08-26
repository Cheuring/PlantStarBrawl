#include "MySocket.h"
#include <graphics.h>
#include <thread>
#include <windows.h>
#include <vector>
#include <iostream>

#include "AnimationWidget.h"
#include "BuffBullet.h"
#include "Bullet.h"
#include "Camera.h"
#include "GameScene.h"
#include "GameType.h"
#include "MediaSource.h"
#include "MenuScene.h"
#include "Platform.h"
#include "Player.h"
#include "Scene.h"
#include "SelectorScene.h"
#include "SceneManager.h"
#include "util.h"

bool is_debug = false;
bool is_connected = false;

Scene* menu_scene = nullptr;
Scene* game_scene = nullptr;
Scene* selector_scene = nullptr;

Camera main_camera;
SceneManager scene_manager(GameType::SERVER);

std::vector<Bullet *> bullet_list;
std::vector<BuffBullet *> buff_bullet_list;
std::vector<Platform> platform_list;

Player* player_1 = nullptr;
Player* player_2 = nullptr;

IMAGE* img_player_1_avatar = nullptr;
IMAGE* img_player_2_avatar = nullptr;

MySocket server;
std::string sendBuf("#");
std::string recvBuf;

AnimationWidget widget_sunflower(PlayerType::Peashooter, 1080, 520);

inline void HandleInput(std::string &buf, bool is_server) {
    ExMessage msg;
    if(buf.size() > 1){
        int len = buf.size();
        for(int i = 1; i + 1 < len; i += 2){
            if(buf[i+1] == '1'){
                msg.message = WM_KEYUP;
            }else{
                msg.message = WM_KEYDOWN;
            }
            msg.vkcode = buf[i];
            scene_manager.OnInput(msg, is_server);
        }
        // std::cout << "Client recvBuf: " << recvBuf << std::endl;
        buf.clear();
        buf.push_back('#');
    }
}

inline void GameCircle() {
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

        server.sendMsg(sendBuf);
        HandleInput(sendBuf, true);
        server.recvMsg(recvBuf);
        HandleInput(recvBuf, false);
    }
}

void LocalInput() {
    ExMessage msg;
    while(true) {
        while(peekmessage(&msg)){
            if(msg.message == WM_KEYDOWN){
                sendBuf.push_back(msg.vkcode);
                sendBuf.push_back('0');
                // scene_manager.OnInput(msg, true);
            }else if(msg.message == WM_KEYUP){
                sendBuf.push_back(msg.vkcode);
                sendBuf.push_back('1');
                // scene_manager.OnInput(msg, true);
            }
        }
    }
}

void WidgetUpdate(){
    while(!is_connected){
        DWORD frame_start_time = GetTickCount();

        static DWORD last_tick_time = GetTickCount();
        DWORD current_tick_time = GetTickCount();
        DWORD delta = current_tick_time - last_tick_time;

        widget_sunflower.OnUpdate(delta);
        last_tick_time = current_tick_time;

        cleardevice();
        outtextxy(200, 200, ("server ip:  " + server.GetLocalIP()).c_str());
        outtextxy(200, 300, "waiting for connection...");
        widget_sunflower.OnDraw(main_camera);
        FlushBatchDraw();

        DWORD frame_end_time = GetTickCount();
        DWORD frame_duration = frame_end_time - frame_start_time;
        if(frame_duration < 1000 / FPS){
            Sleep(1000 / FPS - frame_duration);
        }
    }
}

int main(){
    LoadGameResources();

    initgraph(1280, 720);

    settextstyle(28, 0, _T("zpix"));
    setbkmode(TRANSPARENT);

    setbkcolor(0xeeeeee);
	cleardevice();
	settextcolor(BLACK);
    BeginBatchDraw();

    outtextxy(200, 200, ("server ip:  " + server.GetLocalIP()).c_str());
    outtextxy(200, 300, "waiting for connection...");

    std::thread thread_widget(WidgetUpdate);
    thread_widget.detach();
    server.Accept();

    is_connected = true;

    menu_scene = new MenuScene();
    selector_scene = new SelectorScene();

    scene_manager.SetCurrentScene(menu_scene);

    std::thread thread_send(LocalInput);
    thread_send.detach();

    GameCircle();

    EndBatchDraw();
    closegraph();
}