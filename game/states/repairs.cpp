#include "repairs.h"

#include <string>

#include "game/states/game.h"
#include "game/ui/ui.h"
#include "core//rendersystem.h"
#include "core//controls.h"
#include "game/enums.h"
#include "game/states/shop.h"
#include "game/utilities/helpers.h"
#include "game/sprites.h"
#include "game/player.h"
#include "game/variables.h"
#include "game/entities/pois.h"
#include "core/audiosystem.h"
#include "game/maps/alertregion.h"

static UIElement title = UIElement::getExpander(55, 35, 76, 9, Tween::Easing::OutQuad);
//static UIOptions repair_opts(true, {"BACK", "SOLDIER (REST)", "JEEP", "TANK", "BOAT", "HELI"});
static UIOptions repair_opts(6);
static const std::string repair_texts[] = {"BACK", "SOLDIER (REST)", "JEEP", "TANK", "BOAT", "HELI"};
static uint8_t current_cost = 0;
static UIElement cost_prompt(47,78,38,9,66,82,0,0,Tween::Easing::OutQuad);

static int vehicles_avail = 0;

void showRepairs()
{
    FSM::instance->go(GameStates::ShowRepairs);
    UI::setVisibility(UI::Element::UIDollarCount, true, uint32_t(100));

    UI::hideHealthbar();
    UI::setVisibility(UI::Element::UIKeyACount, false);
    UI::setVisibility(UI::Element::UIKeyBCount, false);
    UI::setVisibility(UI::Element::UIKeyCCount, false);
    UI::setVisibility(UI::Element::UIHackingKitCount, false);

    title.setVisibility(true, uint32_t(50));
    repair_opts.reset();

    vehicles_avail = 0;
    for(int i = 1; i < 5; ++i) {
        PlayerMode m = PlayerMode(i);
        bool avail = POIs::canBuild(m) && Player::available(m);
        repair_opts.setActiveAt(i + 1, avail);
        if (avail) vehicles_avail++;
    }

    cost_prompt.setVisibility(false);
    current_cost = 0;
}

void goBack() {
    AudioSystem::play(sfxCancel);
    title.setVisibility(false);
    cost_prompt.setVisibility(false);
    current_cost = 0;
    showShop(true);
}

void updateRepairsState(FSM &fsm)
{
    ControlStatus status = Controls::getStatus();

    repair_opts.update(status.down.pressed(), status.up.pressed(),[](int8_t idx) {
        if (idx == 0) {
            UI::hideHealthbar();
            current_cost = 0;
        } else if (idx == 1) {
            UI::showHealthbar(SoldierMode);
            current_cost = Player::damaged(SoldierMode) ? soldierRepairCost : 0;
        } else {
            PlayerMode mode = PlayerMode(idx - 1);
            if (Player::alive(mode)) {
                UI::showHealthbar(mode);
                current_cost = Player::damaged(mode) ? repairCosts[idx - 2] : 0;
            } else {
                UI::hideHealthbar();
                current_cost = buildCosts[idx - 2];
                if ((mode == JeepMode && isInRegion(RegionTutorial)) || (mode == TankMode && isInRegion(RegionTankFactory))) {
                    current_cost = 0;
                }
            }
        }
    });
    cost_prompt.setVisibility(current_cost > 0);
    if (status.a.pressed() && GameVariables::dollars() < current_cost) {
        AudioSystem::play(sfxDeny);
    }
    if (status.a.pressed() && GameVariables::dollars() >= current_cost) {
        AudioSystem::play(sfxConfirm);
        if (repair_opts.activeIndex() == 0) {
            goBack();
        } else if (repair_opts.activeIndex() == 1) {
            Player::health(SoldierMode).setMax();
            GameVariables::changeDollars(-current_cost);
            current_cost = 0;
        } else {
            PlayerMode mode = PlayerMode(repair_opts.activeIndex() - 1);
            if (Player::buildVehicleAt(mode, POIs::pos(mode))) {
                UI::showHealthbar(mode);
            }
            GameVariables::changeDollars(-current_cost);
            current_cost = 0;
        }
    } else if (status.b.pressed()) {
        goBack();
    }
}

void drawRepairsState()
{
    drawShadedGame();
    title.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h) {
        if (h > 8) {
            Helpers::printHorizontallyCentered(x + w/2, y + 1, "WORKSHOP", 10);
            repair_opts.foreach([&](uint8_t idx, uint8_t opt, bool active) {
                std::string line = repair_texts[opt];
                if (opt > 1) {
                    line += !Player::alive(PlayerMode(opt-1)) ? " [BUILD]" : " [REPAIR]";
                }
                Helpers::drawNotchedRect(x + 9, y + 10 + idx * 8, w - 9, 7, 0);
                RenderSystem::sprite(x, y + 10 + idx * 8, poi[active ? 1 : 0]);
                RenderSystem::print(x + 12, y + 10 + idx * 8, line.c_str(), active ? 10 : 6);
            });
        }
    }, 0, vehicles_avail * -4); // scale with unlock level
    cost_prompt.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h) {
        if (h > 8) {
            std::string line = "COST: " + std::to_string(current_cost);
            Helpers::printHorizontallyCentered(x + w/2, y + 1, line.c_str(), 10);
        }
    });
}
