#ifndef _SUN_BULLET_EX_H_
#define _SUN_BULLET_EX_H_

#include "Animation.h"
#include "Bullet.h"
#include "Camera.h"
#include "Vector2.h"

extern Atlas atlas_sun_ex;
extern Atlas atlas_sun_ex_explode;

extern Camera main_camera;

class SunBulletEx : public Bullet {
public:
    SunBulletEx();
    ~SunBulletEx() = default;

    void OnCollide() override;
    void OnUpdate(int delta) override;
    void OnDraw(const Camera& camera) const override;

private:
    Animation animation_idle;
    Animation animation_explode;
    Vector2 explode_render_offset;

};

#endif // _SUN_BULLET_EX_H_