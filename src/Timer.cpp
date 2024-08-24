#include "Timer.h"

void Timer::Restart(){
    passed_time = 0;
    shotted = false;
}

void Timer::SetWaitTime(int time){
    wait_time = time;
}

void Timer::SetOneShot(bool one_shot){
    this->one_shot = one_shot;
}

void Timer::SetCallback(std::function<void()> callback){
    this->callback = callback;
}

void Timer::Pause(){
    is_paused = true;
}

void Timer::Resume(){
    is_paused = false;
}

void Timer::OnUpdate(int delta){
    if(is_paused){
        return;
    }

    passed_time += delta;
    if(passed_time >= wait_time){
        if((!one_shot || (one_shot && !shotted)) && callback){
            shotted = true;
            passed_time = 0;
            callback();
        }else{
            shotted = true;
            passed_time = 0;
        }
    }
}

