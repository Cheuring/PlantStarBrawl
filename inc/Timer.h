#ifndef _TIMER_H_
#define _TIMER_H_

#include <functional>

class Timer {
public:
    Timer() = default;
    ~Timer() = default;

    void restart(){
        passed_time = 0;
        shotted = false;
    }

    void set_wait_time(int time){
        wait_time = time;
    }

    void set_one_shot(bool one_shot){
        this->one_shot = one_shot;
    }

    void set_callback(std::function<void()> callback){
        this->callback = callback;
    }

    void pause(){
        is_paused = true;
    }

    void resume(){
        is_paused = false;
    }

    void on_update(int delta){
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

private:
    int passed_time = 0;            //  已过时间
    int wait_time = 0;              //  等待时间
    bool is_paused = false;         //  是否暂停
    bool shotted = false;           //  是否已触发
    bool one_shot = false;          //  是否只触发一次
    std::function<void()> callback; //  回调函数

};

#endif // _TIMER_H_