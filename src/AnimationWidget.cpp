#include "AnimationWidget.h"

AnimationWidget::AnimationWidget(PlayerType player_type, int x, int y) {
    switch (player_type) {
    case PlayerType::Sunflower:
        animation_current.SetAtlas(&atlas_sunflower_idle_right);
        break;
    case PlayerType::Peashooter:
        animation_current.SetAtlas(&atlas_peashooter_idle_right);
        break;
    case PlayerType::Gloomshroom:
        animation_current.SetAtlas(&atlas_gloomshroom_idle_right);
        break;
    case PlayerType::Nut:
        animation_current.SetAtlas(&atlas_nut_idle_right);
        break;
    default:
        throw std::invalid_argument("Invalid player type");
    }
    this->x = x;
    this->y = y;

    animation_current.SetLoop(true);
    animation_current.SetInterval(100);
}

void AnimationWidget::OnUpdate(int delta) {
    animation_current.OnUpdate(delta);
}

void AnimationWidget::OnDraw(const Camera& camera) const {
    animation_current.OnDraw(camera, x, y);
}

void AnimationWidget::SetAnimation(const PlayerType& player_type) {
    switch (player_type) {
    case PlayerType::Sunflower:
        animation_current.SetAtlas(&atlas_sunflower_idle_right);
        break;
    case PlayerType::Peashooter:
        animation_current.SetAtlas(&atlas_peashooter_idle_right);
        break;
    case PlayerType::Gloomshroom:
        animation_current.SetAtlas(&atlas_gloomshroom_idle_right);
        break;
    case PlayerType::Nut:
        animation_current.SetAtlas(&atlas_nut_idle_right);
        break;
    default:
        throw std::invalid_argument("Invalid player type");
    }
}