#ifndef UI_H
#define UI_H

#include <vector>
#include <functional>
#include <algorithm>

#include "core//rendersystem.h"
#include "core//controls.h"
#include "game/enums.h"
#include "game/constants.h"
#include "core/utilities/tween.h"

class UIElement {
    Tween tween;

    int16_t m_x,m_y,m_w,m_h,m_xh,m_yh,m_wh,m_hh;
    uint8_t m_showDuration = 0;
    bool visible;

public:

    UIElement(int16_t x, int16_t y, int16_t w, int16_t h, int16_t x_hidden, int16_t y_hidden, int16_t w_hidden, int16_t h_hidden, Tween::Easing curve);

    void setVisibility(bool visible, bool immediate = false);
    void setVisibility(bool visible, uint32_t delay);
    void update(float dt);
//    void draw(bool notched,void (*)(int16_t x, int16_t y, int16_t w, int16_t h),int dx = 0, int dy = 0);
    void draw(bool notched,std::function<void(int16_t x, int16_t y, int16_t w, int16_t h)>,int dx = 0, int dy = 0);

    void showForDuration(float duration) { setVisibility(true); m_showDuration = std::min(int(duration/physicsTimestep), 255); }

    void setMaxWidth(int w);
    void setCenter(float x, float y);

    bool active() const { return tween.started() && !tween.done(); }
    bool done() const { return tween.started() && tween.done(); }

    bool showing() const { return visible && done(); }
    bool hidden() const { return !visible && done(); }

    static UIElement getExpander(int16_t x, int16_t y, int16_t w, int16_t h, Tween::Easing curve);
};

class UIOptions {
    bool m_active[16] = { true };

    uint8_t m_available;
    int8_t m_active_index;
public:
    UIOptions(int count) { m_available = count; for (int i = 0; i < count; ++i) m_active[i] = true; }

    int update(bool forward, bool back, void (*on_highlight)(int8_t index) = nullptr, bool cycle = false);

    void foreach(std::function<void(uint8_t idx, bool active)>);

    void foreach(std::function<void(uint8_t idx, uint8_t actual_idx, bool active)>);

    uint8_t activeIndex() const { return uint8_t(m_active_index); }

    void setAvailableCount(uint8_t avail) { if (m_active_index >= avail) m_active_index = avail -1; m_available = avail; { for (int i = 0; i < avail; ++i) m_active[i] = true; } }

    void setActiveAt(int idx, bool active) { m_active[idx] = active; }

    void reset() { m_active_index = 0; }
};

class UI {

    static void drawNumber(uint16_t num, int x, int y);

    static int8_t * m_boss_life;
    static int8_t m_max_boss_life;
public:
    enum Element {
        UIHackingKitCount,
        UIDollarCount,
        UIKeyACount,
        UIKeyBCount,
        UIKeyCCount,
        GameSavedPrompt
    };

    static void setVisibility(Element,bool,bool=false);
    static void setVisibility(Element,bool,uint32_t);
    static void showForDuration(Element,float);

    static void showHealthbar(PlayerMode);
    static void showHealthbar();
    static void hideHealthbar();

    static void showBoss(int8_t * life_ref);
    static void clearBoss();

    static void update(float dt);
    static void draw();

};

#endif // UI_H
