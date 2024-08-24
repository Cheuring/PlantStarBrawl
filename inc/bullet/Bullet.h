#ifndef _BUlLET_H_
#define _BUlLET_H_

#include <functional>
#include <graphics.h>

#include "Camera.h"
#include "PlayerId.h"
#include "Vector2.h"

extern bool is_debug;

class Bullet {
public:
    Bullet() = default;
    ~Bullet() = default;

    void SetDamage(int damage);
    void SetPosition(float x, float y);
    auto GetPosition() const -> const Vector2&;
    auto GetSize() const -> const Vector2&;
    void SetVelocity(float x, float y);
    void SetVelocity(const Vector2& velocity);
    void SetCollideTarget(PlayerId target_id);
    auto GetCollideTarget() const -> PlayerId;
    void SetCallback(std::function<void()> callback);
    void SetValid(bool valid);
    auto GetValid() const -> bool;
    auto CheckCanRemove() const -> bool;
    virtual auto GetDamage() const -> int;
    virtual void SetPosition(const Vector2& position);
    virtual void OnCollide();
    virtual auto CheckCollision(const Vector2& position, const Vector2& size) -> bool;
    virtual void OnUpdate(int delta) = 0;
    virtual void OnDraw(const Camera& camera) const;


protected:
    Vector2 size;
    Vector2 collision_offset;
    Vector2 position;
    Vector2 velocity;
    int damage = 10;
    bool valid = true;
    bool can_remove = false;
    std::function<void()> callback;
    PlayerId target_id = PlayerId::P1;

protected:
    auto check_if_exceeds_screen() const -> bool;
};

#endif // _BUlLET_H_