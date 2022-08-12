#include "shop.h"

#include "core//controls.h"
#include "game/entities/pois.h"
#include "game/states/game.h"
#include "game/ui/ui.h"
#include "core//rendersystem.h"
#include "game/states/repairs.h"
#include "core/audiosystem.h"
#include "game/states/blueprints.h"
#include "game/variables.h"
#include "game/player.h"

static UIElement title = UIElement::getExpander(55, 28, 70, 9, Tween::Easing::OutQuad);
static UIOptions title_opts1(true, {"LEAVE", "SAVE", "REPAIR/BUILD"});
static UIOptions title_opts2(true, {"LEAVE", "SAVE", "REPAIR/BUILD", "BLUEPRINTS"});

UIOptions * active_options;

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
        title_opts1.reset();
        title_opts2.reset();
    }
    active_options = GameVariables::hasUnusedBlueprints() ? &title_opts2 : &title_opts1;
}

void quitShopState() {
    goGame();
    title.setVisibility(false);
}

void goRepairState() {
    title.setVisibility(false);
    showRepairs();
}

void goBlueprintState() {
    title.setVisibility(false);
    showBlueprintsShop();
}

void updateShopState(FSM&)
{
    ControlStatus status = Controls::getStatus();

    title_opts1.update(status);
    title_opts2.update(status);

    if (status.a.pressed()) {
        AudioSystem::play(sfxConfirm);
        switch(active_options->activeIndex()) {
        case 0:
            quitShopState();
            break;
        case 1:
            Player::storeData();
            GameVariables::saveGame();
            MapManager::dumpMutables(GameVariables::savefile);
            quitShopState();
            break;
        case 2:
            goRepairState();
            break;
        case 3:
            if (GameVariables::hasUnusedBlueprints()) {
                goBlueprintState();
            } else{
                AudioSystem::play(sfxDeny);
            }
            break;
        default:
            break;
        }
    } else if (status.b.pressed()) {
        AudioSystem::play(sfxCancel);
        quitShopState();
    }
}

void drawShopState()
{
    drawShadedGame();
    title.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h) {
        if (h > 8) {
            Helpers::printHorizontallyCentered(x + w/2, y + 1, "HACKED BASE", 10);
            active_options->foreach([&](uint8_t idx, bool active, const char * text) {
                Helpers::drawNotchedRect(x + 9, y + 10 + idx * 8, w - 9, 7, 0);
                RenderSystem::sprite(x, y + 10 + idx * 8, poi[!active ? 0 : 1]);

                RenderSystem::print(x + 12, y + 10 + idx * 8, text, !active ? 6 : 10);
            });

        }
    });
}
