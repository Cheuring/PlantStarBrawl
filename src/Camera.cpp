#include "Camera.h"

Camera::Camera(){
    timer_shake.SetOneShot(true);
    timer_shake.SetCallback([&](){
        is_shaking = false;
        Reset();
    });
}

auto Camera::GetPosition() const -> const Vector2<int>&{
    return position;
}

void Camera::Reset(){
    position.x = 0;
    position.y = 0;
}

void Camera::OnUpdate(int delta){
    timer_shake.OnUpdate(delta);

    if(is_shaking){
        position.x = dist(engine_) / 50.0f * shaking_strength;
        position.y = dist(engine_) / 50.0f * shaking_strength;
    }
}

void Camera::Shake(int strength, int duration){
    is_shaking = true;
    shaking_strength = strength;

    timer_shake.SetWaitTime(duration);
    timer_shake.Restart();
}