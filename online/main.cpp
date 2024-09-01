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
bool is_start = false;

Scene* menu_scene = nullptr;
Scene* game_scene = nullptr;
Scene* selector_scene = nullptr;

Camera main_camera;
SceneManager scene_manager(GameType::LOCAL);


std::vector<Bullet *> bullet_list;
std::vector<BuffBullet *> buff_bullet_list;
std::vector<Platform> platform_list;

Player* player_1 = nullptr;
Player* player_2 = nullptr;

IMAGE* img_player_1_avatar = nullptr;
IMAGE* img_player_2_avatar = nullptr;

std::string sendBuf("#");
std::string recvBuf;
MySocket mysocket;

std::mutex mutex;
std::mt19937* engine = nullptr;

EasyTextBox text_ip;
EasyButton btn_connect;

EasyButton btn_server;
EasyButton btn_client;
EasyButton btn_local;

AnimationWidget widget_sunflower(PlayerType::Sunflower, 1080, 520);
AnimationWidget widget_peashooter(PlayerType::Peashooter, 1080, 100);
AnimationWidget widget_gloomshroom(PlayerType::Gloomshroom, 100, 520);
AnimationWidget widget_nut(PlayerType::Nut, 100, 100);

void HandleInput_client(std::string &buf, bool is_server) {
    ExMessage msg;
    if(buf.size() > 1){
        int i = 1;

        if(is_server){
            int delta = 0;
            try{
                while(isdigit(buf.at(i))){
                    delta = delta * 10 + (buf[i] - '0');
                    ++i;
                }
                assert(buf[i] == '#');
                ++i;
            }catch (const std::out_of_range& e) {
                std::cout << "recv: " << buf << "\n";
                std::cerr << "HandleInput delta out of range: " << e.what() << std::endl;
            }

            if(player_1 != nullptr) {
                ++i;  //  skip *
                try{
                    int x = 0, y = 0;
                    bool is_neg = false;
                    if(buf.at(i) == '-'){
                        is_neg = true;
                        ++i;
                    }

                    while(isdigit(buf.at(i))){
                        x = x * 10 + (buf[i] - '0');
                        ++i;
                    }
                    x = is_neg ? -x : x;
                    assert(buf[i] == '*');
                    ++i;

                    is_neg = false;
                    if(buf.at(i) == '-'){
                        is_neg = true;
                        ++i;
                    }
                    while(isdigit(buf.at(i))){
                        y = y * 10 + (buf[i] - '0');
                        ++i;
                    }
                    y = is_neg ? -y : y;
                    assert(buf[i] == '*');

                    ++i;
                    player_1->SetPosition(x, y);

                    x = 0, y = 0;
                    is_neg = false;
                    if(buf.at(i) == '-'){
                        is_neg = true;
                        ++i;
                    }

                    while(isdigit(buf.at(i))){
                        x = x * 10 + (buf[i] - '0');
                        ++i;
                    }
                    x = is_neg ? -x : x;
                    assert(buf[i] == '*');
                    ++i;

                    is_neg = false;
                    if(buf.at(i) == '-'){
                        is_neg = true;
                        ++i;
                    }
                    while(isdigit(buf.at(i))){
                        y = y * 10 + (buf[i] - '0');
                        ++i;
                    }
                    y = is_neg ? -y : y;
                    assert(buf[i] == '*');

                    ++i;
                    player_2->SetPosition(x, y);

                } catch (const std::out_of_range& e) {
                    std::cout << "recv: " << buf << "\n";
                    std::cerr << "HandleInput position out of range: " << e.what() << std::endl;
                }
            }else if(i < buf.size() && buf[i] == '*') {
                buf.clear();
                return;
            }
            scene_manager.OnUpdate(delta);
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
        buf.clear();
    }
}

void HandleInput_server(std::string &buf, bool is_server) {
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
        buf.clear();
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
                // scene_manager.OnInput(msg, false);
            }else if(msg.message == WM_KEYUP){
                sendBuf.push_back('1');
                sendBuf.push_back(msg.vkcode);
                // scene_manager.OnInput(msg, false);
            }
        }
        mutex.unlock();

        Sleep(10);
    }
}

void WidgetUpdate_client(){
    while(!is_connected){
        DWORD frame_start_time = GetTickCount();

        static DWORD last_tick_time = GetTickCount();
        DWORD current_tick_time = GetTickCount();
        DWORD delta = current_tick_time - last_tick_time;

        widget_sunflower.OnUpdate(delta);
        widget_nut.OnUpdate(delta);
        last_tick_time = current_tick_time;

        cleardevice();
        outtextxy(220, 210, "Server IP:");
        text_ip.Show();
        btn_connect.Show();
        widget_sunflower.OnDraw(main_camera);
        widget_nut.OnDraw(main_camera);
        FlushBatchDraw();

        DWORD frame_end_time = GetTickCount();
        DWORD frame_duration = frame_end_time - frame_start_time;
        if(frame_duration < 1000 / FPS){
            Sleep(1000 / FPS - frame_duration);
        }
    }
}

void WidgetUpdate(){
    while(!is_start){
        DWORD frame_start_time = GetTickCount();

        static DWORD last_tick_time = GetTickCount();
        DWORD current_tick_time = GetTickCount();
        DWORD delta = current_tick_time - last_tick_time;

        widget_sunflower.OnUpdate(delta);
        widget_peashooter.OnUpdate(delta);
        widget_gloomshroom.OnUpdate(delta);
        widget_nut.OnUpdate(delta);
        last_tick_time = current_tick_time;

        cleardevice();
        btn_local.Show();
        btn_server.Show();
        btn_client.Show();
        widget_sunflower.OnDraw(main_camera);
        widget_peashooter.OnDraw(main_camera);
        widget_gloomshroom.OnDraw(main_camera);
        widget_nut.OnDraw(main_camera);
        FlushBatchDraw();

        DWORD frame_end_time = GetTickCount();
        DWORD frame_duration = frame_end_time - frame_start_time;
        if(frame_duration < 1000 / FPS){
            Sleep(1000 / FPS - frame_duration);
        }
    }
}

void WidgetUpdate_server(){
    while(!is_connected){
        DWORD frame_start_time = GetTickCount();

        static DWORD last_tick_time = GetTickCount();
        DWORD current_tick_time = GetTickCount();
        DWORD delta = current_tick_time - last_tick_time;

        widget_peashooter.OnUpdate(delta);
        widget_gloomshroom.OnUpdate(delta);
        last_tick_time = current_tick_time;

        cleardevice();
        outtextxy(200, 200, ("server ip:  " + mysocket.GetLocalIP()).c_str());
        outtextxy(200, 300, "waiting for connection...");
        widget_peashooter.OnDraw(main_camera);
        widget_gloomshroom.OnDraw(main_camera);
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
    if(!mysocket.Connect(text_ip.Text(), error_msg)){
        error_msg += "\nPlease try again.";
        MessageBox(GetHWnd(), error_msg.c_str(), "Error", MB_OK);
    }else{
        is_connected = true;
    }
}

inline void ParseSeed(std::string& buf){
    assert(buf.size() > 1);
    try {
        unsigned int seed = 0;
        int i = 1;
        while(buf.at(i) != '#'){
            seed = seed * 10 + (buf[i] - '0');
            ++i;
        }
        
        engine = new std::mt19937{seed};
        buf.clear();
    } catch (const std::out_of_range& e) {
        std::cerr << "ParseSeed: " << e.what() << std::endl;
    }
}

inline void GameCircle_client() {
    outtextxy(220, 210, "Server IP:");
    text_ip.Create(400, 200, 800, 250, 20);
    btn_connect.Create(600, 300, 800, 350, "Connect", OnClick);

    scene_manager.SetGameType(GameType::CLIENT);

    std::thread thread_widget(WidgetUpdate_client);
    thread_widget.detach();

    ExMessage temp_msg;
    while(!is_connected) {
        temp_msg = getmessage(EX_MOUSE);
        if(temp_msg.message == WM_LBUTTONDOWN){
            if(text_ip.Check(temp_msg.x, temp_msg.y)) text_ip.OnMessage();

            if(btn_connect.Check(temp_msg.x, temp_msg.y)) btn_connect.OnMessage();
            
        }
    }

    mysocket.recvMsg(recvBuf);
    ParseSeed(recvBuf);

    menu_scene = new MenuScene();
    selector_scene = new SelectorScene();

    scene_manager.SetCurrentScene(menu_scene);

    std::thread thread_local_input(LocalInput);
    thread_local_input.detach();

    while(true) {
        mysocket.recvMsg(recvBuf);
        HandleInput_client(recvBuf, true);
        
        cleardevice();
        scene_manager.OnDraw(main_camera);
        FlushBatchDraw();

        mutex.lock();
        std::string tmp = std::move(sendBuf);
        sendBuf.clear();
        sendBuf.push_back('#');
        mutex.unlock();

        mysocket.sendMsg(tmp);
        HandleInput_client(tmp, false);
    }
}

inline void GameCircle_server() {
    outtextxy(200, 200, ("server ip:  " + mysocket.GetLocalIP()).c_str());
    outtextxy(200, 300, "waiting for connection...");

    scene_manager.SetGameType(GameType::SERVER);

    std::thread thread_widget(WidgetUpdate_server);
    thread_widget.detach();
    mysocket.Accept();

    is_connected = true;

    unsigned int seed = std::random_device{}();
    engine = new std::mt19937{seed};
    sendBuf += std::to_string(seed) + "#";
    mysocket.sendMsg(sendBuf);
    sendBuf = "#";

    menu_scene = new MenuScene();
    selector_scene = new SelectorScene();

    scene_manager.SetCurrentScene(menu_scene);

    std::thread thread_local_input(LocalInput);
    thread_local_input.detach();

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

        mysocket.sendMsg(tmp);
        HandleInput_server(tmp, true);

        mysocket.recvMsg(recvBuf);
        HandleInput_server(recvBuf, false);

        DWORD frame_end_time = GetTickCount();
        DWORD frame_duration = frame_end_time - frame_start_time;
        if(frame_duration < 1000 / FPS){
            Sleep(1000 / FPS - frame_duration);
        }
    }
}

inline void GameCircle_local() {
    ExMessage msg;
    engine = new std::mt19937{std::random_device{}()};

    menu_scene = new MenuScene();
    selector_scene = new SelectorScene();

    scene_manager.SetCurrentScene(menu_scene);

    while(true){
        DWORD frame_start_time = GetTickCount();

        while(peekmessage(&msg, WH_KEYBOARD)){
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

    mciSendString(_T("play bgm_menu repeat from 0"), NULL, 0, NULL);

    btn_local.Create(540, 200, 740, 250, "Local", nullptr);
    btn_server.Create(540, 300, 740, 350, "Server", nullptr);
    btn_client.Create(540, 400, 740, 450, "Client", nullptr);

    // FlushBatchDraw();
    std::thread thread_widget_start(WidgetUpdate);
    thread_widget_start.detach();

    // std::cout<< "start" << std::endl;

	ExMessage temp_msg;
	while (true)
	{
		temp_msg = getmessage(EX_MOUSE);			// 获取消息输入
 
		if (temp_msg.message == WM_LBUTTONDOWN)
		{
            if(btn_local.Check(temp_msg.x, temp_msg.y)){
                // btn_local.OnMessage();
                is_start = true;
                GameCircle_local();
                break;
            }
            if(btn_server.Check(temp_msg.x, temp_msg.y)){
                // btn_server.OnMessage();
                is_start = true;
                GameCircle_server();
                break;
            }
            if(btn_client.Check(temp_msg.x, temp_msg.y)){
                // btn_client.OnMessage();
                is_start = true;
                GameCircle_client();
                break;
            }
		}
	}

    EndBatchDraw();
    closegraph();

    delete engine;
    delete menu_scene;
    delete selector_scene;
}