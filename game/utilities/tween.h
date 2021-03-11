#ifndef TWEEN_H
#define TWEEN_H

#include <cstdint>

class Tween
{
public:
    enum Easing {
        Linear,
        OutQuad,
        InQuad,
        InOutQuad
    };

    Tween(float duration);

    Tween(Easing c, float duration);

    void reset();

    void end();

    float getInterpolation(float start, float end) const;

    int getInterpolationInt(float start, float end) const;

private:
    uint32_t m_start_time;

    const uint32_t m_duration;

    const Easing m_curve = Easing::Linear;
};

#endif // TWEEN_H
