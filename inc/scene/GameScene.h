#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include <graphics.h>

#include "Camera.h"
#include "Platform.h"
#include "Player.h"
#include "MediaSource.h"
#include "Scene.h"
#include "SceneManager.h"
#include "StatusBar.h"
#include "util.h"

extern IMAGE img_sky;
extern IMAGE img_hills;
extern IMAGE img_platform_large;
extern IMAGE img_platform_small;

extern Camera main_camera;
extern std::vector<Bullet *> bullet_list;
extern std::vector<Platform> platform_list;
extern SceneManager scene_manager;

extern Player* player_1;
extern Player* player_2;

extern IMAGE* img_player_1_avatar;
extern IMAGE* img_player_2_avatar;

class GameScene : public Scene {
public:
    GameScene() = default;
    ~GameScene() = default;

    void on_enter() override {
        status_bar_1P.set_avatar(img_player_1_avatar);
        status_bar_2P.set_avatar(img_player_2_avatar);

        status_bar_1P.set_position(235, 625);
        status_bar_2P.set_position(675, 625);

        player_1->set_position(200, 50);
        player_2->set_position(975, 50);

        pos_img_sky.x = (getwidth() - img_sky.getwidth()) / 2;
        pos_img_sky.y = (getheight() - img_sky.getheight()) / 2;

        pos_img_hills.x = (getwidth() - img_hills.getwidth()) / 2;
        pos_img_hills.y = (getheight() - img_hills.getheight()) / 2;

        platform_list.resize(4);
        
        Platform& large_plaform = platform_list[0];
        large_plaform.img = &img_platform_large;
        large_plaform.render_positon.x = 122;
        large_plaform.render_positon.y = 455;
        large_plaform.shape.left = (float)large_plaform.render_positon.x + 30;
        large_plaform.shape.right = (float)large_plaform.render_positon.x + img_platform_large.getwidth() - 30;
        large_plaform.shape.y = (float)large_plaform.render_positon.y + 60;

        Platform& small_platform_1 = platform_list[1];
        small_platform_1.img = &img_platform_small;
        small_platform_1.render_positon.x = 175;
        small_platform_1.render_positon.y = 360;
        small_platform_1.shape.left = (float)small_platform_1.render_positon.x + 40;
        small_platform_1.shape.right = (float)small_platform_1.render_positon.x + img_platform_small.getwidth() - 40;
        small_platform_1.shape.y = (float)small_platform_1.render_positon.y + img_platform_small.getheight() / 2;

        Platform& small_platform_2 = platform_list[2];
        small_platform_2.img = &img_platform_small;
        small_platform_2.render_positon.x = 855;
        small_platform_2.render_positon.y = 360;
        small_platform_2.shape.left = (float)small_platform_2.render_positon.x + 40;
        small_platform_2.shape.right = (float)small_platform_2.render_positon.x + img_platform_small.getwidth() - 40;
        small_platform_2.shape.y = (float)small_platform_2.render_positon.y + img_platform_small.getheight() / 2;

        Platform& small_plaform_3 = platform_list[3];
        small_plaform_3.img = &img_platform_small;
        small_plaform_3.render_positon.x = 515;
        small_plaform_3.render_positon.y = 225;
        small_plaform_3.shape.left = (float)small_plaform_3.render_positon.x + 40;
        small_plaform_3.shape.right = (float)small_plaform_3.render_positon.x + img_platform_small.getwidth() - 40;
        small_plaform_3.shape.y = (float)small_plaform_3.render_positon.y + img_platform_small.getheight() / 2;
    }

    void on_exit() override {
    }

    void on_input(const ExMessage& msg) override {
        player_1->on_input(msg);
        player_2->on_input(msg);

        switch (msg.message) {
            case WM_KEYUP:
                switch (msg.vkcode)
                {
                case VK_ESCAPE:
                    scene_manager.switch_to(SceneManager::SceneType::Menu);
                    break;
                case 'q':
                case 'Q':
                    is_debug = !is_debug;
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
        }
    }

    void on_update(int delta) override {
        player_1->on_update(delta);
        player_2->on_update(delta);

        main_camera.on_update(delta);

        bullet_list.erase(std::remove_if(bullet_list.begin(), bullet_list.end(), [&](Bullet* bullet){
            if(bullet->check_can_remove()){
                delete bullet;
                return true;
            }
            return false;
        }), bullet_list.end());

        for(Bullet* bullet : bullet_list){
            bullet->on_update(delta);
        }

        status_bar_1P.set_hp(player_1->get_hp());
        status_bar_1P.set_mp(player_1->get_mp());
        status_bar_2P.set_hp(player_2->get_hp());
        status_bar_2P.set_mp(player_2->get_mp());
    }

    void on_draw(const Camera& camera) override {
        put_image_alpha(camera, pos_img_sky.x, pos_img_sky.y, &img_sky);
        put_image_alpha(camera, pos_img_hills.x, pos_img_hills.y, &img_hills);

        for(const auto& platform : platform_list){
            platform.on_draw(camera);
        }

        if(is_debug){
            settextcolor(RGB(255, 0, 0));
            outtextxy(15, 15, _T("Debug Mode"));
        }

        player_1->on_draw(camera);
        player_2->on_draw(camera);

        for(const auto& bullet : bullet_list){
            bullet->on_draw(camera);
        }

        status_bar_1P.on_draw();
        status_bar_2P.on_draw();
    }

private:
    POINT pos_img_sky = {0, 0};
    POINT pos_img_hills = {0, 0};

    StatusBar status_bar_1P;
    StatusBar status_bar_2P;

};

#endif // _GAME_SCENE_H_