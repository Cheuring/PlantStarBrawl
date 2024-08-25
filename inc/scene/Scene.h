#ifndef _SCENE_H_
#define _SCENE_H_

#include <graphics.h>

#include "Camera.h"

class Scene
{
public:
    Scene() = default;
    ~Scene() = default;

    virtual void OnEnter() = 0;
    virtual void OnUpdate(int delta) = 0;
    virtual void OnDraw(const Camera& camera) = 0;
    virtual void OnInput(const ExMessage& msg, bool is_1P) = 0;
    virtual void OnInput(const ExMessage& msg) = 0;
    virtual void OnExit() = 0;

private:

};

#endif // _SCENE_H_