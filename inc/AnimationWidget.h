#ifndef _ANIMATION_WIDGET_H_
#define _ANIMATION_WIDGET_H_

#include "Atlas.h"
#include "Animation.h"
#include "Camera.h"
#include "PlayerType.h"

extern Atlas atlas_sunflower_idle_right;
extern Atlas atlas_peashooter_idle_right;
extern Atlas atlas_gloomshroom_idle_right;
extern Atlas atlas_nut_idle_right;

class AnimationWidget {
public:
    AnimationWidget(PlayerType player_type, int x, int y) {
        switch (player_type) {
        case PlayerType::Sunflower:
            animation_current.SetAtlas(&atlas_sunflower_idle_right);
            break;
        case PlayerType::Peashooter:
            animation_current.SetAtlas(&atlas_peashooter_idle_right);
            break;
        case PlayerType::Gloomshroom:
            animation_current.SetAtlas(&atlas_gloomshroom_idle_right);
            break;
        case PlayerType::Nut:
            animation_current.SetAtlas(&atlas_nut_idle_right);
            break;
        default:
            throw std::invalid_argument("Invalid player type");
        }
        this->x = x;
        this->y = y;

        animation_current.SetLoop(true);
        animation_current.SetInterval(100);
    }
    ~AnimationWidget() = default;

    void OnUpdate(int delta) {
        animation_current.OnUpdate(delta);
    }

    void OnDraw(const Camera& camera) const {
        animation_current.OnDraw(camera, x, y);
    }

private:
    Animation animation_current;
    int x, y;
};

#endif // _ANIMATION_WIDGET_H_