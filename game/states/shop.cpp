#include "shop.h"

#include "core//controls.h"
#include "game/entities/pois.h"
#include "game/states/game.h"
#include "game/ui/ui.h"
#include "core//rendersystem.h"
#include "game/states/repairs.h"

static UIElement title = UIElement::getExpander(55, 28, 70, 9, Tween::Easing::OutQuad);
static UIOptions title_opts(true, {"LEAVE", "SAVE", "REPAIR/BUILD"});

void showShop(bool from_repairs)
{
    FSM::instance->go(GameStates::ShowShop);

    UI::hideHealthbar();
    UI::setVisibility(UI::Element::UIDollarCount, false);
    UI::setVisibility(UI::Element::UIKeyACount, false);
    UI::setVisibility(UI::Element::UIKeyBCount, false);
    UI::setVisibility(UI::Element::UIKeyCCount, false);

    UI::setVisibility(UI::Element::UIHackingKitCount, false);

    title.setVisibility(true, uint32_t(50));
    if (!from_repairs) {
        title_opts.reset();
    }
}

void quitShopState() {
    goGame();
    title.setVisibility(false);
}

void goRepairState() {
    title.setVisibility(false);
    showRepairs();
}

void updateShopState(FSM&)
{
    ControlStatus status = Controls::getStatus();

    title_opts.update(status);

    if (status.a.pressed()) {
        switch(title_opts.activeIndex()) {
        case 0:
            quitShopState();
            break;
        case 1:
            // save & show UI save prompt
            break;
        case 2:
            goRepairState();
            break;
        default:
            break;
        }
    } else if (status.b.pressed()) {
        quitShopState();
    }
}

void drawShopState()
{
    drawGameState();
    title.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h) {
        if (h > 8) {
            Helpers::printHorizontallyCentered(x + w/2, y + 2, "HACKED BASE", 10);
            title_opts.foreach([&](uint8_t idx, bool active, const char * text) {
                Helpers::drawNotchedRect(x + 9, y + 10 + idx * 8, w - 9, 7, 0);
                RenderSystem::sprite(x, y + 10 + idx * 8, poi[active ? 1 : 0]);
                RenderSystem::print(x + 12, y + 11 + idx * 8, text, active ? 10 : 6);
            });

        }
    });
}
