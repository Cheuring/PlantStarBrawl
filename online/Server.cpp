#include "MySocket.h"
#include <assert.h>
#include <graphics.h>
#include <thread>
#include <windows.h>
#include <vector>
#include <iostream>
#include <mutex>
#include <random>

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

std::mutex mutex;
std::mt19937* engine = nullptr;

AnimationWidget widget_peashooter(PlayerType::Peashooter, 1080, 520);

void HandleInput(std::string &buf, bool is_server) {
    // if(is_server){
    //     std::cout << "send: " << buf << "\n";
    // }else{
    //     std::cout << "recv: " << buf << "\n";
    // }
    ExMessage msg;
    if(buf.size() > 1){
        int i = 1;
        if(is_server){
            try{
                while(buf.at(i) != '#') ++i;
                ++i;
            }catch (const std::out_of_range& e) {
                std::cout << "send: " << buf << "\n";
                std::cerr << "HandleInput delta out of range: " << e.what() << std::endl;
            }
            
            if(player_1 != nullptr){
                ++i;  //  skip *
                try {
                    while(buf.at(i) != '*') ++i;
                    ++i;
                    while(buf.at(i) != '*') ++i;
                    ++i;
                    while(buf.at(i) != '*') ++i;
                    ++i;
                    while(buf.at(i) != '*') ++i;
                    ++i;

                } catch (const std::out_of_range& e) {
                    std::cout << "send: " << buf << "\n";
                    std::cerr << "HandleInput position out of range: " << e.what() << std::endl;
                }
            }else if(i < buf.size() && buf[i] == '*') {
                buf.clear();
                return;
            }
        }

        int len = buf.size();
        for(; i + 1 < len; i += 2){
            switch(buf[i]){
                case '1':
                    msg.message = WM_KEYUP;
                    msg.vkcode = buf[i+1];
                    scene_manager.OnInput(msg, is_server);
                    break;
                case '0':
                    msg.message = WM_KEYDOWN;
                    msg.vkcode = buf[i+1];
                    scene_manager.OnInput(msg, is_server);
                    break;
                default:
                    std::cout << buf[i] << "\n";
                    break;
            }
        }
        // std::cout << "Client recvBuf: " << recvBuf << std::endl;
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

        mutex.lock();
        std::string tmp = std::move(sendBuf);
        sendBuf.clear();
        sendBuf.push_back('#');
        sendBuf += std::to_string(delta) + "#";

        if(player_1 != nullptr){
            const auto& position_player_1 = player_1->GetPosition();
            sendBuf += "*" + std::to_string(position_player_1.x) + "*" + std::to_string(position_player_1.y) + "*";
            const auto& position_player_2 = player_2->GetPosition();
            sendBuf += std::to_string(position_player_2.x) + "*" + std::to_string(position_player_2.y) + "*";
        }
        mutex.unlock();

        server.sendMsg(tmp);
        HandleInput(tmp, true);

        server.recvMsg(recvBuf);
        HandleInput(recvBuf, false);

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
        mutex.lock();
        while(peekmessage(&msg, WH_KEYBOARD)){
            if(msg.message == WM_KEYDOWN){
                sendBuf.push_back('0');
                sendBuf.push_back(msg.vkcode);
                // scene_manager.OnInput(msg, true);
            }else if(msg.message == WM_KEYUP){
                sendBuf.push_back('1');
                sendBuf.push_back(msg.vkcode);
                // scene_manager.OnInput(msg, true);
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

        widget_peashooter.OnUpdate(delta);
        last_tick_time = current_tick_time;

        cleardevice();
        outtextxy(200, 200, ("server ip:  " + server.GetLocalIP()).c_str());
        outtextxy(200, 300, "waiting for connection...");
        widget_peashooter.OnDraw(main_camera);
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

    initgraph(1280, 720, EX_SHOWCONSOLE);

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

    unsigned int seed = std::random_device{}();
    engine = new std::mt19937{seed};
    sendBuf += std::to_string(seed) + "#";
    server.sendMsg(sendBuf);
    sendBuf = "#";

    menu_scene = new MenuScene();
    selector_scene = new SelectorScene();

    scene_manager.SetCurrentScene(menu_scene);

    std::thread thread_local_input(LocalInput);
    thread_local_input.detach();

    GameCircle();

    EndBatchDraw();
    closegraph();
}