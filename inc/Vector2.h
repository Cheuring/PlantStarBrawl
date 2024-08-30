#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#include <cmath>

template<typename ValueType>
class Vector2 {
public:
    Vector2() = default;
    Vector2(ValueType x, ValueType y) : x(x), y(y) {}

    template<typename OtherType>
    Vector2(const Vector2<OtherType>& other)
        : x(static_cast<ValueType>(other.x)), y(static_cast<ValueType>(other.y)) {}

    ~Vector2() = default;

    Vector2<ValueType> operator+(const Vector2<ValueType>& v) const {
        return Vector2<ValueType>(x + v.x, y + v.y);
    }

    Vector2<ValueType> operator-(const Vector2<ValueType>& v) const {
        return Vector2<ValueType>(x - v.x, y - v.y);
    }

    void operator+=(const Vector2<ValueType>& v) {
        x += v.x;
        y += v.y;
    }

    void operator-=(const Vector2<ValueType>& v) {
        x -= v.x;
        y -= v.y;
    }

    ValueType operator*(const Vector2<ValueType>& v) const {
        return x * v.x + y * v.y;
    }

    Vector2<ValueType> operator*(ValueType s) const {
        return Vector2<ValueType>(x * s, y * s);
    }

    Vector2<ValueType> operator/(ValueType s) const {
        return Vector2<ValueType>(x / s, y / s);
    }

    void operator*=(ValueType s) {
        x *= s;
        y *= s;
    }

    ValueType Length() const {
        return sqrt(x * x + y * y);
    }

    Vector2<ValueType> Normalize() const {
        ValueType len = Length();
        if(len == 0) return Vector2<ValueType>(0, 0);
        
        return Vector2<ValueType>(x / len, y / len);
    }

    ValueType x = 0;
    ValueType y = 0;
};


#endif // _VECTOR2_H_