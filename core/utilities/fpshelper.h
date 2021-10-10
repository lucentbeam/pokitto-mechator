#ifndef FPSHELPER_H
#define FPSHELPER_H

#include <deque>
#include <string>

#include "core//rendersystem.h"

class FPSHelper {
    std::deque<float> m_fps_list;
    float lastTime = 0;

    const int averageCount;
public:
    FPSHelper(int average_over_frames) : averageCount(average_over_frames) {}

    void update();
    void draw(int x, int y, int color) const;
};

#endif // FPSHELPER_H

void FPSHelper::update()
{
    m_fps_list.push_front(1000.0f/float(RenderSystem::getTimeMs() - lastTime));
    lastTime = RenderSystem::getTimeMs();
    while (m_fps_list.size() > averageCount) {
        m_fps_list.pop_back();
    }
}

void FPSHelper::draw(int x, int y, int color) const
{
    float avg = 0.0f;
    for(auto t : m_fps_list) {
        avg += t / m_fps_list.size();
    }
    char fpses[10];
    std::sprintf(fpses, "fps:%d", int(avg));
    RenderSystem::print(x, y, fpses, color);
}

