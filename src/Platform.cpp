#include "Platform.h"

void Platform::OnDraw(const Camera& camera) const {
    PutImageAlpha(camera, render_position.x, render_position.y, img);

    if(is_debug){
        setfillcolor(RGB(0, 0, 255));
        solidcircle(render_position.x, render_position.y, 3);
        setlinecolor(RGB(255, 0, 0));
        Line(camera, shape.left, shape.y, shape.right, shape.y);
    }
}