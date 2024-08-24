#ifndef _TIMER_H_
#define _TIMER_H_

#include <functional>

class Timer {
public:
    Timer() = default;
    ~Timer() = default;

    void Restart();
    void SetWaitTime(int time);
    void SetOneShot(bool one_shot);
    void SetCallback(std::function<void()> callback);
    void Pause();
    void Resume();
    void OnUpdate(int delta);

private:
    int passed_time = 0;            //  已过时间
    int wait_time = 0;              //  等待时间
    bool is_paused = false;         //  是否暂停
    bool shotted = false;           //  是否已触发
    bool one_shot = false;          //  是否只触发一次
    std::function<void()> callback; //  回调函数

};

#endif // _TIMER_H_