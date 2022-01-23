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
    available_vehicles += Jeep::available() ? 4 : 0;
//    available_vehicles += Player::available(PlayerMode::Tank) ? 1 : 0;
//    available_vehicles += Player::available(PlayerMode::Boat) ? 1 : 0;
//    available_vehicles += Player::available(PlayerMode::Helicopter) ? 1 : 0;

    repair_opts.setAvailableCount(2 + available_vehicles);
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
        } else {
            PlayerMode mode = PlayerMode(idx-1);
            switch (mode) {
            case SoldierMode:
                UI::showHealthbar(mode);
                current_cost = Soldier::damaged() ? soldierRepairCost : 0;
                break;
            case JeepMode:
                if (Jeep::alive()) {
                    UI::showHealthbar(mode);
                    current_cost = Jeep::damaged() ? jeepRepairCost : 0;
                } else {
                    UI::hideHealthbar();
                    current_cost = isInRegion("Tutorial Island") ? 0 : jeepBuildCost;
                }
                break;
            case TankMode:
                if (Tank::alive()) {
                    UI::showHealthbar(mode);
                    current_cost = Tank::damaged() ? tankRepairCost : 0;
                } else {
                    UI::hideHealthbar();
                    current_cost = isInRegion("Tank Factory") ? 0 : tankBuildCost;
                }
                break;
            case BoatMode:
                if (Boat::alive()) {
                    UI::showHealthbar(mode);
                    current_cost = Boat::damaged() ? boatRepairCost : 0;
                } else {
                    UI::hideHealthbar();
                    current_cost = boatBuildCost;
                }
                break;
            case HelicopterMode:
                if (Helicopter::alive()) {
                    UI::showHealthbar(mode);
                    current_cost = Helicopter::damaged() ? heliRepairCost : 0;
                } else {
                    UI::hideHealthbar();
                    current_cost = heliBuildCost;
                }
                break;
            default:
                current_cost = 0;
            }
        }
    });
    cost_prompt.setVisibility(current_cost > 0);
    if (status.a.pressed() && GameVariables::dollars() < current_cost) {
        AudioSystem::play(sfxDeny);
    }
    if (status.a.pressed() && GameVariables::dollars() >= current_cost) {
        AudioSystem::play(sfxConfirm);
        switch(repair_opts.activeIndex()) {
        case 0:
            goBack();
            break;
        case 1:
            Soldier::health().setMax();
            GameVariables::changeDollars(-current_cost);
            current_cost = 0;
            break;
        case 2:
            if (!Jeep::alive()) {
                Jeep::setPosition(POIs::pos(PlayerMode::JeepMode));
                UI::showHealthbar(PlayerMode::JeepMode);
            }
            Jeep::health().setMax();
            GameVariables::changeDollars(-current_cost);
            current_cost = 0;
            break;
        case 3:
            if (!Tank::alive()) {
                Tank::setPosition(POIs::pos(PlayerMode::TankMode));
                UI::showHealthbar(PlayerMode::TankMode);
            }
            Tank::health().setMax();
            GameVariables::changeDollars(-current_cost);
            current_cost = 0;
            break;
        case 4:
            if (!Boat::alive()) {
                Boat::setPosition(POIs::pos(PlayerMode::BoatMode));
                UI::showHealthbar(PlayerMode::BoatMode);
            }
            Boat::health().setMax();
            GameVariables::changeDollars(-current_cost);
            current_cost = 0;
            break;
        case 5:
            if (!Helicopter::alive()) {
                Helicopter::setPosition(POIs::pos(PlayerMode::HelicopterMode));
                UI::showHealthbar(PlayerMode::HelicopterMode);
            }
            Helicopter::health().setMax();
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
    drawShadedGame();
    title.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h) {
        if (h > 8) {
            Helpers::printHorizontallyCentered(x + w/2, y + 1, "WORKSHOP", 10);
            repair_opts.foreach([&](uint8_t idx, bool active, const char * text) {
                std::string line = text;
                if (idx > 1) {
//                    line += !Player::alive(PlayerMode(idx-1)) ? " (BUILD)" : " (REPAIR)";
                }
                Helpers::drawNotchedRect(x + 9, y + 10 + idx * 8, w - 9, 7, 0);
                RenderSystem::sprite(x, y + 10 + idx * 8, poi[active ? 1 : 0]);
                RenderSystem::print(x + 12, y + 10 + idx * 8, line.c_str(), active ? 10 : 6);
            });
        }
    }, 0, available_vehicles * -4); // scale with unlock level
    cost_prompt.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h) {
        if (h > 8) {
            std::string line = "COST: " + std::to_string(current_cost);
            Helpers::printHorizontallyCentered(x + w/2, y + 1, line.c_str(), 10);
        }
    });
}
