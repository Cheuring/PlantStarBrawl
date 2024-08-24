#ifndef _PEA_BULLET_H_
#define _PEA_BULLET_H_

#include "Animation.h"
#include "Bullet.h"

extern IMAGE img_pea;
extern Atlas atlas_pea_break;

class PeaBullet : public Bullet {
public:
    PeaBullet();
    ~PeaBullet() = default;

    void OnCollide() override;
    void OnUpdate(int delta) override;
    void OnDraw(const Camera& camera) const override;

private:
    Animation animation_break;

};

#endif // _PEA_BULLET_H_