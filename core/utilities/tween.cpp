#include "tween.h"

#include <cmath>
#include <algorithm>

#include "core/rendering/rendersystem.h"

Tween::Tween(float duration) :
    m_duration(uint32_t(duration*1000))
{
    reset();
}

Tween::Tween(Tween::Easing c, float duration) :
    m_duration(uint32_t(duration*1000)),
    m_curve(c)
{
    reset();
}

void Tween::reset(uint32_t delay)
{
    m_start_time = RenderSystem::getTimeMs() + delay;
}

void Tween::end()
{
    m_start_time = RenderSystem::getTimeMs() - m_duration;
}

float Tween::getInterpolation(float start, float end) const
{
    int delta = int(RenderSystem::getTimeMs()) - int(m_start_time);
    float t = std::clamp<float>(float(delta)/float(m_duration), 0.0f, 1.0f);

    // remappings yoinked from this list:
    //   https://gist.github.com/gre/1650294
    switch (m_curve) {
    case Easing::OutQuad:
        t = t * (2 - t);
        break;
    case Easing::InQuad:
        t = t * t;
        break;
    case Easing::InOutQuad:
        t = t < 0.5f ? 2 * t * t : -1 + (4 - 2 * t) * t;
        break;
    default:
        break;
    }
    return t * end + (1 - t) * start;
}

int Tween::getInterpolationInt(float start, float end) const
{
    return std::floor(getInterpolation(start, end) + 0.5f);
}


