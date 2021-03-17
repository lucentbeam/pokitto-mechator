#include "repairs.h"

#include <string>

#include "game/states/game.h"
#include "game/ui/ui.h"
#include "core/rendering/rendersystem.h"
#include "core/controls/controls.h"
#include "game/enums.h"
#include "game/states/shop.h"
#include "game/utilities/helpers.h"
#include "game/tilesets.h"
#include "game/player.h"
#include "game/variables.h"
#include "game/entities/pois.h"

static UIElement title = UIElement::getExpander(55, 35, 76, 9, Tween::Easing::OutQuad);
static UIOptions repair_opts(true, {"BACK", "SOLDIER (REST)", "JEEP", "TANK", "BOAT", "HELI"});
static uint8_t current_cost = 0;
static UIElement cost_prompt(47,78,38,9,66,82,0,0,Tween::Easing::OutQuad);
static uint8_t available_vehicles = 0;

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

    // TODO: make availability also depend on if POI/shop locations exist (e.g., no heli-pad, no helicopter option)
    available_vehicles = 0;
    available_vehicles += Player::available(PlayerMode::Jeep) ? 1 : 0;
    available_vehicles += Player::available(PlayerMode::Tank) ? 1 : 0;
    available_vehicles += Player::available(PlayerMode::Boat) ? 1 : 0;
    available_vehicles += Player::available(PlayerMode::Helicopter) ? 1 : 0;

    repair_opts.setAvailableCount(2 + available_vehicles);
    cost_prompt.setVisibility(false);
    current_cost = 0;
}

void goBack() {
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
        } else {
            PlayerMode mode = PlayerMode(idx-1);
            if (!Player::alive(mode)) {
                UI::hideHealthbar();
                const uint8_t build_costs[5] = {0, 12, 18, 24, 30};
                current_cost = build_costs[idx-1];
            } else {
                UI::showHealthbar(mode);
                if (Player::damaged(mode)) {
                    const uint8_t repair_costs[5] = {4, 8, 12, 16, 20};
                    current_cost = repair_costs[idx-1];
                } else {
                    current_cost = 0;
                }
            }
        }
    });
    cost_prompt.setVisibility(current_cost > 0);

    if (status.a.pressed() && GameVariables::dollars() >= current_cost) {
        switch(repair_opts.activeIndex()) {
        case 0:
            goBack();
            break;
        case 1:
            Player::soldierHealth().setMax();
            GameVariables::changeDollars(-current_cost);
            current_cost = 0;
            break;
        case 2:
            if (!Player::alive(PlayerMode::Jeep)) {
                Player::setPosition(PlayerMode::Jeep, POIs::pos(PlayerMode::Jeep));
                UI::showHealthbar(PlayerMode::Jeep);
            }
            Player::jeepHealth().setMax();
            GameVariables::changeDollars(-current_cost);
            current_cost = 0;
            break;
        default:
            break;
        }
    } else if (status.b.pressed()) {
        goBack();
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
                    line += !Player::alive(PlayerMode(idx-1)) ? " (BUILD)" : " (REPAIR)";
                }
                Helpers::drawNotchedRect(x + 9, y + 10 + idx * 8, w - 9, 7, 0);
                RenderSystem::sprite(x, y + 10 + idx * 8, poi[active ? 1 : 0]);
                RenderSystem::print(x + 12, y + 11 + idx * 8, line.c_str(), active ? 41 : 25);
            });

        }
    }, 0, available_vehicles * -9); // scale with unlock level
    cost_prompt.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h) {
        if (h > 8) {
            std::string line = "COST: " + std::to_string(current_cost);
            Helpers::printHorizontallyCentered(x + w/2, y + 2, line.c_str(), 41);
        }
    });
}
