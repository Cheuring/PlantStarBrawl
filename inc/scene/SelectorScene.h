#ifndef _SELECTOR_SCENE_H_
#define _SELECTOR_SCENE_H_

#include "Animation.h"
#include "Atlas.h"
#include "GameScene.h"
#include "GameType.h"
#include "GloomShroomPlayer.h"
#include "NutPlayer.h"
#include "PeashooterPlayer.h"
#include "PlayerId.h"
#include "SunflowerPlayer.h"
#include "Scene.h"
#include "SceneManager.h"

extern Player* player_1;
extern Player* player_2;
extern SceneManager scene_manager;
extern Scene* game_scene;
extern IMAGE* img_player_1_avatar;
extern IMAGE* img_player_2_avatar;

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
extern IMAGE img_gloomshroom_selector_background_left;
extern IMAGE img_gloomshroom_selector_background_right;
extern IMAGE img_nut_selector_background_left;
extern IMAGE img_nut_selector_background_right;

extern Atlas atlas_peashooter_idle_right;
extern Atlas atlas_peashooter_idle_left;
extern Atlas atlas_sunflower_idle_right;
extern Atlas atlas_sunflower_idle_left;
extern Atlas atlas_gloomshroom_idle_right;
extern Atlas atlas_gloomshroom_idle_left;
extern Atlas atlas_nut_idle_right;
extern Atlas atlas_nut_idle_left;

extern IMAGE img_avatar_peashooter;
extern IMAGE img_avatar_sunflower;
extern IMAGE img_avatar_gloomshroom;
extern IMAGE img_avatar_nut;


class SelectorScene : public Scene {
public:
    SelectorScene() = default;
    ~SelectorScene() = default;

    void OnEnter() override;
    void OnUpdate(int delta) override;
    void OnDraw(const Camera& camera) override;
    void OnInput(const ExMessage& msg, bool is_1P) override;
    void OnInput(const ExMessage& msg) override;
    void OnExit(GameType type) override;

private:
    enum class PlayerType {
        Peashooter,
        Sunflower,
        Gloomshroom,
        Nut,
        Invalid
    };

    POINT pos_img_VS = { 0, 0 };                //  VS ͼƬλ��
    POINT pos_img_tip = { 0, 0 };               //  ��ʾ��Ϣ�ı�ͼƬλ��
    POINT pos_img_1P = { 0, 0 };                //  1P �ı�ͼƬλ��
    POINT pos_img_2P = { 0, 0 };                //  2P �ı�ͼƬλ��
    POINT pos_img_1P_desc = { 0, 0 };           //  1P ����λ��
    POINT pos_img_2P_desc = { 0, 0 };           //  2P ����λ��
    POINT pos_img_1P_name = { 0, 0 };           //  1P ����λ��
    POINT pos_img_2P_name = { 0, 0 };           //  2P ����λ��
    POINT pos_animation_1P = { 0, 0 };          //  1P ����λ��
    POINT pos_animation_2P = { 0, 0 };          //  2P ����λ��
    POINT pos_img_1P_gravestone = { 0, 0 };     //  1P Ĺ��λ��
    POINT pos_img_2P_gravestone = { 0, 0 };     //  2P Ĺ��λ��
    POINT pos_1P_selector_btn_left = { 0, 0 };  //  1P ��ɫ�л���ťλ��
    POINT pos_1P_selector_btn_right = { 0, 0 }; //  1P ��ɫ�л���ťλ��
    POINT pos_2P_selector_btn_left = { 0, 0 };  //  2P ��ɫ�л���ťλ��
    POINT pos_2P_selector_btn_right = { 0, 0 }; //  2P ��ɫ�л���ťλ��

    Animation animation_peashooter;
    Animation animation_peashooter_left;
    Animation animation_sunflower;
    Animation animation_sunflower_left;
    Animation animation_gloomshroom;
    Animation animation_gloomshroom_left;
    Animation animation_nut;
    Animation animation_nut_left;

    PlayerType player_1P = PlayerType::Peashooter;
    PlayerType player_2P = PlayerType::Sunflower;

    LPCTSTR str_peashooter_name = _T("������");
    LPCTSTR str_sunflower_name = _T("���տ�");
    LPCTSTR str_gloomshroom_name = _T("ĩ�չ�");
    LPCTSTR str_nut_name = _T("������");

    int selector_background_scroll_offset_x = 0;

    bool is_btn_1P_left_down = false;
    bool is_btn_1P_right_down = false;
    bool is_btn_2P_left_down = false;
    bool is_btn_2P_right_down = false;

private:
    inline void outtextxy_shaded(int x, int y, LPCTSTR str){
        settextcolor(RGB(45, 45, 45));
        outtextxy(x + 3, y + 3, str);
        settextcolor(RGB(255, 255, 255));
        outtextxy(x, y, str);
    }

};

#endif // _SELECTOR_SCENE_H_