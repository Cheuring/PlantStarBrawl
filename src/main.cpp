#include <graphics.h>

#include "Scene.h"
#include "MenuScene.h"

int main(){
    ExMessage msg;
    const int FPS = 60;

    initgraph(1280, 720, EW_SHOWCONSOLE);

    BeginBatchDraw();

    Scene* scene = new MenuScene();
    scene->on_enter();

    while(true){
        DWORD frame_start_time = GetTickCount();

        while(peekmessage(&msg)){
            scene->on_input(msg);
        }

        scene->on_update();

        cleardevice();
        scene->on_draw();
        FlushBatchDraw();

        DWORD frame_end_time = GetTickCount();
        DWORD frame_duration = frame_end_time - frame_start_time;
        if(frame_duration < 1000 / FPS){
            Sleep(1000 / FPS - frame_duration);
        }
    }

    scene->on_exit();
    delete scene;

    EndBatchDraw();
    closegraph();
    
}