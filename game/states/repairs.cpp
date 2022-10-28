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
static UIOptions repair_opts(true, {"BACK", "SOLDIER (REST)", "JEEP", "TANK", "BOAT", "HELI"});
static uint8_t current_cost = 0;
static UIElement cost_prompt(47,78,38,9,66,82,0,0,Tween::Easing::OutQuad);

static PlayerMode vehicles_avail[4] = { SoldierMode, SoldierMode, SoldierMode, SoldierMode };

int availableVehicleCount() {
    int ct = 0;
    for(int i = 0; i < 4; ++i) {
        if (vehicles_avail[i] != SoldierMode) ct++;
    }
    return ct;
}

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

    int counter = 0;
    while (counter < 4) {
        vehicles_avail[counter] = SoldierMode;
        counter++;
    }
    counter = 0;

    for(int i = 1; i < 5; ++i) {
        PlayerMode m = PlayerMode(i);
        if (POIs::canBuild(m) && Player::available(m)) {
            vehicles_avail[counter] = m;
            counter++;
        }
    }

    repair_opts.setAvailableCount(2 + availableVehicleCount());
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

    repair_opts.update(status,[](int8_t idx) {
        if (idx == 0) {
            UI::hideHealthbar();
            current_cost = 0;
        } else if (idx == 1) {
            UI::showHealthbar(SoldierMode);
            current_cost = Player::damaged(SoldierMode) ? soldierRepairCost : 0;
        } else {
            PlayerMode mode = vehicles_avail[idx - 2];
            if (mode != SoldierMode) {
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
            PlayerMode mode = vehicles_avail[repair_opts.activeIndex() - 2];
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
            repair_opts.foreach([&](uint8_t idx, bool active, const char * text) {
                std::string line = text;
                if (idx > 1) {
                    line += !Player::alive(PlayerMode(idx-1)) ? " [BUILD]" : " [REPAIR]";
                }
                Helpers::drawNotchedRect(x + 9, y + 10 + idx * 8, w - 9, 7, 0);
                RenderSystem::sprite(x, y + 10 + idx * 8, poi[active ? 1 : 0]);
                RenderSystem::print(x + 12, y + 10 + idx * 8, line.c_str(), active ? 10 : 6);
            });
        }
    }, 0, availableVehicleCount() * -4); // scale with unlock level
    cost_prompt.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h) {
        if (h > 8) {
            std::string line = "COST: " + std::to_string(current_cost);
            Helpers::printHorizontallyCentered(x + w/2, y + 1, line.c_str(), 10);
        }
    });
}
