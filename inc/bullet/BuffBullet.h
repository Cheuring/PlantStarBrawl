#ifndef _BUFF_BULLET_H_
#define _BUFF_BULLET_H_

#include "Animation.h"
#include "Bullet.h"
#include "BuffId.h"

extern Atlas atlas_buffbox_blue;
extern Atlas atlas_buffbox_pink;
extern Atlas atlas_buffbox_yellow;

class BuffBullet : public Bullet {
public:
    BuffBullet(int buffNum);
    ~BuffBullet() = default;

    auto GetBuffId() const -> BuffId;
    void OnCollide() override;
    void OnUpdate(int delta) override;
    void OnDraw(const Camera& camera) const override;

private:
    BuffId buff_id = BuffId::INVALID;
    Animation animation_buff;

private:
    inline void setAtlas(Atlas* atlas){
        animation_buff.SetAtlas(atlas);
        animation_buff.SetInterval(100);
        animation_buff.SetLoop(true);
    }

};

#endif // _BUFF_BULLET_H_