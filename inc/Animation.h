#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <functional>
#include <graphics.h>

#include "Atlas.h"
#include "Camera.h"
#include "util.h"

class Animation
{
public:
    Animation() = default;
    ~Animation() = default;

    void Reset();
    void SetAtlas(Atlas* atlas);
    void SetLoop(bool is_loop);
    void SetInterval(int interval);
    auto GetFrameIndex() const -> int;
    auto GetCurrentFrame() const -> IMAGE* ;
    auto CheckFinish() const -> bool ;
    void OnUpdate(int delta);
    void OnDraw(const Camera& camera, int x, int y) const ;
    void SetOnFinish(std::function<void()> on_finish);

private:
    int timer = 0;
    int interval = 0;
    int frame_index = 0;
    bool is_loop = true;
    Atlas* atlas = nullptr;
    std::function<void()> on_finish = nullptr;
};



#endif // _ANIMATION_H_