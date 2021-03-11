#include "ui.h"

#include "game/tilesets.h"
#include "game/player.h"
#include "game/constants.h"

UIElement::UIElement(int16_t x, int16_t y, int16_t w, int16_t h, int16_t x_hidden, int16_t y_hidden, int16_t w_hidden, int16_t h_hidden, Tween::Easing curve) :
    tween(curve, uiEasingTime),
    m_x(x),
    m_y(y),
    m_w(w),
    m_h(h),
    m_xh(x_hidden),
    m_yh(y_hidden),
    m_wh(w_hidden),
    m_hh(h_hidden)
{

}

void UIElement::setVisibility(bool vis, bool immediate)
{
    if (vis == visible) {
        return;
    }
    visible = vis;
    if (immediate) {
        tween.end();
    } else {
        tween.reset();
    }
}

void UIElement::draw(void (*callback)(int16_t, int16_t, int16_t, int16_t) = nullptr)
{
    float x = visible ? tween.getInterpolation(m_xh, m_x) : tween.getInterpolation(m_x, m_xh);
    float y = visible ? tween.getInterpolation(m_yh, m_y) : tween.getInterpolation(m_y, m_yh);
    float w = visible ? tween.getInterpolation(m_wh, m_w) : tween.getInterpolation(m_w, m_wh);
    float h = visible ? tween.getInterpolation(m_hh, m_h) : tween.getInterpolation(m_h, m_hh);

    RenderSystem::drawRect(x, y, w, h, 0);

    if (callback != nullptr) {
        callback(x,y,w,h);
    }
}

UIElement healthbar(0,0,7,88,-7,0,7,88,Tween::Easing::OutQuad);
UIElement kitcount(66,78,19,9,66,87,19,0,Tween::Easing::OutQuad);
UIElement dollarcount(86,78,23,9,86,87,23,0,Tween::Easing::OutQuad);

UIElement keyacount(90,1,19,8,90,9,19,0,Tween::Easing::OutQuad);
UIElement keybcount(90,10,19,8,90,18,19,0,Tween::Easing::OutQuad);
UIElement keyccount(90,19,19,8,90,27,19,0,Tween::Easing::OutQuad);

void UI::setVisibility(UI::Element element, bool visible, bool immediate)
{
    switch (element) {
    case UI::Element::UIHealthbar:
        healthbar.setVisibility(visible,immediate);
        break;
    case UI::Element::UIHackingKitCount:
        kitcount.setVisibility(visible,immediate);
        break;
    case UI::Element::UIDollarCount:
        dollarcount.setVisibility(visible,immediate);
        break;
    case UI::Element::UIKeyACount:
        keyacount.setVisibility(visible,immediate);
        break;
    case UI::Element::UIKeyBCount:
        keybcount.setVisibility(visible,immediate);
        break;
    case UI::Element::UIKeyCCount:
        keyccount.setVisibility(visible,immediate);
        break;
    }
}

void UI::draw()
{
    healthbar.draw([](int16_t x, int16_t y, int16_t w, int16_t h) {
        PlayerMode mode = Player::mode();
        uint8_t current = mode == PlayerMode::Soldier ? Player::s_stats.health_soldier.value() : Player::s_stats.health_jeep.value();
        uint8_t max = mode == PlayerMode::Soldier ? Player::s_stats.health_soldier.max() : Player::s_stats.health_jeep.max();

        for(uint8_t i = 0; i < max; i++) {
            int idx = i < current ? uint8_t(mode) : 0;
            RenderSystem::sprite(2 + x, 84 - i * 3, health_pips[idx]);
        }
    });

    kitcount.draw([](int16_t x, int16_t y, int16_t w, int16_t h) {

    });

    dollarcount.draw([](int16_t x, int16_t y, int16_t w, int16_t h) {

    });

    keyacount.draw([](int16_t x, int16_t y, int16_t w, int16_t h) {

    });

    keybcount.draw([](int16_t x, int16_t y, int16_t w, int16_t h) {

    });

    keyccount.draw([](int16_t x, int16_t y, int16_t w, int16_t h) {

    });
}

