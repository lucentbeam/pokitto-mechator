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
#include "game/utilities/blinker.h"

static UIElement title = UIElement::getExpander(55, 35, 88, 9, Tween::Easing::OutQuad);
static UIElement bp_cost_prompt(47,78,38,9,66,82,0,0,Tween::Easing::OutQuad);

static UIOptions options(2);
std::vector<int> bps_avail;

int flashing = 0;

static Blinker chevron_blink(1.0f, 0.8f);

void showBlueprintsShop()
{
    FSM::instance->go(GameStates::ShowBlueprintShop);

    UI::hideHealthbar();
    UI::setVisibility(UI::Element::UIHackingKitCount, false, uint32_t(0));
    UI::setVisibility(UI::Element::UIKeyACount, false);
    UI::setVisibility(UI::Element::UIKeyBCount, false);
    UI::setVisibility(UI::Element::UIKeyCCount, false);
    UI::setVisibility(UI::Element::UIDollarCount, true);

    bps_avail.clear();
    for(int i = 0; i < int(Blueprints::LastIdxBP); ++i) {
        if (GameVariables::hasBlueprintToUnlock(Blueprints(i))) {
            bps_avail.push_back(i);
        }
    }
    options = UIOptions(bps_avail.size());
    options.reset();
    title.setCenter(55, 16);
    title.setVisibility(true, uint32_t(0));
    flashing = 0;
    bp_cost_prompt.setVisibility(true);
}

void updateBlueprintsShopState(FSM &fsm)
{
    chevron_blink.update();
    ControlStatus status = Controls::getStatus();
    options.update(status.right.pressed(), status.left.pressed());

    if (flashing > 0) flashing--;
    if (status.a.pressed()) {
        if (GameVariables::dollars() >= bp_costs[bps_avail[options.activeIndex()]]) {
            GameVariables::unlockBlueprint(bps_avail[options.activeIndex()]);
            Player::updateOwnedWeapons();
            AudioSystem::play(sfxGetItem);
            showShop(true);
            showBPUnlock();
            GameVariables::changeDollars(-bp_costs[bps_avail[options.activeIndex()]]);
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
            if (chevron_blink.active()) {
                if (options.activeIndex() > 0) {
                    RenderSystem::sprite(carousel_x - 5, y + h + 2, ui_arrow_left, ui_arrow_left[2]);
                }
                if (options.activeIndex() < bps_avail.size()-1) {
                    RenderSystem::sprite(carousel_x + carousel_size, y + h + 2, ui_arrow_left, ui_arrow_left[2], true);
                }
            }

            x -= 8;
            w += 16;
            options.foreach([&](uint8_t idx, bool active) {
                RenderSystem::sprite(carousel_x + idx * (spacing + 6), y + h + 2, bullet_empty, 1);
                if (active) {
                    RenderSystem::drawRect2(carousel_x + idx * (spacing + 6) + 2, y + h + 2 + 2, 2, 2, 10);

                    Helpers::drawNotchedRect(x, y + h + 15, w, 22, 0);
                    Helpers::printHorizontallyCentered(x + w/2, y + h + 17, bp_names[bps_avail[idx]], 10);
                    Helpers::printHorizontallyCentered(x + w/2, y + h + 27, bp_descs[bps_avail[idx]], 8);

                    Helpers::drawNotchedRect(x, y + h + 48, w + 2, 8, 0);
                    Helpers::printHorizontallyCentered(x + w/2, y + h + 48, bp_desc_types[bps_avail[idx]], 8);
                }
            });
        }
    });

//    if (bp_costs[bps_avail[options.activeIndex()]] == 0) return;
    bp_cost_prompt.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h) {
        if (h > 8) {
            std::string line = "COST: " + std::to_string(bp_costs[bps_avail[options.activeIndex()]]);
            Helpers::printHorizontallyCentered(x + w/2, y + 1, line.c_str(), (flashing > 0 && (flashing % 6 < 4)) ? 15 : 10);
        }
    });
}
