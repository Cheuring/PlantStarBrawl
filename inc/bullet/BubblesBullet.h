#ifndef _BUBBLES_BULLET_H_
#define _BUBBLES_BULLET_H_

#include <graphics.h>

#include "Animation.h"
#include "Bullet.h"
#include "Timer.h"
#include "Vector2.h"

extern Atlas atlas_bubbles;
extern Atlas atlas_bubbles_ex;

class BubblesBullet : public Bullet {
public:
    BubblesBullet(bool is_ex = false);
    ~BubblesBullet() = default;

    auto GetDamage() const -> int override;
    void SetPosition(const Vector2<int>& position) override;
    void OnCollide() override;
    auto CheckCollision(const Vector2<int>& position, const Vector2<int>& size) -> bool override;
    void OnUpdate(int delta) override;
    void OnDraw(const Camera& camera) const override;
    
private:
    int radius = 70;
    int sin45_r = 50;

    bool is_ex = false;
    int collision_count = 0;

    Animation animation;
    Timer timer_valid;

    Vector2<int> pos[8];
    Vector2<int> bullet_size = {25, 25};

private:
    auto check_collision_helper(const Vector2<int>& position_bullet, const Vector2<int>& position_target, const Vector2<int>& size) const -> bool;
};

#endif // _BUBBLES_BULLET_H_