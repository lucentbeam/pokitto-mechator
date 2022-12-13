#include "pause.h"

#include "game/ui/ui.h"
#include "game/states/game.h"
#include "core//controls.h"
#include "game/utilities/helpers.h"
#include "game/weapons.h"
#include "game/player.h"
#include "core/audiosystem.h"
#include "game/utilities/blinker.h"

static UIElement pause_prompt = UIElement::getExpander(50,15,60,11, Tween::Easing::OutQuad);
static UIElement equip_prompt = UIElement::getExpander(50,40,60,11, Tween::Easing::OutQuad);
static UIElement map_prompt = UIElement::getExpander(50,52,60,11, Tween::Easing::OutQuad);
static UIElement opt_prompt = UIElement::getExpander(50,64,60,11, Tween::Easing::OutQuad);

static UIOptions selector(3);

static Weapon::Type selected_weapon = Weapon::MachineGun;

static bool goingMap = false;

static Blinker chevron_blink(1.0f, 0.8f);

void goPause(bool from_map)
{
    UI::setVisibility(UI::Element::UIKeyACount, true, uint32_t(100));
    UI::setVisibility(UI::Element::UIKeyBCount, true, uint32_t(133));
    UI::setVisibility(UI::Element::UIKeyCCount, true, uint32_t(166));
    UI::setVisibility(UI::Element::UIHackingKitCount, true, uint32_t(200));

    UI::setVisibility(UI::Element::UIDollarCount,     true, uint32_t(300));

    UI::setVisibility(UI::Element::GameSavedPrompt, false, true);    

    pause_prompt.setVisibility(true, uint32_t(0));
    equip_prompt.setVisibility(true, uint32_t(50));
    map_prompt.setVisibility(true, uint32_t(100));
    opt_prompt.setVisibility(true, uint32_t(150));
    if (from_map) UI::showHealthbar();
    else selector.setSelection(0);

    selected_weapon = Player::currentWeapon();
    FSM::instance->go(GameStates::Pause);

    if (!from_map) AudioSystem::play(sfxConfirm);

    goingMap = false;
}

void updatePauseState(FSM& fsm)
{
    if (goingMap && pause_prompt.hidden()) {
        FSM::instance->go(GameStates::MapState);
    }
    if (goingMap) return;
    UI::update(physicsTimestep);
    chevron_blink.update();
    ControlStatus status = Controls::getStatus();
    if (status.c.pressed() || status.b.pressed()) {
        pause_prompt.setVisibility(false);
        equip_prompt.setVisibility(false);
        map_prompt.setVisibility(false);
        opt_prompt.setVisibility(false);
        AudioSystem::play(sfxCancel);
        goGame();
        return;
    }
    if (status.a.pressed() && selector.activeIndex() == 1) {
        UI::hideHealthbar();
        pause_prompt.setVisibility(false);
        equip_prompt.setVisibility(false);
        map_prompt.setVisibility(false);
        opt_prompt.setVisibility(false);
        goingMap = true;
        UI::setVisibility(UI::Element::UIKeyACount, false);
        UI::setVisibility(UI::Element::UIKeyBCount, false);
        UI::setVisibility(UI::Element::UIKeyCCount, false);
        UI::setVisibility(UI::Element::UIDollarCount, false);
        UI::setVisibility(UI::Element::UIHackingKitCount, false);
        AudioSystem::play(sfxConfirm);
    }
    if (status.a.pressed() && selector.activeIndex() == 2) {
        fsm.go(GameOptionsState);
        AudioSystem::play(sfxConfirm);
        return;
    }
    selector.update(status.down.pressed(), status.up.pressed());
    if (selector.activeIndex() == 0) {
        if (status.right.pressed()) {
            Player::cycleWeaponNext();
            selected_weapon = Player::currentWeapon();
            AudioSystem::play(sfxSelect);
        } else if (status.left.pressed()) {
            Player::cycleWeaponPrev();
            selected_weapon = Player::currentWeapon();
            AudioSystem::play(sfxSelect);
        }
    }
}

void drawPauseState()
{
    drawShadedGame();
    pause_prompt.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h) {
        if (h > 10) {
            Helpers::printHorizontallyCentered(x + w/2, y + 2, "GAME PAUSED", 10);//chevron_blink.active(0.4f) ? 10 : 6);
        }
    });
    equip_prompt.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h) {
        if (h > 10) {
            Helpers::printHorizontallyCentered(x + w/2, y + 2, Weapon::getName(selected_weapon).c_str(), selector.activeIndex() == 0 ? 10 : 6);
            if (selector.activeIndex() == 0) {
                if (chevron_blink.active()) {
                    RenderSystem::sprite(x - 5, y + 3, ui_arrow_left, ui_arrow_left[2]);
                    RenderSystem::sprite(x + w + 1, y + 3, ui_arrow_left, ui_arrow_left[2], true);
                }

                WeaponHelper carousel = Player::getCurrentWeaponInfo();
                int spacing = 4;
                x = x + w/2 - ((carousel.count-1) * spacing + carousel.count * 6) / 2;
                y -= 18;
                Helpers::drawNotchedRect(x - spacing, y + h, (spacing + 6) * carousel.count + spacing, 6, 2);
                for (int i = 0; i < carousel.count; ++i) {
                    RenderSystem::sprite(x + i * (spacing + 6), y + h, bullet_empty, 1);
                    if (i == carousel.index) {
                        RenderSystem::drawRect2(x + i * (spacing + 6) + 2, y + h + 2, 2, 2, 10);
                    }
                }
            }

        }
    });

    map_prompt.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h) {
        if (h > 10) {
            Helpers::printHorizontallyCentered(x + w/2, y + 2, "Map", selector.activeIndex() == 1 ? 10 : 6);
            if (selector.activeIndex() == 1) {
                if (chevron_blink.active()) RenderSystem::sprite(x - 3, y + 3, ui_arrow_left, ui_arrow_left[2], true);
//                RenderSystem::sprite(x + w + 2, y + 3, ui_arrow_left, ui_arrow_left[2], false);
            }
        }
    });


    opt_prompt.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h) {
        if (h > 10) {
            Helpers::printHorizontallyCentered(x + w/2, y + 2, "Options", selector.activeIndex() == 2 ? 10 : 6);
            if (selector.activeIndex() == 2) {
                if (chevron_blink.active()) RenderSystem::sprite(x - 3, y + 3, ui_arrow_left, ui_arrow_left[2], true);
//                RenderSystem::sprite(x + w + 2, y + 3, ui_arrow_left, ui_arrow_left[2], false);
            }
        }
    });
}
