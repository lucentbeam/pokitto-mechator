#ifndef UI_H
#define UI_H

#include <vector>

#include "core/rendering/rendersystem.h"
#include "core/controls/controls.h"
#include "game/enums.h"
#include "game/utilities/tween.h"

class UIElement {
    Tween tween;

    const int16_t m_x,m_y,m_w,m_h,m_xh,m_yh,m_wh,m_hh;
    bool visible;

public:

    UIElement(int16_t x, int16_t y, int16_t w, int16_t h, int16_t x_hidden, int16_t y_hidden, int16_t w_hidden, int16_t h_hidden, Tween::Easing curve);

    void setVisibility(bool,bool=false);
    void setVisibility(bool,uint32_t);
    void draw(bool,void (*)(int16_t x, int16_t y, int16_t w, int16_t h));

    static UIElement getExpander(int16_t x, int16_t y, int16_t w, int16_t h, Tween::Easing curve);
};

class UIOptions {
    const bool m_vertical;
    std::vector<const char*> m_options;

    int8_t m_active_index;
public:
    UIOptions(bool vertical, std::initializer_list<const char*> options);

    void update(const ControlStatus&);

    void foreach(void (*)(uint8_t idx, bool active, const char *));

    uint8_t activeIndex() const { return uint8_t(m_active_index); }

    void reset() { m_active_index = 0; }
};

class UI {

    static void drawNumber(uint16_t num, int x, int y);

public:
    enum Element {
        UIHealthbar,
        UIHackingKitCount,
        UIDollarCount,
        UIKeyACount,
        UIKeyBCount,
        UIKeyCCount
    };

    static void setVisibility(Element,bool,bool=false);
    static void setVisibility(Element,bool,uint32_t);

    static void draw();

};

#endif // UI_H
