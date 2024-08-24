#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include <graphics.h>

#include "Camera.h"
#include "util.h"

extern bool is_debug;

class Platform{
public:
    struct CollisionShape {
        float y;
        float left, right;
    };

public:
    CollisionShape shape;
    IMAGE* img = nullptr;
    POINT render_position{0, 0};

public:
    Platform() = default;
    ~Platform() = default;

    void OnDraw(const Camera& camera) const;
};

#endif // _PLATFORM_H_