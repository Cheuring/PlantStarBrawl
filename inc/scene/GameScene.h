#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include <graphics.h>

#include "BuffBullet.h"
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
extern std::vector<BuffBullet *> buff_bullet_list;
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
    GameScene();
    ~GameScene();

    void OnEnter() override;
    void OnExit() override;
    void OnInput(const ExMessage& msg, bool is_1P) override;
    void OnUpdate(int delta) override;
    void OnDraw(const Camera& camera) override;

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

    Timer timer_buff_generate;
};

#endif // _GAME_SCENE_H_