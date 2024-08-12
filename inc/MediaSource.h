#ifndef _MEDIA_H_
#define _MEDIA_H_

#define MEDIA_PATH_PREFIX "C:/Users/Salamanca/Desktop/PlantStarBrawl/media/"

#include <graphics.h>

#include "Atlas.h"
#include "util.h"

IMAGE img_menu_background;                      //  菜单背景

IMAGE img_VS;                                   //  VS 艺术字图片
IMAGE img_1P;                                   //  1P 图片
IMAGE img_2P;                                   //  2P 图片
IMAGE img_1P_desc;                              //  1P 描述图片
IMAGE img_2P_desc;                              //  2P 描述图片
IMAGE img_gravestone_left;                      //  左侧墓碑图片
IMAGE img_gravestone_right;                     //  右侧墓碑图片
IMAGE img_selector_tip;                         //  选择器提示图片
IMAGE img_selector_background;                  //  选择器背景图片
IMAGE img_1P_selector_btn_idle_left;            //  1P 选择器按钮闲置状态左侧图片
IMAGE img_1P_selector_btn_idle_right;           //  1P 选择器按钮闲置状态右侧图片
IMAGE img_1P_selector_btn_down_left;            //  1P 选择器按钮按下状态左侧图片
IMAGE img_1P_selector_btn_down_right;           //  1P 选择器按钮按下状态右侧图片
IMAGE img_2P_selector_btn_idle_left;            //  2P 选择器按钮闲置状态左侧图片
IMAGE img_2P_selector_btn_idle_right;           //  2P 选择器按钮闲置状态右侧图片
IMAGE img_2P_selector_btn_down_left;            //  2P 选择器按钮按下状态左侧图片
IMAGE img_2P_selector_btn_down_right;           //  2P 选择器按钮按下状态右侧图片
IMAGE img_peashooter_selector_background_left;  //  豌豆射手选择器背景左侧图片
IMAGE img_peashooter_selector_background_right; //  豌豆射手选择器背景右侧图片
IMAGE img_sunflower_selector_background_left;   //  向日葵选择器背景左侧图片
IMAGE img_sunflower_selector_background_right;  //  向日葵选择器背景右侧图片

IMAGE img_sky;                                  //  天空图片
IMAGE img_hills;                                //  山丘图片
IMAGE img_platform_large;                       //  大平台图片
IMAGE img_platform_small;                       //  小平台图片

IMAGE img_1P_cursor;                            //  1P 光标图片
IMAGE img_2P_cursor;                            //  2P 光标图片

Atlas atlas_peashooter_idle_left;               //  豌豆射手闲置状态左侧图集
Atlas atlas_peashooter_idle_right;              //  豌豆射手闲置状态右侧图集
Atlas atlas_peashooter_run_left;                //  豌豆射手奔跑状态左侧图集
Atlas atlas_peashooter_run_right;               //  豌豆射手奔跑状态右侧图集
Atlas atlas_peashooter_attack_ex_left;          //  豌豆射手攻击状态左侧图集
Atlas atlas_peashooter_attack_ex_right;         //  豌豆射手攻击状态右侧图集
Atlas atlas_peashooter_die_left;                //  豌豆射手死亡状态左侧图集
Atlas atlas_peashooter_die_right;               //  豌豆射手死亡状态右侧图集

Atlas atlas_sunflower_idle_left;                //  向日葵闲置状态左侧图集
Atlas atlas_sunflower_idle_right;               //  向日葵闲置状态右侧图集
Atlas atlas_sunflower_run_left;                 //  向日葵奔跑状态左侧图集
Atlas atlas_sunflower_run_right;                //  向日葵奔跑状态右侧图集
Atlas atlas_sunflower_attack_ex_left;           //  向日葵攻击状态左侧图集
Atlas atlas_sunflower_attack_ex_right;          //  向日葵攻击状态右侧图集
Atlas atlas_sunflower_die_left;                 //  向日葵死亡状态左侧图集
Atlas atlas_sunflower_die_right;                //  向日葵死亡状态右侧图集

IMAGE img_pea;                                  //  豌豆图片
Atlas atlas_pea_break;                          //  豌豆击中效果图集
Atlas atlas_sun;                                //  太阳图片图集
Atlas atlas_sun_explode;                        //  太阳爆炸效果图集
Atlas atlas_sun_ex;                             //  太阳额外效果图集
Atlas atlas_ex_explode;                         //  额外效果爆炸图集
Atlas atlas_sun_text;                           //  太阳文本图集

Atlas atlas_run_effect;                         //  奔跑效果图集
Atlas atlas_jump_effect;                        //  跳跃效果图集
Atlas atlas_land_effect;                        //  着陆效果图集

IMAGE img_1P_winner;                            //  1P 胜利图片
IMAGE img_2P_winner;                            //  2P 胜利图片
IMAGE img_winner_bar;                           //  胜利条图片

IMAGE img_avatat_peashooter;                    //  豌豆射手头像图片
IMAGE img_avatat_sunflower;                     //  向日葵头像图片


void flip_atlas(Atlas& src, Atlas& dest){
    dest.clear();
    for(int i = 0; i < src.get_size(); i++){
        IMAGE img_flip;
        flip_image(src.get_image(i), &img_flip);
        dest.add_image(img_flip);
    }
}

void load_game_resources(){
    AddFontResourceEx(_T(MEDIA_PATH_PREFIX "IPix.ttf"), FR_PRIVATE, NULL);

    loadimage(&img_menu_background, _T(MEDIA_PATH_PREFIX "menu_background.png"));

    loadimage(&img_VS, _T(MEDIA_PATH_PREFIX "VS.png"));
    loadimage(&img_1P, _T(MEDIA_PATH_PREFIX "1P.png"));
    loadimage(&img_2P, _T(MEDIA_PATH_PREFIX "2P.png"));
    loadimage(&img_1P_desc, _T(MEDIA_PATH_PREFIX "1P_desc.png"));
    loadimage(&img_2P_desc, _T(MEDIA_PATH_PREFIX "2P_desc.png"));
    loadimage(&img_gravestone_right, _T(MEDIA_PATH_PREFIX "gravestone.png"));
    flip_image(&img_gravestone_right, &img_gravestone_left);
    loadimage(&img_selector_tip, _T(MEDIA_PATH_PREFIX "selector_tip.png"));
    loadimage(&img_selector_background, _T(MEDIA_PATH_PREFIX "selector_background.png"));
    loadimage(&img_1P_selector_btn_idle_right, _T(MEDIA_PATH_PREFIX "1P_selector_btn_idle.png"));
    flip_image(&img_1P_selector_btn_idle_right, &img_1P_selector_btn_idle_left);
    loadimage(&img_1P_selector_btn_down_right, _T(MEDIA_PATH_PREFIX "1P_selector_btn_down.png"));
    flip_image(&img_1P_selector_btn_down_right, &img_1P_selector_btn_down_left);
    loadimage(&img_2P_selector_btn_idle_right, _T(MEDIA_PATH_PREFIX "2P_selector_btn_idle.png"));
    flip_image(&img_2P_selector_btn_idle_right, &img_2P_selector_btn_idle_left);
    loadimage(&img_2P_selector_btn_down_right, _T(MEDIA_PATH_PREFIX "2P_selector_btn_down.png"));
    flip_image(&img_2P_selector_btn_down_right, &img_2P_selector_btn_down_left);
    loadimage(&img_peashooter_selector_background_right, _T(MEDIA_PATH_PREFIX "peashooter_selector_background.png"));
    flip_image(&img_peashooter_selector_background_right, &img_peashooter_selector_background_left);
    loadimage(&img_sunflower_selector_background_right, _T(MEDIA_PATH_PREFIX "sunflower_selector_background.png"));
    flip_image(&img_sunflower_selector_background_right, &img_sunflower_selector_background_left);

    loadimage(&img_sky, _T(MEDIA_PATH_PREFIX "sky.png"));
    loadimage(&img_hills, _T(MEDIA_PATH_PREFIX "hills.png"));
    loadimage(&img_platform_large, _T(MEDIA_PATH_PREFIX "platform_large.png"));
    loadimage(&img_platform_small, _T(MEDIA_PATH_PREFIX "platform_small.png"));

    loadimage(&img_1P_cursor, _T(MEDIA_PATH_PREFIX "1P_cursor.png"));
    loadimage(&img_2P_cursor, _T(MEDIA_PATH_PREFIX "2P_cursor.png"));

    atlas_peashooter_idle_right.load_from_file(_T(MEDIA_PATH_PREFIX "peashooter_idle_%d.png"), 9);
    flip_atlas(atlas_peashooter_idle_right, atlas_peashooter_idle_left);
    atlas_peashooter_run_right.load_from_file(_T(MEDIA_PATH_PREFIX "peashooter_run_%d.png"), 5);
    flip_atlas(atlas_peashooter_run_right, atlas_peashooter_run_left);
    atlas_peashooter_attack_ex_right.load_from_file(_T(MEDIA_PATH_PREFIX "peashooter_attack_ex_%d.png"), 3);
    flip_atlas(atlas_peashooter_attack_ex_right, atlas_peashooter_attack_ex_left);
    atlas_peashooter_die_right.load_from_file(_T(MEDIA_PATH_PREFIX "peashooter_die_%d.png"), 4);
    flip_atlas(atlas_peashooter_die_right, atlas_peashooter_die_left);

    atlas_sunflower_idle_right.load_from_file(_T(MEDIA_PATH_PREFIX "sunflower_idle_%d.png"), 8);
    flip_atlas(atlas_sunflower_idle_right, atlas_sunflower_idle_left);
    atlas_sunflower_run_right.load_from_file(_T(MEDIA_PATH_PREFIX "sunflower_run_%d.png"), 5);
    flip_atlas(atlas_sunflower_run_right, atlas_sunflower_run_left);
    atlas_sunflower_attack_ex_right.load_from_file(_T(MEDIA_PATH_PREFIX "sunflower_attack_ex_%d.png"), 9);
    flip_atlas(atlas_sunflower_attack_ex_right, atlas_sunflower_attack_ex_left);
    atlas_sunflower_die_right.load_from_file(_T(MEDIA_PATH_PREFIX "sunflower_die_%d.png"), 2);
    flip_atlas(atlas_sunflower_die_right, atlas_sunflower_die_left);

    loadimage(&img_pea, _T(MEDIA_PATH_PREFIX "pea.png"));
    atlas_pea_break.load_from_file(_T(MEDIA_PATH_PREFIX "pea_break_%d.png"), 3);
    atlas_sun.load_from_file(_T(MEDIA_PATH_PREFIX "sun_%d.png"), 5);
    atlas_sun_explode.load_from_file(_T(MEDIA_PATH_PREFIX "sun_explode_%d.png"), 5);
    atlas_sun_ex.load_from_file(_T(MEDIA_PATH_PREFIX "sun_ex_%d.png"), 5);
    atlas_ex_explode.load_from_file(_T(MEDIA_PATH_PREFIX "ex_explode_%d.png"), 5);
    atlas_sun_text.load_from_file(_T(MEDIA_PATH_PREFIX "sun_text_%d.png"), 6);

    atlas_run_effect.load_from_file(_T(MEDIA_PATH_PREFIX "run_effect_%d.png"), 4);
    atlas_jump_effect.load_from_file(_T(MEDIA_PATH_PREFIX "jump_effect_%d.png"), 5);
    atlas_land_effect.load_from_file(_T(MEDIA_PATH_PREFIX "land_effect_%d.png"), 2);

    loadimage(&img_1P_winner, _T(MEDIA_PATH_PREFIX "1P_winner.png"));
    loadimage(&img_2P_winner, _T(MEDIA_PATH_PREFIX "2P_winner.png"));
    loadimage(&img_winner_bar, _T(MEDIA_PATH_PREFIX "winner_bar.png"));

    loadimage(&img_avatat_peashooter, _T(MEDIA_PATH_PREFIX "avatat_peashooter.png"));
    loadimage(&img_avatat_sunflower, _T(MEDIA_PATH_PREFIX "avatat_sunflower.png"));

    mciSendString(_T("open " MEDIA_PATH_PREFIX "bgm_game.mp3 alias bgm_game"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "bgm_menu.mp3 alias bgm_menu"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "pea_break_1.mp3 alias pea_break_1"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "pea_break_2.mp3 alias pea_break_2"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "pea_break_3.mp3 alias pea_break_3"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "pea_shoot_1.mp3 alias pea_shoot_1"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "pea_shoot_2.mp3 alias pea_shoot_2"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "pea_shoot_ex.mp3 alias pea_shoot_ex"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "sun_explode.mp3 alias sun_explode"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "sun_explode_ex.mp3 alias sun_explode_ex"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "sun_text.mp3 alias sun_text"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "ui_confirm.wav alias ui_confirm"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "ui_switch.wav alias ui_switch"), NULL, 0, NULL);
    mciSendString(_T("open " MEDIA_PATH_PREFIX "ui_win.wav alias ui_win"), NULL, 0, NULL);
}
#endif // _MEDIA_H_
