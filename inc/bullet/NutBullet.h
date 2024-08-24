#ifndef _NUT_BULLET_H_
#define _NUT_BULLET_H_

#include "Animation.h"
#include "Bullet.h"
#include "Camera.h"
#include "Vector2.h"

extern Atlas atlas_nut_explode;

class NutBullet : public Bullet {
public:
    NutBullet();
    ~NutBullet() = default;

    void OnUpdate(int delta) override;
    void OnDraw(const Camera& camera) const override;
    
private:
    Animation animation_explode;
    Timer timer_valid;
};


#endif // _NUT_BULLET_H_