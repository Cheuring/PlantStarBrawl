#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#include <cmath>

class Vector2 {
public:
    Vector2() = default;
    Vector2(float x, float y) : x(x), y(y) {}
    ~Vector2() = default;

    Vector2 operator+(const Vector2& v) const {
        return Vector2(x + v.x, y + v.y);
    }

    Vector2 operator-(const Vector2& v) const {
        return Vector2(x - v.x, y - v.y);
    }

    void operator+=(const Vector2& v) {
        x += v.x;
        y += v.y;
    }

    void operator-=(const Vector2& v) {
        x -= v.x;
        y -= v.y;
    }

    float operator*(const Vector2& v) const {
        return x * v.x + y * v.y;
    }

    Vector2 operator*(float s) const {
        return Vector2(x * s, y * s);
    }

    friend Vector2 operator*(float s, const Vector2& v) {
        return Vector2(v.x * s, v.y * s);
    }

    void operator*=(float s) {
        x *= s;
        y *= s;
    }

    float length() const {
        return sqrt(x * x + y * y);
    }

    Vector2 normalize() const {
        float len = length();
        if(len == 0) return Vector2(0, 0);
        
        return Vector2(x / len, y / len);
    }

    float x = 0;
    float y = 0;
};


#endif // _VECTOR2_H_