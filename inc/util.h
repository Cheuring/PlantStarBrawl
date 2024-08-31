#ifndef _UTIL_H_
#define _UTIL_H_

#include <graphics.h>
#include <stdexcept>

#include "Camera.h"
#include "Vector2.h"

constexpr int FPS = 45;
constexpr int LOGICAL_FPS = 20;

inline void FlipImage(IMAGE* src, IMAGE* dest) {
    if(src == nullptr || dest == nullptr){
        throw std::runtime_error("FlipImage: src or dest is nullptr");
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

inline void PutImageAlpha(int dst_x, int dst_y, IMAGE* img) {
    int w = img->getwidth();
    int h = img->getheight();
    AlphaBlend(GetImageHDC(GetWorkingImage()), dst_x, dst_y, w, h,
        GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA});
}

inline void PutImageAlpha(int dst_x, int dst_y, int width, int height, IMAGE* img, int src_x, int src_y) {
    int w = width > 0 ? width : img->getwidth();
    int h = height > 0 ? height : img->getheight();
    AlphaBlend(GetImageHDC(GetWorkingImage()), dst_x, dst_y, w, h,
        GetImageHDC(img), src_x, src_y, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA});
}

inline void PutImageAlpha(const Camera& camera, int dst_x, int dst_y, IMAGE* img) {
    int w = img->getwidth();
    int h = img->getheight();
    const auto& camera_pos = camera.GetPosition();
    AlphaBlend(GetImageHDC(GetWorkingImage()), (int)(dst_x - camera_pos.x), (int)(dst_y - camera_pos.y),
    w, h, GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA});
}

inline void Line(const Camera& camera, int x1, int y1, int x2, int y2){
    const auto& camera_pos = camera.GetPosition();
    ::line((int)(x1 - camera_pos.x), (int)(y1 - camera_pos.y), (int)(x2 - camera_pos.x), (int)(y2 - camera_pos.y));
}

inline void SketchImage(IMAGE* src, IMAGE* dest) {
    int w = src->getwidth();
    int h = src->getheight();
    Resize(dest, w, h);
    DWORD* src_buf = GetImageBuffer(src);
    DWORD* dest_buf = GetImageBuffer(dest);
    for(int y = 0; y < h; y++){
        for(int x = 0; x < w; x++){
            int idx = y * w + x;
            dest_buf[idx] = BGR(RGB(255, 255, 255)) | (src_buf[idx] & 0xFF000000);
        }
    }
}

inline void RedToBlue(IMAGE* src, IMAGE* dest) {
    int w = src->getwidth();
    int h = src->getheight();
    Resize(dest, w, h);
    DWORD* src_buf = GetImageBuffer(src);
    DWORD* dest_buf = GetImageBuffer(dest);
    for(int y = 0; y < h; ++y){
        for(int x = 0; x < w; ++x){
            int idx = y * w + x;
            int r = GetRValue(src_buf[idx]);
            int g = GetGValue(src_buf[idx]);
            int b = GetBValue(src_buf[idx]);
            dest_buf[idx] = RGB(b, g, r) | (src_buf[idx] & 0xFF000000);
        }
    }
}

#endif // _UTIL_H_