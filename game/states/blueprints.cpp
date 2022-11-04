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

static UIElement title = UIElement::getExpander(55, 35, 88, 9, Tween::Easing::OutQuad);
static UIElement bp_cost_prompt(47,78,38,9,66,82,0,0,Tween::Easing::OutQuad);

static UIOptions options(false, {"NO", "YES"});
std::vector<int> bps_avail;

int flashing = 0;

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
//        if (i > 2) {
            opts.push_back(bp_names[i]);
            bps_avail.push_back(i);
        }
    }
    options = UIOptions(false, opts);
    options.reset();
    title.setCenter(55, 16);
    title.setVisibility(true, uint32_t(0));
    flashing = 0;
    bp_cost_prompt.setVisibility(true);
}

void updateBlueprintsShopState(FSM &fsm)
{
    ControlStatus status = Controls::getStatus();
    options.update(status);

    if (flashing > 0) flashing--;
    if (status.a.pressed()) {
        if (GameVariables::dollars() >= bp_costs[options.activeIndex()]) {
            GameVariables::unlockBlueprint(bps_avail[options.activeIndex()]);
            Player::updateOwnedWeapons();
            AudioSystem::play(sfxGetItem);
            showShop(true);
            GameVariables::changeDollars(-bp_costs[options.activeIndex()]);
            return;
        } else {
            AudioSystem::play(sfxDeny);
            flashing = 20;
        }
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
            const int spacing = 2;
            const int carousel_size = (spacing + 6) * bps_avail.size() + spacing;
            const int carousel_x = x + w/2 - carousel_size/2;
            Helpers::drawNotchedRect(carousel_x - spacing/2, y + h + 2, carousel_size, 6, 1);
            if (options.activeIndex() > 0) {
                RenderSystem::sprite(carousel_x - 5, y + h + 3, ui_arrow_left, ui_arrow_left[2]);
            }
            if (options.activeIndex() < bps_avail.size()-1) {
                RenderSystem::sprite(carousel_x + carousel_size - 1, y + h + 3, ui_arrow_left, ui_arrow_left[2], true);
            }

            x -= 8;
            w += 16;
            options.foreach([&](uint8_t idx, bool active, const char * text) {
                RenderSystem::sprite(carousel_x + idx * (spacing + 6), y + h + 2, bullet_empty, 1);
                if (active) {
                    RenderSystem::drawRect2(carousel_x + idx * (spacing + 6) + 2, y + h + 2 + 2, 2, 2, 10);

                    Helpers::drawNotchedRect(x, y + h + 15, w, 22, 0);
                    Helpers::printHorizontallyCentered(x + w/2, y + h + 17, text, 10);
                    Helpers::printHorizontallyCentered(x + w/2, y + h + 27, bp_descs[bps_avail[idx]], 8);
                }
            });
        }
    });

    bp_cost_prompt.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h) {
        if (flashing > 0 && (flashing % 6 < 4)) {
            Helpers::drawNotchedRect(x, y, w, h, 15);
        }
        if (h > 8 && options.activeIndex() > 0) {
            std::string line = "COST: " + std::to_string(bp_costs[bps_avail[options.activeIndex()-1]]);
            Helpers::printHorizontallyCentered(x + w/2, y + 1, line.c_str(), 10);

        }
    });
}
