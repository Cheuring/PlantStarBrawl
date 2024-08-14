#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "Atlas.h"
#include "Camera.h"
#include "util.h"
#include "Vector2.h"

class Particle {
public:
    Particle() = default;
    Particle(const Vector2& position, int lifespan, Atlas* atlas)
        : position(position), lifespan(lifespan), atlas(atlas) {}
    ~Particle() = default;

    void set_atlas(Atlas* atlas) {
        this->atlas = atlas;
    }

    void set_position(const Vector2& position) {
        this->position = position;
    }

    void set_lifespan(int lifespan) {
        this->lifespan = lifespan;
    }

    bool check_valid() const {
        return valid;
    }

    void on_update(int delta) {
        timer += delta;
        if(timer >= lifespan) {
            timer = 0;
            idx_frame++;
            if(idx_frame >= atlas->get_size()) {
                idx_frame = atlas->get_size() - 1;
                valid = false;
            }
        }
    }

    void on_draw(const Camera& camera) const {
        put_image_alpha(camera, (int)position.x, (int)position.y, atlas->get_image(idx_frame));
    }

private:
    int timer = 0;
    int lifespan = 0;
    int idx_frame = 0;
    Vector2 position;
    bool valid = true;
    Atlas* atlas = nullptr;

};

#endif // _PARTICLE_H_