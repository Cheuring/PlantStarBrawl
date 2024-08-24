#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "Timer.h"
#include "Vector2.h"

class Camera {
public:
    Camera();
    ~Camera() = default;

    auto GetPosition() const -> const Vector2&;
    void Reset();
    void OnUpdate(int delta);
    void Shake(float strength, int duration);

private:
    Vector2 position;
    Timer timer_shake;
    bool is_shaking = false;
    float shaking_strength = 0;
};



#endif // _CAMERA_H_