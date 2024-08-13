#ifndef _UTIL_H_
#define _UTIL_H_

#include <graphics.h>

#include "Camera.h"

const int FPS = 60;

inline void flip_image(IMAGE* src, IMAGE* dest){
    if(src == nullptr || dest == nullptr){
        throw std::runtime_error("flip_image: src or dest is nullptr");
    }
    int w = src->getwidth();
    int h = src->getheight();
    Resize(dest, w, h);
    DWORD* src_buf = GetImageBuffer(src);
    DWORD* dest_buf = GetImageBuffer(dest);
    for(int y = 0; y < h; y++){
        for(int x = 0; x < w; x++){
            int idx_src = y * w + x;
            int idx_dest = y * w + (w - x - 1);
            dest_buf[idx_dest] = src_buf[idx_src];
        }
    }
}

inline void put_image_alpha(int dst_x, int dst_y, IMAGE* img){
    int w = img->getwidth();
    int h = img->getheight();
    AlphaBlend(GetImageHDC(GetWorkingImage()), dst_x, dst_y, w, h,
        GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA});
}
inline void put_image_alpha(int dst_x, int dst_y, int width, int height, IMAGE* img, int src_x, int src_y){
    int w = width > 0 ? width : img->getwidth();
    int h = height > 0 ? height : img->getheight();
    AlphaBlend(GetImageHDC(GetWorkingImage()), dst_x, dst_y, w, h,
        GetImageHDC(img), src_x, src_y, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA});
}
inline void put_image_alpha(const Camera& camera, int dst_x, int dst_y, IMAGE* img){
    int w = img->getwidth();
    int h = img->getheight();
    const Vector2& camera_pos = camera.get_position();
    AlphaBlend(GetImageHDC(GetWorkingImage()), (int)(dst_x - camera_pos.x), (int)(dst_y - camera_pos.y),
    w, h, GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA});
}

inline void line(const Camera& camera, int x1, int y1, int x2, int y2){
    const Vector2& camera_pos = camera.get_position();
    ::line((int)(x1 - camera_pos.x), (int)(y1 - camera_pos.y), (int)(x2 - camera_pos.x), (int)(y2 - camera_pos.y));
}

#endif // _UTIL_H_