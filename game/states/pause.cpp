#include "pause.h"

#include "game/ui/ui.h"
#include "game/states/game.h"
#include "core//controls.h"
#include "game/utilities/helpers.h"
#include "game/weapons.h"
#include "game/player.h"

static UIElement pause_prompt = UIElement::getExpander(53,35,60,11, Tween::Easing::OutQuad);
static UIElement equip_prompt = UIElement::getExpander(53,47,60,11, Tween::Easing::OutQuad);

Weapon::Type selected_weapon = Weapon::MachineGun;

void goPause()
{
    UI::setVisibility(UI::Element::UIKeyACount, true, uint32_t(150));
    UI::setVisibility(UI::Element::UIKeyBCount, true, uint32_t(150));
    UI::setVisibility(UI::Element::UIKeyCCount, true, uint32_t(150));
    UI::setVisibility(UI::Element::UIDollarCount,     true, uint32_t(300));
    UI::setVisibility(UI::Element::UIHackingKitCount, true, uint32_t(300));
    pause_prompt.setVisibility(true, uint32_t(0));
    equip_prompt.setVisibility(true, uint32_t(0));

    selected_weapon = Player::currentWeapon();
    FSM::instance->go(GameStates::Pause);
}

void updatePauseState(FSM&)
{
    ControlStatus status = Controls::getStatus();
    if (status.c.pressed() || status.b.pressed()) {
        pause_prompt.setVisibility(false);
        equip_prompt.setVisibility(false);
        goGame();
        return;
    }
    if (status.right.pressed()) {
        Player::cycleWeaponNext();
        selected_weapon = Player::currentWeapon();
    } else if (status.left.pressed()) {
        Player::cycleWeaponPrev();
        selected_weapon = Player::currentWeapon();
    }
}

void drawPauseState()
{
    drawShadedGame();
    pause_prompt.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h) {
        static int counter = 0;
        counter++;
        if (h > 10) {
            Helpers::printHorizontallyCentered(x + w/2, y + 2, "GAME PAUSED", (counter % 90) < 78 ? 10 : 6);
        }
    });
    equip_prompt.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h) {
        if (h > 10) {
            Helpers::printHorizontallyCentered(x + w/2, y + 2, Weapon::getName(selected_weapon).c_str(), 10);
            RenderSystem::sprite(x - 5, y + 3, ui_arrow_left, ui_arrow_left[2]);
            RenderSystem::sprite(x + w, y + 3, ui_arrow_left, ui_arrow_left[2], true);
        }
    });
}
