#include "Particle.h"

void Particle::SetAtlas(Atlas* atlas) {
    this->atlas = atlas;
}

void Particle::SetPosition(const Vector2<int>& position) {
    this->position = position;
}

void Particle::SetLifespan(int lifespan) {
    this->lifespan = lifespan;
}

auto Particle::CheckValid() const -> bool {
    return valid;
}

void Particle::OnUpdate(int delta) {
    timer += delta;
    if(timer >= lifespan) {
        timer = 0;
        idx_frame++;
        if(idx_frame >= atlas->GetSize()) {
            idx_frame = atlas->GetSize() - 1;
            valid = false;
        }
    }
}

void Particle::OnDraw(const Camera& camera) const {
    PutImageAlpha(camera, position.x, position.y, atlas->GetImage(idx_frame));
}