#include "MySocket.h"
#include <graphics.h>
#include <thread>
#include <windows.h>
#include <vector>
#include <iostream>
#include <mutex>

#include "AnimationWidget.h"
#include "BuffBullet.h"
#include "Bullet.h"
#include "Camera.h"
#include "EasyWidget.h"
#include "GameScene.h"
#include "GameType.h"
#include "MediaSource.h"
#include "MenuScene.h"
#include "Platform.h"
#include "Player.h"
#include "PlayerType.h"
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
SceneManager scene_manager(GameType::CLIENT);

std::vector<Bullet *> bullet_list;
std::vector<BuffBullet *> buff_bullet_list;
std::vector<Platform> platform_list;

Player* player_1 = nullptr;
Player* player_2 = nullptr;

IMAGE* img_player_1_avatar = nullptr;
IMAGE* img_player_2_avatar = nullptr;

std::string sendBuf("#");
std::string recvBuf;
MySocket client;

std::mutex mutex;

EasyTextBox text_ip;
EasyButton btn_connect;

AnimationWidget widget_sunflower(PlayerType::Sunflower, 1080, 520);

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
        buf.clear();
        // buf.push_back('#');
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

        client.recvMsg(recvBuf);
        HandleInput(recvBuf, true);

        mutex.lock();
        std::string tmp = std::move(sendBuf);
        sendBuf.clear();
        sendBuf.push_back('#');
        mutex.unlock();

        client.sendMsg(tmp);
        HandleInput(tmp, false);
    }
}

void LocalInput(MySocket& client) {
    ExMessage msg;
    while(true) {
        mutex.lock();
        while(peekmessage(&msg, WH_KEYBOARD)){
            if(msg.message == WM_KEYDOWN){
                sendBuf.push_back(msg.vkcode);
                sendBuf.push_back('0');
                // scene_manager.OnInput(msg, false);
            }else if(msg.message == WM_KEYUP){
                sendBuf.push_back(msg.vkcode);
                sendBuf.push_back('1');
                // scene_manager.OnInput(msg, false);
            }
        }
        mutex.unlock();

        Sleep(10);
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
        outtextxy(220, 210, "Server IP:");
        text_ip.Show();
        btn_connect.Show();
        widget_sunflower.OnDraw(main_camera);
        FlushBatchDraw();

        DWORD frame_end_time = GetTickCount();
        DWORD frame_duration = frame_end_time - frame_start_time;
        if(frame_duration < 1000 / FPS){
            Sleep(1000 / FPS - frame_duration);
        }
    }
}

void OnClick() {
    std::string error_msg;
    if(!client.Connect(text_ip.Text(), error_msg)){
        error_msg += "\nPlease try again.";
        MessageBox(GetHWnd(), error_msg.c_str(), "Error", MB_OK);
    }else{
        is_connected = true;
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

    outtextxy(220, 210, "Server IP:");
    text_ip.Create(400, 200, 800, 250, 20);
    btn_connect.Create(600, 300, 800, 350, "Connect", OnClick);

    std::thread thread_widget(WidgetUpdate);
    thread_widget.detach();

    ExMessage temp_msg;
    while(!is_connected) {
        temp_msg = getmessage(EX_MOUSE);
        if(temp_msg.message == WM_LBUTTONDOWN){
            if(text_ip.Check(temp_msg.x, temp_msg.y)) text_ip.OnMessage();

            if(btn_connect.Check(temp_msg.x, temp_msg.y)) btn_connect.OnMessage();
            
        }
    }



    menu_scene = new MenuScene();
    selector_scene = new SelectorScene();

    scene_manager.SetCurrentScene(menu_scene);

    std::thread thread_send(LocalInput, std::ref(client));
    thread_send.detach();

    GameCircle();

    EndBatchDraw();
    closegraph();
}