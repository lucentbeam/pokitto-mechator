#include "shop.h"

#include "core/controls/controls.h"
#include "game/entities/pois.h"
#include "game/states/game.h"
#include "game/ui/ui.h"
#include "core/rendering/rendersystem.h"

static UIElement title = UIElement::getExpander(55, 28, 70, 9, Tween::Easing::OutQuad);
static UIOptions title_opts(true, {"LEAVE", "SAVE", "REPAIR/BUILD"});

void showShop()
{
    FSM::instance->go(GameStates::ShowShop);
    UI::setVisibility(UI::Element::UIDollarCount, true, uint32_t(100));

    UI::setVisibility(UI::Element::UIHealthbar, false);
    UI::setVisibility(UI::Element::UIKeyACount, false);
    UI::setVisibility(UI::Element::UIKeyBCount, false);
    UI::setVisibility(UI::Element::UIKeyCCount, false);
    UI::setVisibility(UI::Element::UIHackingKitCount, false);

    title.setVisibility(true, uint32_t(50));
}

void quitShopState() {
    goGame();
    title.setVisibility(false);
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
            // show UI save prompt
            break;
        case 2:
            // go to repair sub menu
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
            Helpers::printHorizontallyCentered(x + w/2, y + 2, "HACKED BASE", 41);
            title_opts.foreach([&](uint8_t idx, bool active, const char * text) {
                Helpers::drawNotchedRect(x + 9, y + 10 + idx * 8, w - 9, 7, 0);
                RenderSystem::sprite(x, y + 10 + idx * 8, poi[active ? 1 : 0]);
                RenderSystem::print(x + 12, y + 11 + idx * 8, text, active ? 41 : 25);
            });

        }
    });
}
