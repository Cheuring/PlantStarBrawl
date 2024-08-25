#include "GameScene.h"
#include <ctime>

GameScene::GameScene() : status_bar_1P(player_1->GetBuffList()), status_bar_2P(player_2->GetBuffList()) {
    srand((unsigned int)time(nullptr));
}
GameScene::~GameScene() {
    delete player_1;
    delete player_2;
    player_1 = nullptr;
    player_2 = nullptr;
}

void GameScene::OnEnter() {
    is_game_over = false;
    is_slide_out_started = false;

    pos_img_winner_bar.x = -img_winner_bar.getwidth();
    pos_img_winner_bar.y = (getheight() - img_winner_bar.getheight()) / 2;
    pos_x_img_winner_bat_dest = (getwidth() - img_winner_bar.getwidth()) / 2;

    pos_img_winner_text.x = img_1P_winner.getwidth();
    pos_img_winner_text.y = (getheight() - img_1P_winner.getheight()) / 2;
    pos_x_img_winner_text_dest = (getwidth() - img_1P_winner.getwidth()) / 2;

    timer_winner_slide_in.Restart();
    timer_winner_slide_in.SetWaitTime(2500);
    timer_winner_slide_in.SetOneShot(true);
    timer_winner_slide_in.SetCallback([&](){
        is_slide_out_started = true;
    });

    timer_winner_slide_out.Restart();
    timer_winner_slide_out.SetWaitTime(1000);
    timer_winner_slide_out.SetOneShot(true);
    timer_winner_slide_out.SetCallback([&](){
        scene_manager.SwitchTo(SceneManager::SceneType::Menu);
    });

    timer_buff_generate.Restart();
    timer_buff_generate.SetWaitTime(2000);
    timer_buff_generate.SetOneShot(false);
    timer_buff_generate.SetCallback([&](){
        int rand_buff_num = rand() % 4;
        BuffBullet* buff_bullet = new BuffBullet(rand_buff_num, rand());
        buff_bullet_list.push_back(buff_bullet);
    });

    status_bar_1P.SetAvatar(img_player_1_avatar);
    status_bar_2P.SetAvatar(img_player_2_avatar);

    status_bar_1P.SetPosition(235, 625);
    status_bar_2P.SetPosition(675, 625);

    player_1->SetPosition(200, 50);
    player_2->SetPosition(975, 50);

    pos_img_sky.x = (getwidth() - img_sky.getwidth()) / 2;
    pos_img_sky.y = (getheight() - img_sky.getheight()) / 2;

    pos_img_hills.x = (getwidth() - img_hills.getwidth()) / 2;
    pos_img_hills.y = (getheight() - img_hills.getheight()) / 2;

    platform_list.resize(4);
    
    Platform& large_plaform = platform_list[0];
    large_plaform.img = &img_platform_large;
    large_plaform.render_position.x = 122;
    large_plaform.render_position.y = 455;
    large_plaform.shape.left = (float)large_plaform.render_position.x + 30;
    large_plaform.shape.right = (float)large_plaform.render_position.x + img_platform_large.getwidth() - 30;
    large_plaform.shape.y = (float)large_plaform.render_position.y + 60;

    Platform& small_platform_1 = platform_list[1];
    small_platform_1.img = &img_platform_small;
    small_platform_1.render_position.x = 175;
    small_platform_1.render_position.y = 360;
    small_platform_1.shape.left = (float)small_platform_1.render_position.x + 40;
    small_platform_1.shape.right = (float)small_platform_1.render_position.x + img_platform_small.getwidth() - 40;
    small_platform_1.shape.y = (float)small_platform_1.render_position.y + img_platform_small.getheight() / 2;

    Platform& small_platform_2 = platform_list[2];
    small_platform_2.img = &img_platform_small;
    small_platform_2.render_position.x = 855;
    small_platform_2.render_position.y = 360;
    small_platform_2.shape.left = (float)small_platform_2.render_position.x + 40;
    small_platform_2.shape.right = (float)small_platform_2.render_position.x + img_platform_small.getwidth() - 40;
    small_platform_2.shape.y = (float)small_platform_2.render_position.y + img_platform_small.getheight() / 2;

    Platform& small_plaform_3 = platform_list[3];
    small_plaform_3.img = &img_platform_small;
    small_plaform_3.render_position.x = 515;
    small_plaform_3.render_position.y = 225;
    small_plaform_3.shape.left = (float)small_plaform_3.render_position.x + 40;
    small_plaform_3.shape.right = (float)small_plaform_3.render_position.x + img_platform_small.getwidth() - 40;
    small_plaform_3.shape.y = (float)small_plaform_3.render_position.y + img_platform_small.getheight() / 2;

    mciSendString(_T("play bgm_game repeat from 0"), NULL, 0, NULL);
}

void GameScene::OnExit() {
    for(auto& bullet : bullet_list){
        delete bullet;
    }
    bullet_list.clear();

    for(auto& buff : buff_bullet_list){
        delete buff;
    }
    buff_bullet_list.clear();
    
    main_camera.Reset();

    mciSendString(_T("stop bgm_game"), NULL, 0, NULL);
}

void GameScene::OnInput(const ExMessage& msg, bool is_1P) {
    if(is_1P) {
        player_1->OnInput(msg);
    }else {
        player_2->OnInput(msg);
    }

    switch (msg.message) {
        case WM_KEYUP:
            switch (msg.vkcode)
            {
            case VK_ESCAPE:
                scene_manager.SwitchTo(SceneManager::SceneType::Menu);
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

void GameScene::OnUpdate(int delta) {
    player_1->OnUpdate(delta);
    player_2->OnUpdate(delta);

    main_camera.OnUpdate(delta);

    bullet_list.erase(std::remove_if(bullet_list.begin(), bullet_list.end(), [&](Bullet* bullet){
        if(bullet->CheckCanRemove()){
            delete bullet;
            return true;
        }
        return false;
    }), bullet_list.end());

    buff_bullet_list.erase(std::remove_if(buff_bullet_list.begin(), buff_bullet_list.end(), [&](BuffBullet* buff){
        if(buff->CheckCanRemove()){
            delete buff;
            return true;
        }
        return false;
    }), buff_bullet_list.end());

    for(Bullet* bullet : bullet_list){
        bullet->OnUpdate(delta);
    }

    for(BuffBullet* buff : buff_bullet_list){
        buff->OnUpdate(delta);
    }

    timer_buff_generate.OnUpdate(delta);

    const auto& player_1_pos = player_1->GetPosition();
    const auto& player_2_pos = player_2->GetPosition();

    if(player_1_pos.y >= getheight()){
        player_1->SetHp(0);
    }
    if(player_2_pos.y >= getheight()){
        player_2->SetHp(0);
    }

    if(player_1->GetHp() <= 0 || player_2->GetHp() <= 0){
        if(!is_game_over) {
            mciSendString(_T("stop bgm_game"), NULL, 0, NULL);
            mciSendString(_T("play ui_win from 0"), NULL, 0, NULL);
        }

        is_game_over = true;
    }

    status_bar_1P.SetHp(player_1->GetHp());
    status_bar_1P.SetMp(player_1->GetMp());
    status_bar_2P.SetHp(player_2->GetHp());
    status_bar_2P.SetMp(player_2->GetMp());

    if(is_game_over) {
        pos_img_winner_bar.x += (int)(speed_winner_bar * delta);
        pos_img_winner_text.x += (int)(speed_winner_text * delta);

        if(is_slide_out_started){
            timer_winner_slide_out.OnUpdate(delta);
        }else{
            timer_winner_slide_in.OnUpdate(delta);

            if(pos_img_winner_bar.x > pos_x_img_winner_bat_dest){
                pos_img_winner_bar.x = pos_x_img_winner_bat_dest;
            }
            if(pos_img_winner_text.x > pos_x_img_winner_text_dest){
                pos_img_winner_text.x = pos_x_img_winner_text_dest;
            }
        }
    }
}

void GameScene::OnDraw(const Camera& camera) {
    PutImageAlpha(camera, pos_img_sky.x, pos_img_sky.y, &img_sky);
    PutImageAlpha(camera, pos_img_hills.x, pos_img_hills.y, &img_hills);

    for(const auto& platform : platform_list){
        platform.OnDraw(camera);
    }

    if(is_debug){
        settextcolor(RGB(255, 0, 0));
        outtextxy(15, 15, _T("Debug Mode"));
    }

    player_1->OnDraw(camera);
    player_2->OnDraw(camera);

    for(const auto& bullet : bullet_list){
        bullet->OnDraw(camera);
    }

    for(const auto& buff : buff_bullet_list){
        buff->OnDraw(camera);
    }

    if(is_game_over) {
        PutImageAlpha(pos_img_winner_bar.x, pos_img_winner_bar.y, &img_winner_bar);
        PutImageAlpha(pos_img_winner_text.x, pos_img_winner_text.y,
            player_1->GetHp() > player_2->GetHp() ? &img_1P_winner : &img_2P_winner);
    }else {
        status_bar_1P.OnDraw();
        status_bar_2P.OnDraw();
    }
}