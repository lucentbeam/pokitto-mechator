#include "pause.h"

#include "game/ui/ui.h"
#include "game/states/game.h"
#include "core//controls.h"
#include "game/utilities/helpers.h"
#include "game/weapons.h"
#include "game/player.h"
#include "core/audiosystem.h"

static UIElement pause_prompt = UIElement::getExpander(53,35,60,11, Tween::Easing::OutQuad);
static UIElement equip_prompt = UIElement::getExpander(53,47,60,11, Tween::Easing::OutQuad);
static UIElement map_prompt = UIElement::getExpander(53,64,60,11, Tween::Easing::OutQuad);

Weapon::Type selected_weapon = Weapon::MachineGun;

static int selection = 0;

void goPause()
{
    UI::setVisibility(UI::Element::UIKeyACount, true, uint32_t(150));
    UI::setVisibility(UI::Element::UIKeyBCount, true, uint32_t(150));
    UI::setVisibility(UI::Element::UIKeyCCount, true, uint32_t(150));
    UI::setVisibility(UI::Element::UIDollarCount,     true, uint32_t(300));
    UI::setVisibility(UI::Element::UIHackingKitCount, true, uint32_t(300));
    pause_prompt.setVisibility(true, uint32_t(0));
    equip_prompt.setVisibility(true, uint32_t(0));
    map_prompt.setVisibility(true, uint32_t(0));

    selection = 0;
    selected_weapon = Player::currentWeapon();
    FSM::instance->go(GameStates::Pause);

    AudioSystem::play(sfxConfirm);
}

void updatePauseState(FSM&)
{
    ControlStatus status = Controls::getStatus();
    if (status.c.pressed() || status.b.pressed()) {
        pause_prompt.setVisibility(false);
        equip_prompt.setVisibility(false);
        map_prompt.setVisibility(false);
        AudioSystem::play(sfxCancel);
        goGame();
        return;
    }
    if (status.down.pressed() && selection == 0) {
        selection = 1;
        AudioSystem::play(sfxSelect);
    } else if (status.up.pressed() && selection == 1) {
        selection = 0;
        AudioSystem::play(sfxSelect);
    }
    if (selection == 0) {
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
        static int counter = 0;
        counter++;
        if (h > 10) {
            Helpers::printHorizontallyCentered(x + w/2, y + 2, "GAME PAUSED", (counter % 90) < 78 ? 10 : 6);
        }
    });
    equip_prompt.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h) {
        if (h > 10) {
            Helpers::printHorizontallyCentered(x + w/2, y + 2, Weapon::getName(selected_weapon).c_str(), selection == 0 ? 10 : 8);
            if (selection == 0) {
                RenderSystem::sprite(x - 5, y + 3, ui_arrow_left, ui_arrow_left[2]);
                RenderSystem::sprite(x + w + 1, y + 3, ui_arrow_left, ui_arrow_left[2], true);
            }

            WeaponHelper carousel = Player::getCurrentWeaponInfo();
            int spacing = 2;
            x = x + w/2 - ((carousel.count-1) * spacing + carousel.count * 4) / 2;
            const uint8_t bullet_empty[] = {4, 4, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1};
            for (int i = 0; i < carousel.count; ++i) {
                RenderSystem::sprite(x + i * (spacing + 4), y + h + 1, bullet_empty, 1);
                if (i == carousel.index) {
                    RenderSystem::drawRect2(x + i * (spacing + 4) + 1, y + h + 2, 2, 2, 10);
                }
            }
        }
    });

    map_prompt.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h) {
        if (h > 10) {
            Helpers::printHorizontallyCentered(x + w/2, y + 2, "Map", selection == 1 ? 10 : 8);
            if (selection == 1) {
                RenderSystem::sprite(x - 3, y + 3, ui_arrow_left, ui_arrow_left[2], true);
//                RenderSystem::sprite(x + w + 2, y + 3, ui_arrow_left, ui_arrow_left[2], false);
            }
        }
    });
}
