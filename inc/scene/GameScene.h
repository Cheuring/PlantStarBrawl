#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include <graphics.h>

#include "Camera.h"
#include "Platform.h"
#include "Player.h"
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

extern bool is_debug;

extern Player* player_1;
extern Player* player_2;

extern IMAGE* img_player_1_avatar;
extern IMAGE* img_player_2_avatar;

extern IMAGE img_1P_winner;
extern IMAGE img_2P_winner;
extern IMAGE img_winner_bar;

class GameScene : public Scene {
public:
    GameScene() = default;
    ~GameScene() = default;

    void on_enter() override {
        is_game_over = false;
        is_slide_out_started = false;

        pos_img_winner_bar.x = -img_winner_bar.getwidth();
        pos_img_winner_bar.y = (getheight() - img_winner_bar.getheight()) / 2;
        pos_x_img_winner_bat_dest = (getwidth() - img_winner_bar.getwidth()) / 2;

        pos_img_winner_text.x = img_1P_winner.getwidth();
        pos_img_winner_text.y = (getheight() - img_1P_winner.getheight()) / 2;
        pos_x_img_winner_text_dest = (getwidth() - img_1P_winner.getwidth()) / 2;

        timer_winner_slide_in.restart();
        timer_winner_slide_in.set_wait_time(2500);
        timer_winner_slide_in.set_one_shot(true);
        timer_winner_slide_in.set_callback([&](){
            is_slide_out_started = true;
        });

        timer_winner_slide_out.restart();
        timer_winner_slide_out.set_wait_time(1000);
        timer_winner_slide_out.set_one_shot(true);
        timer_winner_slide_out.set_callback([&](){
            scene_manager.switch_to(SceneManager::SceneType::Menu);
        });

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

        mciSendString(_T("play bgm_game repeat from 0"), NULL, 0, NULL);
    }

    void on_exit() override {
        delete player_1;
        delete player_2;
        player_1 = nullptr;
        player_2 = nullptr;

        is_debug = false;
        is_bgm_paused = false;

        for(auto& bullet : bullet_list){
            delete bullet;
        }
        bullet_list.clear();
        
        main_camera.reset();

        mciSendString(_T("stop bgm_game"), NULL, 0, NULL);
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
                case 'M':
                case 'm':
                    if(is_bgm_paused){
                        mciSendString(_T("resume bgm_game"), NULL, 0, NULL);
                        is_bgm_paused = false;
                    }
                    else{
                        mciSendString(_T("pause bgm_game"), NULL, 0, NULL);
                        is_bgm_paused = true;
                    }
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

        const auto& player_1_pos = player_1->get_position();
        const auto& player_2_pos = player_2->get_position();

        if(player_1_pos.y >= getheight()){
            player_1->set_hp(0);
        }
        if(player_2_pos.y >= getheight()){
            player_2->set_hp(0);
        }

        if(player_1->get_hp() <= 0 || player_2->get_hp() <= 0){
            if(!is_game_over) {
                mciSendString(_T("stop bgm_game"), NULL, 0, NULL);
                mciSendString(_T("play ui_win from 0"), NULL, 0, NULL);
            }

            is_game_over = true;
        }

        status_bar_1P.set_hp(player_1->get_hp());
        status_bar_1P.set_mp(player_1->get_mp());
        status_bar_2P.set_hp(player_2->get_hp());
        status_bar_2P.set_mp(player_2->get_mp());

        if(is_game_over) {
            pos_img_winner_bar.x += (int)(speed_winner_bar * delta);
            pos_img_winner_text.x += (int)(speed_winner_text * delta);

            if(is_slide_out_started){
                timer_winner_slide_out.on_update(delta);
            }else{
                timer_winner_slide_in.on_update(delta);

                if(pos_img_winner_bar.x > pos_x_img_winner_bat_dest){
                    pos_img_winner_bar.x = pos_x_img_winner_bat_dest;
                }
                if(pos_img_winner_text.x > pos_x_img_winner_text_dest){
                    pos_img_winner_text.x = pos_x_img_winner_text_dest;
                }
            }
        }
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

        if(is_game_over) {
            put_image_alpha(pos_img_winner_bar.x, pos_img_winner_bar.y, &img_winner_bar);
            put_image_alpha(pos_img_winner_text.x, pos_img_winner_text.y,
                player_1->get_hp() > player_2->get_hp() ? &img_1P_winner : &img_2P_winner);
        }else {
            status_bar_1P.on_draw();
            status_bar_2P.on_draw();
        }
    }

private:
    const float speed_winner_bar = 3.f;
    const float speed_winner_text = 1.5f;

    POINT pos_img_sky = {0, 0};
    POINT pos_img_hills = {0, 0};

    StatusBar status_bar_1P;
    StatusBar status_bar_2P;

    bool is_game_over = false;
    bool is_bgm_paused = false;

    POINT pos_img_winner_bar = {0, 0};
    POINT pos_img_winner_text = {0, 0};
    int pos_x_img_winner_bat_dest = 0;
    int pos_x_img_winner_text_dest = 0;
    Timer timer_winner_slide_in;
    Timer timer_winner_slide_out;
    bool is_slide_out_started = false;
};

#endif // _GAME_SCENE_H_