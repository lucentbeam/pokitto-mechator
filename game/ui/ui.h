#ifndef UI_H
#define UI_H

#include <vector>
#include <functional>
#include <algorithm>

#include "core//rendersystem.h"
#include "core//controls.h"
#include "game/enums.h"
#include "core/utilities/tween.h"

class UIElement {
    Tween tween;
    float m_showDuration = 0;

    const int16_t m_x,m_y,m_w,m_h,m_xh,m_yh,m_wh,m_hh;
    bool visible;

public:

    UIElement(int16_t x, int16_t y, int16_t w, int16_t h, int16_t x_hidden, int16_t y_hidden, int16_t w_hidden, int16_t h_hidden, Tween::Easing curve);

    void setVisibility(bool visible, bool immediate = false);
    void setVisibility(bool visible, uint32_t delay);
    void update(float dt);
    void draw(bool notched,void (*)(int16_t x, int16_t y, int16_t w, int16_t h),int dx = 0, int dy = 0);

    void showForDuration(float duration) { setVisibility(true); m_showDuration = duration; }

    static UIElement getExpander(int16_t x, int16_t y, int16_t w, int16_t h, Tween::Easing curve);
};

class UIOptions {
    const bool m_vertical;
    std::vector<const char*> m_options;

    uint8_t m_available;
    int8_t m_active_index;
public:
    UIOptions(bool vertical, std::initializer_list<const char*> options);

    void update(const ControlStatus&, void (*on_highlight)(int8_t index) = nullptr);

    void foreach(std::function<void(uint8_t idx, bool active, const char *)>);

    uint8_t activeIndex() const { return uint8_t(m_active_index); }

    void setAvailableCount(uint8_t avail) { m_available = std::min<uint8_t>(avail, uint8_t(m_options.size())); }

    void reset() { m_active_index = 0; }
};

class UI {

    static void drawNumber(uint16_t num, int x, int y);

public:
    enum Element {
        UIHackingKitCount,
        UIDollarCount,
        UIKeyACount,
        UIKeyBCount,
        UIKeyCCount
    };

    static void setVisibility(Element,bool,bool=false);
    static void setVisibility(Element,bool,uint32_t);
    static void showForDuration(Element,float);

    static void showHealthbar(PlayerMode);
    static void showHealthbar();
    static void hideHealthbar();


    static void update(float dt);
    static void draw();

    static void drawProgressBar(float fraction);
    static void drawBPrompt();

};

#endif // UI_H
