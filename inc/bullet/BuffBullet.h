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
    BuffBullet(int buffNum){
        size.x = 80, size.y = 80;
        collision_offset.x = 14, collision_offset.y = 14;

        velocity.y = 0.1f;
        position.y = -size.y / 2;
        position.x = rand() % (getwidth() - (int)size.x);

        switch(buffNum){
            case 0:
                setAtlas(&atlas_buffbox_blue);
                buff_id = BuffId::RECOVER_MP;
                break;
            case 1:
                setAtlas(&atlas_buffbox_pink);
                buff_id = BuffId::RECOVER_HP;
                break;
            case 2:
                setAtlas(&atlas_buffbox_yellow);
                buff_id = BuffId::INVISIBLE;
                break;
            case 3:
                setAtlas(&atlas_buffbox_yellow);
                buff_id = BuffId::HURRY;
                break;
            default:
                throw std::runtime_error("Invalid buff number");
                break;
        }
    }
    ~BuffBullet() = default;

    BuffId getBuffId() const {
        return buff_id;
    }

    void on_collide() override {
        can_remove = true;

        mciSendString(_T("play ui_confirm from 0"), NULL, 0, NULL);
    }

    void on_update(int delta) override {
        if(valid){
            position += velocity * (float)delta;
            animation_buff.on_update(delta);

            if(check_if_exceeds_screen()){
                can_remove = true;
            }
        }
    }

    void on_draw(const Camera& camera) const override{
        if(valid){
            animation_buff.on_draw(camera, position.x, position.y);
        }

        Bullet::on_draw(camera);
    }

private:
    BuffId buff_id = BuffId::INVALID;
    Animation animation_buff;

private:
    inline void setAtlas(Atlas* atlas){
        animation_buff.set_atlas(atlas);
        animation_buff.set_interval(100);
        animation_buff.set_loop(true);
    }

};

// class BuffRecoverHp : public BuffBullet {
// public:
//     BuffRecoverHp() : BuffBullet() {
//         setAtlas(&atlas_buffbox_pink);
//         buff_id = BuffId::RECOVER_HP;
//     }
// };

// class BuffRecoverMp : public BuffBullet {
// public:
//     BuffRecoverMp() : BuffBullet() {
//         setAtlas(&atlas_buffbox_blue);
//         buff_id = BuffId::RECOVER_MP;
//     }
// };

// class BuffInvisible : public BuffBullet {
// public:
//     BuffInvisible() : BuffBullet() {
//         setAtlas(&atlas_buffbox_yellow);
//         buff_id = BuffId::INVISIBLE;
//     }
// };

// class BuffHurry : public BuffBullet {
// public:
//     BuffHurry() : BuffBullet() {
//         setAtlas(&atlas_buffbox_yellow);
//         buff_id = BuffId::HURRY;
//     }
// };
#endif // _BUFF_BULLET_H_