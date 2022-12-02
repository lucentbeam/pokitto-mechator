#include "ui.h"

#include "game/sprites.h"
#include "game/player.h"
#include "game/constants.h"
#include "game/sprites.h"
#include "game/variables.h"
#include "game/funcs.h"

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
    if (showing) m_showDuration--;
    if (m_showDuration == 0 && showing) {
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

void UIElement::setCenter(float x, float y)
{
    m_x = x - m_w/2;
    m_y = y - m_h/2;
    m_xh = x;
    m_yh = y;
}

UIElement UIElement::getExpander(int16_t x, int16_t y, int16_t w, int16_t h, Tween::Easing curve)
{
    return UIElement(x-w/2,y-h/2,w,h,x,y,0,0,curve);
}

void UIOptions::update(bool forward, bool back, void (*on_highlight)(int8_t index), bool cycle)
{
    if (!forward && !back) return;
    int8_t next = m_active_index;
    if (forward && next < (m_available - 1)) {
        ++next;
        while (!m_active[next] && next < m_available) {
            ++next;
            if (cycle) next %= m_available;
        }
    } else if (back && next > 0) {
        --next;
        while (!m_active[next] && (cycle || next > 0)) {
            --next;
            if (cycle && next < 0) {
                next += m_available;
            }
        }
    }
    if (!m_active[next]) return;
    if (next != m_active_index) {
        m_active_index = next;
        if (on_highlight != nullptr) on_highlight(m_active_index);
        AudioSystem::play(sfxSelect);
    }
}

void UIOptions::foreach(std::function<void (uint8_t, bool)> callback)
{
    int idx = 0;
    for(int i = 0; i < m_available; ++i) {
        if (m_active[i]) {
            callback(idx, i == m_active_index);
            ++idx;
        }
    }
}

void UIOptions::foreach(std::function<void (uint8_t, uint8_t, bool)> callback)
{
    int idx = 0;
    for(int i = 0; i < m_available; ++i) {
        if (m_active[i]) {
            callback(idx, i, i == m_active_index);
            ++idx;
        }
    }
}

static UIElement soldier_healthbar(0,0,7,88,-7,0,7,88,Tween::Easing::OutQuad);
static UIElement jeep_healthbar(0,0,7,88,-7,0,7,88,Tween::Easing::OutQuad);
static UIElement tank_healthbar(0,0,7,88,-7,0,7,88,Tween::Easing::OutQuad);
static UIElement boat_healthbar(0,0,7,88,-7,0,7,88,Tween::Easing::OutQuad);
static UIElement heli_healthbar(0,0,7,88,-7,0,7,88,Tween::Easing::OutQuad);

static UIElement boss_healthbar(104,0,7,88,110,0,7,88,Tween::Easing::OutQuad);

static UIElement keyacount(90,25,19,8,100,29,0,0,Tween::Easing::OutQuad);
static UIElement keybcount(90,34,19,8,100,38,0,0,Tween::Easing::OutQuad);
static UIElement keyccount(90,43,19,8,100,47,0,0,Tween::Easing::OutQuad);
static UIElement kitcount(90,52,19,8,100,56,0,0,Tween::Easing::OutQuad);

static UIElement dollarcount(86,78,23,9,97,82,0,0,Tween::Easing::OutQuad);

static UIElement savedprompt(40, 77, 30, 9, 55, 81, 0, 0, Tween::Easing::OutQuad);

int8_t * UI::m_boss_life = nullptr;
int8_t UI::m_max_boss_life = 0;

void UI::drawNumber(uint16_t num, int x, int y)
{
    char buf[5];
    sprintf(buf, "%d", num);
    if (num < 10) {
        x += 1;
    }
    Helpers::printHorizontallyCentered(x, y, buf, num == 0 ? 6 : 10);
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
    case UI::Element::GameSavedPrompt:
        savedprompt.setVisibility(visible,immediate);
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
    case UI::Element::GameSavedPrompt:
        savedprompt.setVisibility(visible,delay);
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
    case UI::Element::GameSavedPrompt:
        savedprompt.showForDuration(duration);
        break;
    }
}

void UI::showHealthbar(PlayerMode mode)
{
    UIElement * bars[] = {&soldier_healthbar, &jeep_healthbar, &tank_healthbar, &boat_healthbar, &heli_healthbar};
    for(int i = 0; i < 5; ++i) {
        if (int(mode) == i) {
            bars[i]->setVisibility(true, uint32_t(uiEasingTimeMs));
        } else {
            bars[i]->setVisibility(false);
        }
    }
}

void UI::showHealthbar()
{
    showHealthbar(Player::mode());
    if (m_boss_life != nullptr) boss_healthbar.setVisibility(true);
}

void UI::hideHealthbar()
{
    soldier_healthbar.setVisibility(false);
    jeep_healthbar.setVisibility(false);
    tank_healthbar.setVisibility(false);
    boat_healthbar.setVisibility(false);
    heli_healthbar.setVisibility(false);
    boss_healthbar.setVisibility(false);
}

void UI::showBoss(int8_t *life_ref)
{
    m_boss_life = life_ref;
    m_max_boss_life = *m_boss_life;
}

void UI::clearBoss()
{
    m_boss_life = 0;
    m_max_boss_life = 0;
}

void UI::update(float dt)
{
    kitcount.update(dt);
    dollarcount.update(dt);
    keyacount.update(dt);
    keybcount.update(dt);
    keyccount.update(dt);
    savedprompt.update(dt);
    if (m_boss_life != nullptr && *m_boss_life <= 0) {
        m_boss_life = nullptr;
        onBossDestroyed();
    }
}

static PlayerMode activeDrawMode;

void drawHealthBar(int16_t x, int16_t, int16_t, int16_t) {
    uint8_t current = Player::health(activeDrawMode).value();
    uint8_t max = Player::health(activeDrawMode).max();

    const uint8_t flash[] = {3, 2, 10, 10, 10, 10, 10, 10};
    bool hurt = Player::hurting();
    for(uint8_t i = 0; i < max; i++) {
        int idx = i < current ? int(activeDrawMode) + 1 : 0;
        RenderSystem::sprite(2 + x, 84 - i * 3, hurt ? flash : health_pips[idx]);
    }
}

void UI::draw()
{
    activeDrawMode = SoldierMode;
    soldier_healthbar.draw(false, drawHealthBar);
    activeDrawMode = JeepMode;
    jeep_healthbar.draw(false, drawHealthBar);
    activeDrawMode = TankMode;
    tank_healthbar.draw(false, drawHealthBar);
    activeDrawMode = BoatMode;
    boat_healthbar.draw(false, drawHealthBar);
    activeDrawMode = HelicopterMode;
    heli_healthbar.draw(false, drawHealthBar);

//    if (Player::mode() == HelicopterMode || Player::mode() == BoatMode) {
    if (FSM::instance->is(Game) && GameVariables::eventVisited(ExitTutorial)) {
        constexpr int sz = 16;
        Helpers::drawNotchedRect(109-sz-2, 1, sz+2, sz+2, 0);
        Vec2f ppos = Player::position() * Vec2f(54.0f, 56.0f) / 216.0f / 6.0f + Vec2f(11.0f, -2.0f);
        Helpers::drawRLE(108-sz, 2, mechator_reduced, -1, -1, nullptr, ppos.x() - sz/2, ppos.y() - sz/2, sz, sz);
        RenderSystem::pixel(109 - sz/2, 2 + sz/2, 10);
    }

    if (m_boss_life != nullptr) {
        boss_healthbar.draw(false, [](int16_t x, int16_t, int16_t, int16_t) {
            for(uint8_t i = 0; i < m_max_boss_life; i++) {
                int idx = i < *m_boss_life ? 3 : 0;
                RenderSystem::sprite(1 + x, 84 - i * 3, health_pips[idx]);
            }
        });
    }


    kitcount.draw(true, [](int16_t x, int16_t y, int16_t, int16_t h) {
        if (h > 7) {
            RenderSystem::sprite(x, y, pickup_hackingkit[0], pickup_hackingkit[0][2]);
            drawNumber(GameVariables::hackingKits(), x + 11, y+1);
        }
    });

    dollarcount.draw(true, [](int16_t x, int16_t y, int16_t, int16_t h) {
        if (h > 7) {
            RenderSystem::sprite(x + 1, y + 1, pickup_dollar[0]);
            drawNumber(GameVariables::dollars(), x + 15, y+1);
        }
    });

    keyacount.draw(true, [](int16_t x, int16_t y, int16_t, int16_t h) {
        if (h > 7) {
            RenderSystem::sprite(x+1, y, pickup_keycard1[1], pickup_keycard1[1][2]);
            drawNumber(GameVariables::keysA(), x + 11, y+1);
        }
    });

    keybcount.draw(true, [](int16_t x, int16_t y, int16_t, int16_t h) {
        if (h > 7) {
            RenderSystem::sprite(x+1, y, pickup_keycard2[1], pickup_keycard2[1][2]);
            drawNumber(GameVariables::keysB(), x + 11, y+1);
        }
    });

    keyccount.draw(true, [](int16_t x, int16_t y, int16_t, int16_t h) {
        if (h > 7) {
            RenderSystem::sprite(x+1, y, pickup_keycard3[1], pickup_keycard3[1][2]);
            drawNumber(GameVariables::keysC(), x + 11, y+1);
        }
    });

    savedprompt.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h) {
        if (h > 7) {
            Helpers::printHorizontallyCentered(x + w/2, y + 1, "SAVED", 10);
        }
    });

    if (FSM::instance->is(GameStates::Game)) {
        ControlStatus ctrl = Controls::getStatus();
        if (Player::mode() == PlayerMode::SoldierMode && Soldier::isSprinting()) {
            Helpers::drawNotchedRect(8, 88-1-9, 27, 9, 0);
            RenderSystem::print(10, 88-1-8, "sprint", 10);
        } else if (ctrl.a.held()) {
            Helpers::drawNotchedRect(8, 88-1-9, 22, 9, 0);
            RenderSystem::sprite(10, 80, ui_lock, 0);
            RenderSystem::print(16, 88-1-8, "aim", 10);
        }
    }
}
