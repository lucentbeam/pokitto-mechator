#include "ui.h"

#include "game/sprites.h"
#include "game/player.h"
#include "game/constants.h"
#include "game/sprites.h"
#include "game/variables.h"

#include "core/audiosystem.h"
#include "core/utilities/babyfsm.h"

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
    m_showDuration = 0;
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
    m_showDuration = 0;
    if (vis == visible) {
        return;
    }
    visible = vis;
    tween.reset(delay);
}

void UIElement::update(float dt)
{
    bool showing = m_showDuration > 0;
    m_showDuration -= dt;
    if (m_showDuration < 0 && showing) {
        setVisibility(false);
    }
}

//void UIElement::draw(bool useNotched, void (*callback)(int16_t, int16_t, int16_t, int16_t) = nullptr, int dx, int dy)
//{
//    if (!visible && tween.getInterpolation(0, 1.0) > 0.999f) {
//        return;
//    }
//    int16_t x = visible ? int16_t(tween.getInterpolationInt(m_xh, m_x)) : int16_t(tween.getInterpolationInt(m_x, m_xh));
//    x += dx;
//    int16_t y = visible ? int16_t(tween.getInterpolationInt(m_yh, m_y)) : int16_t(tween.getInterpolationInt(m_y, m_yh));
//    y += dy;
//    int16_t w = visible ? int16_t(tween.getInterpolationInt(m_wh, m_w)) : int16_t(tween.getInterpolationInt(m_w, m_wh));
//    int16_t h = visible ? int16_t(tween.getInterpolationInt(m_hh, m_h)) : int16_t(tween.getInterpolationInt(m_h, m_hh));

//    if (w <= 0 || h <= 0) { return; }

//    if (useNotched) {
//        Helpers::drawNotchedRect(x, y, uint8_t(w), uint8_t(h), 0);
//    } else {
//        RenderSystem::drawRect(x, y, w, h, 0);
//    }

//    if (callback != nullptr) {
//        callback(x,y,w,h);
//    }
//}

void UIElement::draw(bool notched, std::function<void (int16_t, int16_t, int16_t, int16_t)> callback, int dx, int dy)
{
    if (!visible && tween.getInterpolation(0, 1.0) > 0.999f) {
        return;
    }
    int16_t x = visible ? int16_t(tween.getInterpolationInt(m_xh, m_x)) : int16_t(tween.getInterpolationInt(m_x, m_xh));
    x += dx;
    int16_t y = visible ? int16_t(tween.getInterpolationInt(m_yh, m_y)) : int16_t(tween.getInterpolationInt(m_y, m_yh));
    y += dy;
    int16_t w = visible ? int16_t(tween.getInterpolationInt(m_wh, m_w)) : int16_t(tween.getInterpolationInt(m_w, m_wh));
    int16_t h = visible ? int16_t(tween.getInterpolationInt(m_hh, m_h)) : int16_t(tween.getInterpolationInt(m_h, m_hh));

    if (w <= 0 || h <= 0) { return; }

    if (notched) {
        Helpers::drawNotchedRect(x, y, uint8_t(w), uint8_t(h), 0);
    } else {
        RenderSystem::drawRect(x, y, w, h, 0);
    }

    if (callback != nullptr) {
        callback(x,y,w,h);
    }
}

void UIElement::setMaxWidth(int w)
{
    m_x += m_w / 2;
    m_w = w;
    m_x -= m_w / 2;
}

UIElement UIElement::getExpander(int16_t x, int16_t y, int16_t w, int16_t h, Tween::Easing curve)
{
    return UIElement(x-w/2,y-h/2,w,h,x,y,0,0,curve);
}

UIOptions::UIOptions(bool vertical, std::initializer_list<const char *> options) :
    m_vertical(vertical),
    m_options(options),
    m_available(uint8_t(options.size()))
{

}

void UIOptions::update(const ControlStatus &status, void (*on_highlight)(int8_t index))
{
    int8_t previous = m_active_index;
    if (m_vertical) {
        if (status.up.pressed() || status.down.pressed()) {
            m_active_index += status.y < 0 ? -1 : 1;
            AudioSystem::play(sfxSelect);
        }
    } else {
        if (status.right.pressed() || status.left.pressed()) {
            m_active_index += status.x < 0 ? -1 : 1;
            AudioSystem::play(sfxSelect);
        }
    }
    if (m_active_index < 0) {
        m_active_index += m_available;
    } else {
        m_active_index %= m_available;
    }
    if (on_highlight != nullptr && m_active_index != previous) {
        on_highlight(m_active_index);
    }
}

void UIOptions::foreach(std::function<void (uint8_t, bool, const char *)> callback)
{
    uint8_t i = 0;
    for(const char * s : m_options) {
        if (i < m_available) {
            callback(i, i == m_active_index, s);
        }
        i++;
    }
}

static UIElement soldier_healthbar(0,0,7,88,-7,0,7,88,Tween::Easing::OutQuad);
static UIElement jeep_healthbar(0,0,7,88,-7,0,7,88,Tween::Easing::OutQuad);
static UIElement tank_healthbar(0,0,7,88,-7,0,7,88,Tween::Easing::OutQuad);
static UIElement boat_healthbar(0,0,7,88,-7,0,7,88,Tween::Easing::OutQuad);
static UIElement heli_healthbar(0,0,7,88,-7,0,7,88,Tween::Easing::OutQuad);

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
    RenderSystem::print(x, y, buf, num == 0 ? 6 : 10);
}

void UI::setVisibility(UI::Element element, bool visible, bool immediate)
{
    switch (element) {
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

void UI::showForDuration(UI::Element element, float duration)
{
    switch (element) {
    case UI::Element::UIHackingKitCount:
        kitcount.showForDuration(duration);
        break;
    case UI::Element::UIDollarCount:
        dollarcount.showForDuration(duration);
        break;
    case UI::Element::UIKeyACount:
        keyacount.showForDuration(duration);
        break;
    case UI::Element::UIKeyBCount:
        keybcount.showForDuration(duration);
        break;
    case UI::Element::UIKeyCCount:
        keyccount.showForDuration(duration);
        break;
    default:
        break;
    }
}

void UI::showHealthbar(PlayerMode mode)
{
    switch(mode) {
    case PlayerMode::SoldierMode:
        soldier_healthbar.setVisibility(true, uint32_t(uiEasingTimeMs));

        jeep_healthbar.setVisibility(false);
        tank_healthbar.setVisibility(false);
        boat_healthbar.setVisibility(false);
        heli_healthbar.setVisibility(false);
        break;
    case PlayerMode::JeepMode:
        jeep_healthbar.setVisibility(true, uint32_t(uiEasingTimeMs));

        soldier_healthbar.setVisibility(false);
        tank_healthbar.setVisibility(false);
        boat_healthbar.setVisibility(false);
        heli_healthbar.setVisibility(false);
        break;
    case PlayerMode::TankMode:
        tank_healthbar.setVisibility(true, uint32_t(uiEasingTimeMs));

        soldier_healthbar.setVisibility(false);
        jeep_healthbar.setVisibility(false);
        boat_healthbar.setVisibility(false);
        heli_healthbar.setVisibility(false);
        break;
    case PlayerMode::BoatMode:
        boat_healthbar.setVisibility(true, uint32_t(uiEasingTimeMs));

        soldier_healthbar.setVisibility(false);
        jeep_healthbar.setVisibility(false);
        tank_healthbar.setVisibility(false);
        heli_healthbar.setVisibility(false);
        break;
    case PlayerMode::HelicopterMode:
        heli_healthbar.setVisibility(true, uint32_t(uiEasingTimeMs));

        soldier_healthbar.setVisibility(false);
        jeep_healthbar.setVisibility(false);
        tank_healthbar.setVisibility(false);
        boat_healthbar.setVisibility(false);
        break;
    }
}

void UI::showHealthbar()
{
    showHealthbar(Player::mode());
}

void UI::hideHealthbar()
{
    soldier_healthbar.setVisibility(false);
    jeep_healthbar.setVisibility(false);
    tank_healthbar.setVisibility(false);
    boat_healthbar.setVisibility(false);
    heli_healthbar.setVisibility(false);
}

void UI::update(float dt)
{
    kitcount.update(dt);
    dollarcount.update(dt);
    keyacount.update(dt);
    keybcount.update(dt);
    keyccount.update(dt);
}

void UI::draw()
{
    soldier_healthbar.draw(false, [](int16_t x, int16_t, int16_t, int16_t) {
        uint8_t current = Soldier::health().value();
        uint8_t max = Soldier::health().max();

        const uint8_t flash[] = {3, 2, 10, 10, 10, 10, 10, 10};
        bool hurt = Player::hurting();
        for(uint8_t i = 0; i < max; i++) {
            int idx = i < current ? 1 : 0;
            RenderSystem::sprite(2 + x, 84 - i * 3, hurt ? flash : health_pips[idx]);
        }
    });

    jeep_healthbar.draw(false, [](int16_t x, int16_t, int16_t, int16_t) {
        uint8_t current = Jeep::health().value();
        uint8_t max = Jeep::health().max();

        const uint8_t flash[] = {3, 2, 10, 10, 10, 10, 10, 10};
        bool hurt = Player::hurting();
        for(uint8_t i = 0; i < max; i++) {
            int idx = i < current ? 2: 0;
            RenderSystem::sprite(2 + x, 84 - i * 3, hurt ? flash : health_pips[idx]);
        }
    });

    tank_healthbar.draw(false, [](int16_t x, int16_t, int16_t, int16_t) {
        uint8_t current = Tank::health().value();
        uint8_t max = Tank::health().max();

        const uint8_t flash[] = {3, 2, 10, 10, 10, 10, 10, 10};
        bool hurt = Player::hurting();
        for(uint8_t i = 0; i < max; i++) {
            int idx = i < current ? 3 : 0;
            RenderSystem::sprite(2 + x, 84 - i * 3, hurt ? flash : health_pips[idx]);
        }
    });

    boat_healthbar.draw(false, [](int16_t x, int16_t, int16_t, int16_t) {
        uint8_t current = Boat::health().value();
        uint8_t max = Boat::health().max();

        const uint8_t flash[] = {3, 2, 10, 10, 10, 10, 10, 10};
        bool hurt = Player::hurting();
        for(uint8_t i = 0; i < max; i++) {
            int idx = i < current ? 4 : 0;
            RenderSystem::sprite(2 + x, 84 - i * 3, hurt ? flash : health_pips[idx]);
        }
    });

    heli_healthbar.draw(false, [](int16_t x, int16_t, int16_t, int16_t) {
        uint8_t current = Helicopter::health().value();
        uint8_t max = Helicopter::health().max();

        const uint8_t flash[] = {3, 2, 10, 10, 10, 10, 10, 10};
        bool hurt = Player::hurting();
        for(uint8_t i = 0; i < max; i++) {
            int idx = i < current ? 5 : 0;
            RenderSystem::sprite(2 + x, 84 - i * 3, hurt ? flash : health_pips[idx]);
        }
    });

    kitcount.draw(true, [](int16_t x, int16_t y, int16_t, int16_t h) {
        if (h > 7) {
            RenderSystem::sprite(x, y + 1, pickup_hackingkit[0], pickup_hackingkit[0][2]);
            drawNumber(GameVariables::hackingKits(), x + 9, y+1);
        }
    });

    dollarcount.draw(true, [](int16_t x, int16_t y, int16_t, int16_t h) {
        if (h > 7) {
            RenderSystem::sprite(x + 1, y + 1, pickup_dollar[0]);
            drawNumber(GameVariables::dollars(), x + 10, y+1);
        }
    });

    keyacount.draw(true, [](int16_t x, int16_t y, int16_t, int16_t h) {
        if (h > 7) {
            RenderSystem::sprite(x, y, pickup_keycard1[1], pickup_keycard1[1][2]);
            drawNumber(GameVariables::keysA(), x + 8, y+1);
        }
    });

    keybcount.draw(true, [](int16_t x, int16_t y, int16_t, int16_t h) {
        if (h > 7) {
            RenderSystem::sprite(x, y, pickup_keycard2[1], pickup_keycard2[1][2]);
            drawNumber(GameVariables::keysB(), x + 8, y+1);
        }
    });

    keyccount.draw(true, [](int16_t x, int16_t y, int16_t, int16_t h) {
        if (h > 7) {
            RenderSystem::sprite(x, y, pickup_keycard3[1], pickup_keycard3[1][2]);
            drawNumber(GameVariables::keysC(), x + 8, y+1);
        }
    });

    if (Player::mode() == PlayerMode::SoldierMode) {
        if (Soldier::sprintCooldown() > 0.05f) drawProgressBar(Soldier::sprintCooldown());
        if (!FSM::instance->is(GameStates::Pause) && Soldier::overlaps()) drawBPrompt();
    }
}

void UI::drawProgressBar(float fraction)
{
    Helpers::drawNotchedRect(9, 83, 22, 4, 0);
    RenderSystem::drawRect(10, 84, 20, 2, 2);
    int width = int(fraction * 20.0f);
    RenderSystem::drawRect(10, 84, width, 1, 43);
    RenderSystem::drawLine(10, 85, 10 + width - 1, 85, 42);
}

void UI::drawBPrompt()
{
    RenderSystem::sprite(2, 2, ui_circle, ui_circle[2]);
    RenderSystem::print(4, 2, "B", 1);
}
