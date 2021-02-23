#ifndef VEC_H
#define VEC_H

#include <cmath>

class Vec2f
{
    float m_x, m_y;
//    float m_length;
//    bool m_init;
public:
    Vec2f(float x, float y) : m_x(x), m_y(y) {}

    float x() const { return m_x; }
    float y() const { return m_y; }
    float length() const { return std::sqrt(m_x*m_x+m_y*m_y); }
//    float length() { if (m_init) { return m_length; } else { m_length = std::sqrt(m_x*m_x+m_y*m_y); m_init = true; return m_length; }}

    void set(float x, float y) { m_x = x; m_y = y; }
    void setX(float x) { m_x = x; }
    void setY(float y) { m_y = y; }

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
