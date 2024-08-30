#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "Atlas.h"
#include "Camera.h"
#include "util.h"
#include "Vector2.h"

class Particle {
public:
    Particle() = default;
    Particle(const Vector2<int>& position, int lifespan, Atlas* atlas)
        : position(position), lifespan(lifespan), atlas(atlas) {}
    ~Particle() = default;

    void SetAtlas(Atlas* atlas);
    void SetPosition(const Vector2<int>& position);
    void SetLifespan(int lifespan);
    auto CheckValid() const -> bool;
    void OnUpdate(int delta);
    void OnDraw(const Camera& camera) const;

private:
    int timer = 0;
    int lifespan = 0;
    int idx_frame = 0;
    Vector2<int> position;
    bool valid = true;
    Atlas* atlas = nullptr;

};

#endif // _PARTICLE_H_