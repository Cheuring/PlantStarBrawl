#ifndef _SCENE_H_
#define _SCENE_H_

#include <graphics.h>

#include "Camera.h"

class Scene
{
public:
    Scene() = default;
    ~Scene() = default;

    virtual void on_enter() {};
    virtual void on_update(int delta) {};
    virtual void on_draw(const Camera& camera) {};
    virtual void on_input(const ExMessage& msg) {};
    virtual void on_exit() {};

private:

};

#endif // _SCENE_H_