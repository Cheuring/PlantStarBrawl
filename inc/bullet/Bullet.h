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
    void SetPosition(int x, int y);
    auto GetPosition() const -> const Vector2<int>&;
    auto GetSize() const -> const Vector2<int>&;
    void SetVelocity(float x, float y);
    void SetVelocity(const Vector2<float>& velocity);
    void SetCollideTarget(PlayerId target_id);
    auto GetCollideTarget() const -> PlayerId;
    void SetCallback(std::function<void()> callback);
    void SetValid(bool valid);
    auto GetValid() const -> bool;
    auto CheckCanRemove() const -> bool;
    virtual auto GetDamage() const -> int;
    virtual void SetPosition(const Vector2<int>& position);
    virtual void OnCollide();
    virtual auto CheckCollision(const Vector2<int>& position, const Vector2<int>& size) -> bool;
    virtual void OnUpdate(int delta) = 0;
    virtual void OnDraw(const Camera& camera) const;


protected:
    Vector2<int> size;
    Vector2<int> collision_offset;
    Vector2<int> position;
    Vector2<float> velocity;
    int damage = 10;
    bool valid = true;
    bool can_remove = false;
    std::function<void()> callback;
    PlayerId target_id = PlayerId::P1;

protected:
    auto check_if_exceeds_screen() const -> bool;
};

#endif // _BUlLET_H_