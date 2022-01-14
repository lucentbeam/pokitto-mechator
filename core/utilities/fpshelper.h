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
    void draw(int x, int y, int color_bg, int color_fg) const;
};

#endif // FPSHELPER_H

void FPSHelper::update()
{
    if (RenderSystem::getTimeMs() == lastTime) return;
    m_fps_list.push_front(1000.0f/float(RenderSystem::getTimeMs() - lastTime));
    lastTime = RenderSystem::getTimeMs();
    while (m_fps_list.size() > averageCount) {
        m_fps_list.pop_back();
    }
}

void FPSHelper::draw(int x, int y, int color) const
{
    if (m_fps_list.size() == 0) return;
    float avg = 0.0f;
    for(auto t : m_fps_list) {
        avg += t / m_fps_list.size();
    }
    char fpses[10];
    std::sprintf(fpses, "fps:%d", int(avg));
    RenderSystem::print(x, y, fpses, color);
}

void FPSHelper::draw(int x, int y, int color_bg, int color_fg) const
{
    if (m_fps_list.size() == 0) return;
    float avg = 0.0f;
    for(auto t : m_fps_list) {
        avg += t / m_fps_list.size();
    }
    char fpses[10];
    std::sprintf(fpses, "fps:%d", int(avg));
    RenderSystem::print(x, y + 1, fpses, color_bg);
    RenderSystem::print(x, y, fpses, color_fg);
}

