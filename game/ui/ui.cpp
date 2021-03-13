#include "ui.h"

#include "game/tilesets.h"
#include "game/player.h"
#include "game/constants.h"
#include "game/tilesets.h"
#include "game/variables.h"

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

void UIElement::setVisibility(bool vis, uint32_t delay)
{
    if (vis == visible) {
        return;
    }
    visible = vis;
    tween.reset(delay);
}

void UIElement::draw(bool useNotched, void (*callback)(int16_t, int16_t, int16_t, int16_t) = nullptr)
{
    int x = visible ? tween.getInterpolationInt(m_xh, m_x) : tween.getInterpolationInt(m_x, m_xh);
    int y = visible ? tween.getInterpolationInt(m_yh, m_y) : tween.getInterpolationInt(m_y, m_yh);
    int w = visible ? tween.getInterpolationInt(m_wh, m_w) : tween.getInterpolationInt(m_w, m_wh);
    int h = visible ? tween.getInterpolationInt(m_hh, m_h) : tween.getInterpolationInt(m_h, m_hh);

    if (w <= 0 || h <= 0) { return; }

    if (useNotched) {
        Helpers::drawNotchedRect(x, y, w, h, 0);
    } else {
        RenderSystem::drawRect(x, y, w, h, 0);
    }

    if (callback != nullptr) {
        callback(x,y,w,h);
    }
}

UIElement UIElement::getExpander(int16_t x, int16_t y, int16_t w, int16_t h, Tween::Easing curve)
{
    return UIElement(x-w/2,y-h/2,w,h,x,y,0,0,curve);
}

static UIElement healthbar(0,0,7,88,-7,0,7,88,Tween::Easing::OutQuad);

static UIElement kitcount(66,78,19,9,76,82,0,0,Tween::Easing::OutQuad);
static UIElement dollarcount(86,78,23,9,97,82,0,0,Tween::Easing::OutQuad);

static UIElement keyacount(90,1,19,8,100,5,0,0,Tween::Easing::OutQuad);
static UIElement keybcount(90,10,19,8,100,14,0,0,Tween::Easing::OutQuad);
static UIElement keyccount(90,19,19,8,100,23,0,0,Tween::Easing::OutQuad);

void UI::drawNumber(uint16_t num, int x, int y)
{
    char buf[5];
    sprintf(buf, "%d", num);
    if (num < 10) {
        x += 1;
    }
    RenderSystem::print(x, y, buf, num == 0 ? 13 : 41);
}

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

void UI::setVisibility(UI::Element element, bool visible, uint32_t delay)
{
    switch (element) {
    case UI::Element::UIHealthbar:
        healthbar.setVisibility(visible,delay);
        break;
    case UI::Element::UIHackingKitCount:
        kitcount.setVisibility(visible,delay);
        break;
    case UI::Element::UIDollarCount:
        dollarcount.setVisibility(visible,delay);
        break;
    case UI::Element::UIKeyACount:
        keyacount.setVisibility(visible,delay);
        break;
    case UI::Element::UIKeyBCount:
        keybcount.setVisibility(visible,delay);
        break;
    case UI::Element::UIKeyCCount:
        keyccount.setVisibility(visible,delay);
        break;
    }
}

void UI::draw()
{
    healthbar.draw(false, [](int16_t x, int16_t, int16_t, int16_t) {
        PlayerMode mode = Player::mode();
        uint8_t current = mode == PlayerMode::Soldier ? Player::s_stats.health_soldier.value() : Player::s_stats.health_jeep.value();
        uint8_t max = mode == PlayerMode::Soldier ? Player::s_stats.health_soldier.max() : Player::s_stats.health_jeep.max();

        for(uint8_t i = 0; i < max; i++) {
            int idx = i < current ? uint8_t(mode) : 0;
            RenderSystem::sprite(2 + x, 84 - i * 3, health_pips[idx]);
        }
    });

    kitcount.draw(true, [](int16_t x, int16_t y, int16_t, int16_t h) {
        if (h > 7) {
            RenderSystem::sprite(x, y + 1, pickup_hackingkit[0], pickup_hackingkit[0][2]);
            drawNumber(GameVariables::hackingKits(), x + 9, y + 2);
        }
    });

    dollarcount.draw(true, [](int16_t x, int16_t y, int16_t, int16_t h) {
        if (h > 7) {
            RenderSystem::sprite(x + 1, y + 1, pickup_dollar[0]);
            drawNumber(GameVariables::dollars(), x + 10, y + 2);
        }
    });

    keyacount.draw(true, [](int16_t x, int16_t y, int16_t, int16_t h) {
        if (h > 7) {
            RenderSystem::sprite(x, y, pickup_keycard1[0], pickup_keycard1[0][2]);
            drawNumber(GameVariables::keysA(), x + 8, y + 1);
        }
    });

    keybcount.draw(true, [](int16_t x, int16_t y, int16_t, int16_t h) {
        if (h > 7) {
            RenderSystem::sprite(x, y, pickup_keycard2[0], pickup_keycard2[0][2]);
            drawNumber(GameVariables::keysB(), x + 8, y + 1);
        }
    });

    keyccount.draw(true, [](int16_t x, int16_t y, int16_t, int16_t h) {
        if (h > 7) {
            RenderSystem::sprite(x, y, pickup_keycard3[0], pickup_keycard3[0][2]);
            drawNumber(GameVariables::keysC(), x + 8, y + 1);
        }
    });
}

