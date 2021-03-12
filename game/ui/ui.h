#ifndef UI_H
#define UI_H

#include "core/rendering/rendersystem.h"
#include "game/enums.h"
#include "game/utilities/tween.h"

class UIElement {
    Tween tween;

    int16_t m_x,m_y,m_w,m_h,m_xh,m_yh,m_wh,m_hh;
    bool visible;

public:

    UIElement(int16_t x, int16_t y, int16_t w, int16_t h, int16_t x_hidden, int16_t y_hidden, int16_t w_hidden, int16_t h_hidden, Tween::Easing curve);

    void setVisibility(bool,bool=false);
    void setVisibility(bool,uint32_t);
    void draw(bool,void (*)(int16_t x, int16_t y, int16_t w, int16_t h));
};

class UI {

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
