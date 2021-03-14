#include "repairs.h"

#include "game/states/game.h"
#include "game/ui/ui.h"
#include "core/rendering/rendersystem.h"
#include "core/controls/controls.h"
#include "game/enums.h"
#include "game/states/shop.h"
#include "game/utilities/helpers.h"
#include "game/tilesets.h"
#include "game/player.h"

static UIElement title = UIElement::getExpander(55, 35, 76, 9, Tween::Easing::OutQuad);
static UIOptions repair_opts(true, {"BACK", "SOLDIER (REPAIR)", "JEEP", "TANK", "BOAT", "HELI"});

void showRepairs()
{
    FSM::instance->go(GameStates::ShowRepairs);
    UI::setVisibility(UI::Element::UIDollarCount, true, uint32_t(100));

    UI::setVisibility(UI::Element::UIHealthbar, false);
    UI::setVisibility(UI::Element::UIKeyACount, false);
    UI::setVisibility(UI::Element::UIKeyBCount, false);
    UI::setVisibility(UI::Element::UIKeyCCount, false);
    UI::setVisibility(UI::Element::UIHackingKitCount, false);

    title.setVisibility(true, uint32_t(50));
    repair_opts.reset();
    repair_opts.setAvailableCount(2 + Player::unlockLevel());
}

void updateRepairsState(FSM &fsm)
{
    ControlStatus status = Controls::getStatus();

    repair_opts.update(status);

    if (status.a.pressed()) {
        switch(repair_opts.activeIndex()) {
        case 0:
            showShop(true);
            break;
        case 1:
            // repair soldier
            break;
        case 2:
            // build/repair jeep
            break;
        default:
            break;
        }
    } else if (status.b.pressed()) {
        showShop(true);
    }
}

void drawRepairsState()
{
    drawGameState();
    title.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h) {
        if (h > 8) {
            Helpers::printHorizontallyCentered(x + w/2, y + 2, "WORKSHOP", 41);
            repair_opts.foreach([&](uint8_t idx, bool active, const char * text) {
                std::string line = text;
                if (idx > 1) {
                    line += " (BUILD)";
                }
                Helpers::drawNotchedRect(x + 9, y + 10 + idx * 8, w - 9, 7, 0);
                RenderSystem::sprite(x, y + 10 + idx * 8, poi[active ? 1 : 0]);
                RenderSystem::print(x + 12, y + 11 + idx * 8, line.c_str(), active ? 41 : 25);
            });

        }
    }, 0, Player::unlockLevel() * -9);
}
