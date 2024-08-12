#ifndef _SELECTOR_SCENE_H_
#define _SELECTOR_SCENE_H_

#include "Animation.h"
#include "Atlas.h"
#include "Scene.h"
#include "SceneManager.h"

extern SceneManager scene_manager;

extern IMAGE img_VS;
extern IMAGE img_1P;
extern IMAGE img_2P;
extern IMAGE img_1P_desc;
extern IMAGE img_2P_desc;
extern IMAGE img_gravestone_left;
extern IMAGE img_gravestone_right;
extern IMAGE img_selector_tip;
extern IMAGE img_selector_background;
extern IMAGE img_1P_selector_btn_idle_left;
extern IMAGE img_1P_selector_btn_idle_right;
extern IMAGE img_1P_selector_btn_down_left;
extern IMAGE img_1P_selector_btn_down_right;
extern IMAGE img_2P_selector_btn_idle_left;
extern IMAGE img_2P_selector_btn_idle_right;
extern IMAGE img_2P_selector_btn_down_left;
extern IMAGE img_2P_selector_btn_down_right;
extern IMAGE img_peashooter_selector_background_left;
extern IMAGE img_peashooter_selector_background_right;
extern IMAGE img_sunflower_selector_background_left;
extern IMAGE img_sunflower_selector_background_right;

extern Atlas atlas_peashooter_idle_right;
extern Atlas atlas_sunflower_idle_right;

extern IMAGE img_avatat_peashooter;
extern IMAGE img_avatat_sunflower;


class SelectorScene : public Scene {
public:
    SelectorScene() = default;
    ~SelectorScene() = default;

    void on_enter() override {
        animation_peashooter.set_atlas(&atlas_peashooter_idle_right);
        animation_peashooter.set_interval(100);

        animation_sunflower.set_atlas(&atlas_sunflower_idle_right);
        animation_sunflower.set_interval(100);

        static const int OFFSET_X = 50;

        pos_img_VS.x = (getwidth() - img_VS.getwidth()) / 2;
        pos_img_VS.y = (getheight() - img_VS.getheight()) / 2;
        pos_img_tip.x = (getwidth() - img_selector_tip.getwidth()) / 2;
        pos_img_tip.y = getheight() - 125;
        pos_img_1P.x = (getwidth() / 2 - img_1P.getwidth()) / 2 - OFFSET_X;
        pos_img_1P.y = 35;
        pos_img_2P.x = getwidth() / 2 + (getwidth() / 2 - img_2P.getwidth()) / 2 + OFFSET_X;
        pos_img_2P.y = pos_img_1P.y;
        pos_img_1P_desc.x = (getwidth() / 2 - img_1P_desc.getwidth()) / 2 - OFFSET_X;
        pos_img_1P_desc.y = getheight() - 150;
        pos_img_2P_desc.x = getwidth() / 2 + (getwidth() / 2 - img_2P_desc.getwidth()) / 2 + OFFSET_X;
        pos_img_2P_desc.y = pos_img_1P_desc.y;
        pos_img_1P_gravestone.x = (getwidth() / 2 - img_gravestone_left.getwidth()) / 2 - OFFSET_X;
        pos_img_1P_gravestone.y = pos_img_1P.y + img_1P.getheight() + 35;
        pos_img_2P_gravestone.x = getwidth() / 2 + (getwidth() / 2 - img_gravestone_right.getwidth()) / 2 + OFFSET_X;
        pos_img_2P_gravestone.y = pos_img_1P_gravestone.y;
        pos_animation_1P.x = (getwidth() / 2 - atlas_peashooter_idle_right.get_image(0)->getwidth()) / 2 - OFFSET_X;
        pos_animation_1P.y = pos_img_1P_gravestone.y + 80;
        pos_animation_2P.x = getwidth() / 2 + (getwidth() / 2 - atlas_sunflower_idle_right.get_image(0)->getwidth()) / 2 + OFFSET_X;
        pos_animation_2P.y = pos_animation_1P.y;
        pos_img_1P_name.y = pos_animation_1P.y + 155;
        pos_img_2P_name.y = pos_img_1P_name.y;
        pos_1P_selector_btn_left.x = pos_img_1P_gravestone.x - img_1P_selector_btn_idle_left.getwidth();
        pos_1P_selector_btn_left.y = pos_img_1P_gravestone.y + (img_gravestone_left.getheight() - img_1P_selector_btn_idle_left.getheight()) / 2;
        pos_1P_selector_btn_right.x = pos_img_1P_gravestone.x + img_gravestone_left.getwidth();
        pos_1P_selector_btn_right.y = pos_1P_selector_btn_left.y;
        pos_2P_selector_btn_left.x = pos_img_2P_gravestone.x - img_2P_selector_btn_idle_left.getwidth();
        pos_2P_selector_btn_left.y = pos_1P_selector_btn_left.y;
        pos_2P_selector_btn_right.x = pos_img_2P_gravestone.x + img_gravestone_right.getwidth();
        pos_2P_selector_btn_right.y = pos_1P_selector_btn_left.y;
    }

    void on_update(int delta) override {
    }
    void on_draw(const Camera& camera) override {
        putimage(0, 0, &img_selector_background);

        put_image_alpha(pos_img_VS.x, pos_img_VS.y, &img_VS);

        put_image_alpha(pos_img_1P.x, pos_img_1P.y, &img_1P);
        put_image_alpha(pos_img_2P.x, pos_img_2P.y, &img_2P);
        put_image_alpha(pos_img_1P_gravestone.x, pos_img_1P_gravestone.y, &img_gravestone_left);
        put_image_alpha(pos_img_2P_gravestone.x, pos_img_2P_gravestone.y, &img_gravestone_right);

        put_image_alpha(pos_img_1P_desc.x, pos_img_1P_desc.y, &img_1P_desc);
        put_image_alpha(pos_img_2P_desc.x, pos_img_2P_desc.y, &img_2P_desc);

        put_image_alpha(pos_img_tip.x, pos_img_tip.y, &img_selector_tip);
    }
    void on_input(const ExMessage& msg) override {
    };
    void on_exit() override {
    }

private:
    POINT pos_img_VS = { 0, 0 };                //  VS 艺术字图片位置
    POINT pos_img_tip = { 0, 0 };               //  提示信息文本图片位置
    POINT pos_img_1P = { 0, 0 };                //  1P 文本图片位置
    POINT pos_img_2P = { 0, 0 };                //  2P 文本图片位置
    POINT pos_img_1P_desc = { 0, 0 };           //  1P 描述位置
    POINT pos_img_2P_desc = { 0, 0 };           //  2P 描述位置
    POINT pos_img_1P_name = { 0, 0 };           //  1P 名字位置
    POINT pos_img_2P_name = { 0, 0 };           //  2P 名字位置
    POINT pos_animation_1P = { 0, 0 };          //  1P 动画位置
    POINT pos_animation_2P = { 0, 0 };          //  2P 动画位置
    POINT pos_img_1P_gravestone = { 0, 0 };     //  1P 墓碑位置
    POINT pos_img_2P_gravestone = { 0, 0 };     //  2P 墓碑位置
    POINT pos_1P_selector_btn_left = { 0, 0 };  //  1P 向左切换按钮位置
    POINT pos_1P_selector_btn_right = { 0, 0 }; //  1P 向右切换按钮位置
    POINT pos_2P_selector_btn_left = { 0, 0 };  //  2P 向左切换按钮位置
    POINT pos_2P_selector_btn_right = { 0, 0 }; //  2P 向右切换按钮位置

    Animation animation_peashooter;
    Animation animation_sunflower;

};

#endif // _SELECTOR_SCENE_H_