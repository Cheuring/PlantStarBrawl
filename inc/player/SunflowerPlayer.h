#ifndef _SUNFLOWER_PLAYER_H_
#define _SUNFLOWER_PLAYER_H_

#include "Atlas.h"
#include "Player.h"
#include "SunBullet.h"
#include "SunBulletEx.h"
#include "Vector2.h"

extern Atlas atlas_sunflower_idle_left;
extern Atlas atlas_sunflower_idle_right;
extern Atlas atlas_sunflower_run_left;
extern Atlas atlas_sunflower_run_right;
extern Atlas atlas_sunflower_attack_ex_left;
extern Atlas atlas_sunflower_attack_ex_right;
extern Atlas atlas_sun_text;
extern Atlas atlas_sunflower_die_left;
extern Atlas atlas_sunflower_die_right;

extern Player* player_1;
extern Player* player_2;

class SunflowerPlayer : public Player {
public:
    SunflowerPlayer();
    ~SunflowerPlayer() = default;

    void OnUpdate(int delta) override;
    void OnDraw(const Camera& camera) override;
    void OnAttack() override;
    void OnAttackEx() override;


private:
    const float speed_sun_ex = 0.30f;
    const Vector2<float> velocity_sun = {0.25f, -0.5f};

    Animation animation_sun_text;
    bool is_sun_text_visible = false;

};

#endif // _SUNFLOWER_PLAYER_H_