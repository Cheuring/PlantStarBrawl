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
    AnimationWidget(PlayerType player_type, int x, int y);
    ~AnimationWidget() = default;

    void OnUpdate(int delta);
    void OnDraw(const Camera& camera) const;
    void SetAnimation(const PlayerType& player_type);

private:
    Animation animation_current;
    int x, y;
};

#endif // _ANIMATION_WIDGET_H_