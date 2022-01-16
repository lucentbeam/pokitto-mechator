#include "blueprints.h"

#include "core//controls.h"
#include "game/entities/pois.h"
#include "game/states/game.h"
#include "core//rendersystem.h"

#include "game/states/shop.h"

#include "core/utilities/tween.h"
#include "game/constants.h"

#include "game/ui/ui.h"

#include "game/variables.h"
#include "core/audiosystem.h"

static UIElement title = UIElement::getExpander(59, 35, 88, 9, Tween::Easing::OutQuad);
//static UIElement alert_box = UIElement::getExpander(55, 45, 76, 9, Tween::Easing::OutQuad);
static UIOptions options(true, {"NO", "YES"});
std::vector<int> bps_avail;

void showBlueprintsShop()
{
    FSM::instance->go(GameStates::ShowBlueprintShop);

    UI::hideHealthbar();
    UI::setVisibility(UI::Element::UIHackingKitCount, false, uint32_t(0));
    UI::setVisibility(UI::Element::UIKeyACount, false);
    UI::setVisibility(UI::Element::UIKeyBCount, false);
    UI::setVisibility(UI::Element::UIKeyCCount, false);
    UI::setVisibility(UI::Element::UIDollarCount, true);

    std::vector<const char *> opts;
    bps_avail.clear();
    for(int i = 0; i < int(Blueprints::LastIdxBP); ++i) {
        if (GameVariables::hasBlueprintToUnlock(Blueprints(i))) {
            opts.push_back(bp_names[i]);
            bps_avail.push_back(i);
        }
    }
    options = UIOptions(true, opts);
    options.reset();
    int v = 8 * opts.size() + 10;
    title.setCenter(59, 44 - v / 2);
    title.setVisibility(true, uint32_t(0));
}

void updateBlueprintsShopState(FSM &fsm)
{
    ControlStatus status = Controls::getStatus();
    options.update(status);

    // update cost pill

    if (status.a.pressed()) {
        GameVariables::unlockBlueprint(bps_avail[options.activeIndex()]);
        AudioSystem::play(sfxGetItem);
        showShop(true);
        return;
    }

    if (status.b.pressed()) {
        showShop(true);
        title.setVisibility(false, uint32_t(0));
        AudioSystem::play(sfxCancel);
    }
}

void drawBlueprintsShopState()
{

    drawShadedGame();

    title.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h) {
        if (h > 8) {
            Helpers::printHorizontallyCentered(x + w/2, y + 1, "UNLOCKABLES", 10);
            x -= 12;
            w += 15;
            options.foreach([&](uint8_t idx, bool active, const char * text) {
                Helpers::drawNotchedRect(x + 9, y + 10 + idx * 8, w - 9, 7, 0);
                RenderSystem::sprite(x, y + 10 + idx * 8, poi[!active ? 0 : 1]);

                Helpers::printHorizontallyCentered(x + w/2 + 4, y + 10 + idx * 8, text, !active ? 6 : 10);
            });

        }
    });
}
