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
    POINT render_positon{0, 0};

public:
    Platform() = default;
    ~Platform() = default;

    void on_draw(const Camera& camera) const{
        put_image_alpha(camera, render_positon.x, render_positon.y, img);

        if(is_debug){
            setlinecolor(RGB(255, 0, 0));
            line(camera, (int)shape.left, (int)shape.y, (int)shape.right, (int)shape.y);
        }
    }
};

#endif // _PLATFORM_H_