#include <iostream>  //  Debug

#include "SelectorScene.h"

void SelectorScene::OnEnter() {
    animation_peashooter.SetAtlas(&atlas_peashooter_idle_right);
    animation_peashooter.SetInterval(100);

    animation_peashooter_left.SetAtlas(&atlas_peashooter_idle_left);
    animation_peashooter_left.SetInterval(100);

    animation_sunflower.SetAtlas(&atlas_sunflower_idle_right);
    animation_sunflower.SetInterval(100);

    animation_sunflower_left.SetAtlas(&atlas_sunflower_idle_left);
    animation_sunflower_left.SetInterval(100);

    animation_gloomshroom.SetAtlas(&atlas_gloomshroom_idle_right);
    animation_gloomshroom.SetInterval(100);

    animation_gloomshroom_left.SetAtlas(&atlas_gloomshroom_idle_left);
    animation_gloomshroom_left.SetInterval(100);

    animation_nut.SetAtlas(&atlas_nut_idle_right);
    animation_nut.SetInterval(100);

    animation_nut_left.SetAtlas(&atlas_nut_idle_left);
    animation_nut_left.SetInterval(100);

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
    pos_animation_1P.x = (getwidth() / 2 - atlas_peashooter_idle_right.GetImage(0)->getwidth()) / 2 - OFFSET_X;
    pos_animation_1P.y = pos_img_1P_gravestone.y + 80;
    pos_animation_2P.x = getwidth() / 2 + (getwidth() / 2 - atlas_sunflower_idle_right.GetImage(0)->getwidth()) / 2 + OFFSET_X;
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

void SelectorScene::OnUpdate(int delta) {
    animation_peashooter.OnUpdate(delta);
    animation_peashooter_left.OnUpdate(delta);
    animation_sunflower.OnUpdate(delta);
    animation_sunflower_left.OnUpdate(delta);
    animation_gloomshroom.OnUpdate(delta);
    animation_gloomshroom_left.OnUpdate(delta);
    animation_nut.OnUpdate(delta);
    animation_nut_left.OnUpdate(delta);

    selector_background_scroll_offset_x += 5;
    if(selector_background_scroll_offset_x >= img_peashooter_selector_background_left.getwidth()){
        selector_background_scroll_offset_x = 0;
    }
}

void SelectorScene::OnDraw(const Camera& camera) {
    IMAGE* img_p1_selector_background = nullptr;
    IMAGE* img_p2_selector_background = nullptr;

    switch (player_2P) {
        case PlayerType::Peashooter:
            img_p1_selector_background = &img_peashooter_selector_background_right;
            break;

        default:
        case PlayerType::Sunflower:
            img_p1_selector_background = &img_sunflower_selector_background_right;
            break;

        case PlayerType::Gloomshroom:
            img_p1_selector_background = &img_gloomshroom_selector_background_right;
            break;

        case PlayerType::Nut:
            img_p1_selector_background = &img_nut_selector_background_right;
            break;
    }

    switch (player_1P) {
        case PlayerType::Peashooter:
            img_p2_selector_background = &img_peashooter_selector_background_left;
            break;

        default:
        case PlayerType::Sunflower:
            img_p2_selector_background = &img_sunflower_selector_background_left;
            break;
        
        case PlayerType::Gloomshroom:
            img_p2_selector_background = &img_gloomshroom_selector_background_left;
            break;

        case PlayerType::Nut:
            img_p2_selector_background = &img_nut_selector_background_left;
            break;
    }

    putimage(0, 0, &img_selector_background);

    PutImageAlpha(selector_background_scroll_offset_x - img_p1_selector_background->getwidth(), 0, img_p1_selector_background);
    PutImageAlpha(selector_background_scroll_offset_x, 0,
        img_p1_selector_background->getwidth() - selector_background_scroll_offset_x, 0, img_p1_selector_background, 0, 0);
    PutImageAlpha(getwidth() - img_p2_selector_background->getwidth(), 0, img_p2_selector_background->getwidth() -
        selector_background_scroll_offset_x, 0, img_p2_selector_background, selector_background_scroll_offset_x, 0);
    PutImageAlpha(getwidth() - selector_background_scroll_offset_x, 0, img_p2_selector_background);

    PutImageAlpha(pos_img_VS.x, pos_img_VS.y, &img_VS);

    PutImageAlpha(pos_img_1P.x, pos_img_1P.y, &img_1P);
    PutImageAlpha(pos_img_2P.x, pos_img_2P.y, &img_2P);
    PutImageAlpha(pos_img_1P_gravestone.x, pos_img_1P_gravestone.y, &img_gravestone_left);
    PutImageAlpha(pos_img_2P_gravestone.x, pos_img_2P_gravestone.y, &img_gravestone_right);

    switch (player_1P) {
        case PlayerType::Peashooter:
            animation_peashooter.OnDraw(camera, pos_animation_1P.x, pos_animation_1P.y);
            pos_img_1P_name.x = pos_img_1P_gravestone.x + (img_gravestone_left.getwidth() - textwidth(str_peashooter_name)) / 2;
            outtextxy_shaded(pos_img_1P_name.x, pos_img_1P_name.y, str_peashooter_name);
            break;
        case PlayerType::Sunflower:
            animation_sunflower.OnDraw(camera, pos_animation_1P.x, pos_animation_1P.y);
            pos_img_1P_name.x = pos_img_1P_gravestone.x + (img_gravestone_left.getwidth() - textwidth(str_sunflower_name)) / 2;
            outtextxy_shaded(pos_img_1P_name.x, pos_img_1P_name.y, str_sunflower_name);
            break;
        case PlayerType::Gloomshroom:
            animation_gloomshroom.OnDraw(camera, pos_animation_1P.x, pos_animation_1P.y);
            pos_img_1P_name.x = pos_img_1P_gravestone.x + (img_gravestone_left.getwidth() - textwidth(str_gloomshroom_name)) / 2;
            outtextxy_shaded(pos_img_1P_name.x, pos_img_1P_name.y, str_gloomshroom_name);
            break;
        case PlayerType::Nut:
            animation_nut.OnDraw(camera, pos_animation_1P.x, pos_animation_1P.y);
            pos_img_1P_name.x = pos_img_1P_gravestone.x + (img_gravestone_left.getwidth() - textwidth(str_nut_name)) / 2;
            outtextxy_shaded(pos_img_1P_name.x, pos_img_1P_name.y, str_nut_name);
            break;
    }

    switch (player_2P) {
        case PlayerType::Peashooter:
            animation_peashooter_left.OnDraw(camera, pos_animation_2P.x, pos_animation_2P.y);
            pos_img_2P_name.x = pos_img_2P_gravestone.x + (img_gravestone_right.getwidth() - textwidth(str_peashooter_name)) / 2;
            outtextxy_shaded(pos_img_2P_name.x, pos_img_2P_name.y, str_peashooter_name);
            break;
        case PlayerType::Sunflower:
            animation_sunflower_left.OnDraw(camera, pos_animation_2P.x, pos_animation_2P.y);
            pos_img_2P_name.x = pos_img_2P_gravestone.x + (img_gravestone_right.getwidth() - textwidth(str_sunflower_name)) / 2;
            outtextxy_shaded(pos_img_2P_name.x, pos_img_2P_name.y, str_sunflower_name);
            break;
        case PlayerType::Gloomshroom:
            animation_gloomshroom_left.OnDraw(camera, pos_animation_2P.x, pos_animation_2P.y);
            pos_img_2P_name.x = pos_img_2P_gravestone.x + (img_gravestone_right.getwidth() - textwidth(str_gloomshroom_name)) / 2;
            outtextxy_shaded(pos_img_2P_name.x, pos_img_2P_name.y, str_gloomshroom_name);
            break;
        case PlayerType::Nut:
            animation_nut_left.OnDraw(camera, pos_animation_2P.x, pos_animation_2P.y);
            pos_img_2P_name.x = pos_img_2P_gravestone.x + (img_gravestone_right.getwidth() - textwidth(str_nut_name)) / 2;
            outtextxy_shaded(pos_img_2P_name.x, pos_img_2P_name.y, str_nut_name);
            break;
    }

    PutImageAlpha(pos_1P_selector_btn_left.x, pos_1P_selector_btn_left.y,
        is_btn_1P_left_down ? &img_1P_selector_btn_down_left : &img_1P_selector_btn_idle_left);
    PutImageAlpha(pos_1P_selector_btn_right.x, pos_1P_selector_btn_right.y,
        is_btn_1P_right_down ? &img_1P_selector_btn_down_right : &img_1P_selector_btn_idle_right);
    PutImageAlpha(pos_2P_selector_btn_left.x, pos_2P_selector_btn_left.y,
        is_btn_2P_left_down ? &img_2P_selector_btn_down_left : &img_2P_selector_btn_idle_left);
    PutImageAlpha(pos_2P_selector_btn_right.x, pos_2P_selector_btn_right.y,
        is_btn_2P_right_down ? &img_2P_selector_btn_down_right : &img_2P_selector_btn_idle_right);

    PutImageAlpha(pos_img_1P_desc.x, pos_img_1P_desc.y, &img_1P_desc);
    PutImageAlpha(pos_img_2P_desc.x, pos_img_2P_desc.y, &img_2P_desc);

    PutImageAlpha(pos_img_tip.x, pos_img_tip.y, &img_selector_tip);
}

void SelectorScene::OnInput(const ExMessage& msg, bool is_1P) {
    if(is_1P) {
        switch (msg.message) {
            case WM_KEYDOWN:
                switch (msg.vkcode){
                    case 'A':
                    case 'a':
                        is_btn_1P_left_down = true;
                        break;
                    case 'D':
                    case 'd':
                        is_btn_1P_right_down = true;
                        break;
                    default:
                        // std::cout << "Selector 1P: key " << msg.vkcode << '\n';
                        break;
                }
                break;
            case WM_KEYUP:
                switch (msg.vkcode){
                    case 'A':
                    case 'a':
                        is_btn_1P_left_down = false;
                        player_1P = (PlayerType)(((int)PlayerType::Invalid + (int)player_1P - 1) % (int)PlayerType::Invalid);
                        // std::cout << "start playing ui_switch\n"; //  Debug
                        mciSendString(_T("play ui_switch from 0"), NULL, 0, NULL);
                        break;
                    case 'D':
                    case 'd':
                        is_btn_1P_right_down = false;
                        player_1P = (PlayerType)(((int)player_1P + 1) % (int)PlayerType::Invalid);
                        mciSendString(_T("play ui_switch from 0"), NULL, 0, NULL);
                        break;
                    case VK_RETURN:
                        scene_manager.SwitchTo(SceneManager::SceneType::Game);
                        mciSendString(_T("play ui_confirm from 0"), NULL, 0, NULL);
                        break;
                    default:
                        // std::cout << "Selector 1P: key " << msg.vkcode << '\n';
                        break;
                }
                break;
            default:
                break;
        }
    }else {
        switch (msg.message) {
            case WM_KEYDOWN:
                switch (msg.vkcode){
                    case 'A':
                    case 'a':
                        is_btn_2P_left_down = true;
                        break;
                    case 'D':
                    case 'd':
                        is_btn_2P_right_down = true;
                        break;
                    default:
                        // std::cout << "Selector 2P: key " << msg.vkcode << '\n';
                        break;
                }
                break;
            case WM_KEYUP:
                switch (msg.vkcode){
                    case 'A':
                    case 'a':
                        is_btn_2P_left_down = false;
                        player_2P = (PlayerType)(((int)PlayerType::Invalid + (int)player_2P - 1) % (int)PlayerType::Invalid);
                        // std::cout << "start playing ui_switch\n"; //  Debug
                        mciSendString(_T("play ui_switch from 0"), NULL, 0, NULL);
                        break;
                    case 'D':
                    case 'd':
                        is_btn_2P_right_down = false;
                        player_2P = (PlayerType)(((int)player_2P + 1) % (int)PlayerType::Invalid);
                        // std::cout << "start playing ui_switch\n"; //  Debug
                        mciSendString(_T("play ui_switch from 0"), NULL, 0, NULL);
                        break;
                    case VK_RETURN:
                        scene_manager.SwitchTo(SceneManager::SceneType::Game);
                        mciSendString(_T("play ui_confirm from 0"), NULL, 0, NULL);
                        break;
                    default:
                        // std::cout << "Selector 2P: key " << msg.vkcode << '\n';
                        break;
                }
                break;
            default:
                break;
        }
    }
}

void SelectorScene::OnInput(const ExMessage& msg) {
    switch (msg.message) {
        case WM_KEYDOWN:
            switch (msg.vkcode){
                case 'A':
                case 'a':
                    is_btn_1P_left_down = true;
                    break;
                case 'D':
                case 'd':
                    is_btn_1P_right_down = true;
                    break;
                case VK_LEFT:
                    is_btn_2P_left_down = true;
                    break;
                case VK_RIGHT:
                    is_btn_2P_right_down = true;
                    break;
            }
            break;
        case WM_KEYUP:
            switch (msg.vkcode){
                case 'A':
                case 'a':
                    is_btn_1P_left_down = false;
                    player_1P = (PlayerType)(((int)PlayerType::Invalid + (int)player_1P - 1) % (int)PlayerType::Invalid);
                    mciSendString(_T("play ui_switch from 0"), nullptr, 0, nullptr);
                    break;
                case 'D':
                case 'd':
                    is_btn_1P_right_down = false;
                    player_1P = (PlayerType)(((int)player_1P + 1) % (int)PlayerType::Invalid);
                    mciSendString(_T("play ui_switch from 0"), nullptr, 0, nullptr);
                    break;
                case VK_LEFT:
                    is_btn_2P_left_down = false;
                    player_2P = (PlayerType)(((int)PlayerType::Invalid + (int)player_2P - 1) % (int)PlayerType::Invalid);
                    mciSendString(_T("play ui_switch from 0"), nullptr, 0, nullptr);
                    break;
                case VK_RIGHT:
                    is_btn_2P_right_down = false;
                    player_2P = (PlayerType)(((int)player_2P + 1) % (int)PlayerType::Invalid);
                    mciSendString(_T("play ui_switch from 0"), nullptr, 0, nullptr);
                    break;
                case VK_RETURN:
                    scene_manager.SwitchTo(SceneManager::SceneType::Game);
                    mciSendString(_T("play ui_confirm from 0"), nullptr, 0, nullptr);
                    break;
            }
            break;
        default:
            break;
    }
}

void SelectorScene::OnExit(GameType type) {
    switch (player_1P) {
        case PlayerType::Peashooter:
            player_1 = new PeashooterPlayer();
            img_player_1_avatar = &img_avatar_peashooter;
            break;
        case PlayerType::Sunflower:
            player_1 = new SunflowerPlayer();
            img_player_1_avatar = &img_avatar_sunflower;
            break;
        case PlayerType::Gloomshroom:
            player_1 = new GloomShroomPlayer();
            img_player_1_avatar = &img_avatar_gloomshroom;
            break;
        case PlayerType::Nut:
            player_1 = new NutPlayer();
            img_player_1_avatar = &img_avatar_nut;
            break;
    }
    player_1->SetId(PlayerId::P1);
    if(type == GameType::SERVER){
        player_1->SetLocal(true);
    }

    switch (player_2P) {
        case PlayerType::Peashooter:
            player_2 = new PeashooterPlayer();
            img_player_2_avatar = &img_avatar_peashooter;
            break;
        case PlayerType::Sunflower:
            player_2 = new SunflowerPlayer();
            img_player_2_avatar = &img_avatar_sunflower;
            break;
        case PlayerType::Gloomshroom:
            player_2 = new GloomShroomPlayer();
            img_player_2_avatar = &img_avatar_gloomshroom;
            break;
        case PlayerType::Nut:
            player_2 = new NutPlayer();
            img_player_2_avatar = &img_avatar_nut;
            break;
    }
    player_2->SetId(PlayerId::P2);
    if(type == GameType::CLIENT){
        player_2->SetLocal(true);
    }

    game_scene = new GameScene();

    mciSendString(_T("stop bgm_menu"), NULL, 0, NULL);
}