#ifndef VEC_H
#define VEC_H

#include <cmath>

class Vec2f
{
    float m_x, m_y;

public:
    Vec2f() : Vec2f(0, 0) {}
    Vec2f(float x, float y) : m_x(x), m_y(y) {}
    Vec2f(const Vec2f &other) : m_x(other.m_x), m_y(other.m_y) {}

    float x() const { return m_x; }
    float y() const { return m_y; }
    float length() const { return std::sqrt(m_x*m_x+m_y*m_y); }

    void set(float x, float y) { m_x = x; m_y = y; }
    void setX(float x) { m_x = x; }
    void setY(float y) { m_y = y; }

    Vec2f rot90() const { return Vec2f(m_y, -m_x); }

    void rotBy(float angle, bool normalize = true) {
        float l = 1.0f;
        if (!normalize) l = length();
        if (l == 0) return;
        float a = std::atan2(m_y, m_x);
        a += angle / 180.0f * 3.1415926f;
        m_x = std::cos(a) * l;
        m_y = std::sin(a) * l;
    }

    float dot(const Vec2f &other) { return m_x * other.m_x + m_y * other.m_y; }

    Vec2f operator-(const Vec2f &other) const {
        return Vec2f(m_x - other.m_x, m_y - other.m_y);
    }

    Vec2f operator+(const Vec2f &other) const {
        return Vec2f(m_x + other.m_x, m_y + other.m_y);
    }

    Vec2f& operator+=(const Vec2f &other) {
        m_x += other.m_x;
        m_y += other.m_y;
//        m_init = false;
        return *this;
    }

    Vec2f operator*(float val) const {
        return Vec2f(m_x * val, m_y * val);
    }

    Vec2f& operator*=(float val) {
        m_x *= val;
        m_y *= val;
//        m_length *= val;
        return *this;
    }

    Vec2f operator/(float val) const {
        return Vec2f(m_x / val, m_y / val);
    }

    Vec2f& operator/=(float val) {
        m_x /= val;
        m_y /= val;
//        m_length /= val;
        return *this;
    }
};

#endif // VEC_H
