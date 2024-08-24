#ifndef _SUN_BULLET_H_
#define _SUN_BULLET_H_

#include "Animation.h"
#include "Bullet.h"
#include "Camera.h"
#include "Vector2.h"

extern Atlas atlas_sun;
extern Atlas atlas_sun_explode;

extern Camera main_camera;

class SunBullet : public Bullet {
public:
    SunBullet();
    ~SunBullet() = default;

    void OnCollide() override;
    void OnUpdate(int delta) override;
    void OnDraw(const Camera& camera) const override;

private:
    const float gravity = 1e-3f;

    Animation animation_idle;
    Animation animation_explode;
    Vector2 explode_render_offset;
};

#endif // _SUN_BULLET_H_