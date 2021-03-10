#include "tween.h"

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

void Tween::reset()
{
    m_start_time = RenderSystem::getTimeMs();
}

float Tween::getInterpolation(float start, float end) const
{
    uint32_t delta = RenderSystem::getTimeMs() - m_start_time;
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


