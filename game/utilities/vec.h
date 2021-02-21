#ifndef VEC_H
#define VEC_H

#include <cmath>

class Vec2f
{
    float m_x, m_y, m_length;
    bool init;
public:
    Vec2f(float x, float y) : m_x(x), m_y(y) {}

    float x() const { return m_x; }
    float y() const { return m_y; }
    float length() { if (init) { return m_length; } else { m_length = std::sqrt(m_x*m_x+m_y*m_y); init = true; return m_length; }}

    void set(float x, float y) { m_x = x; m_y = y; init = false; }
    void setX(float x) { m_x = x; init = false; }
    void setY(float y) { m_y = y; init = false; }

    float dot(const Vec2f &other) { return m_x * other.m_x + m_y * other.m_y; }
};

#endif // VEC_H
