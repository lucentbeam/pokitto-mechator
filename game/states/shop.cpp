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

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

static UIElement title = UIElement::getExpander(55, 28, 70, 9, Tween::Easing::OutQuad);
static UIOptions title_opts(4);
static const char * title_opt_lines[4] = {"LEAVE", "SAVE", "REPAIR/BUILD", "BLUEPRINTS"};

static UIElement blueprintunlock = UIElement::getExpander(55, 82, 80, 9, Tween::Easing::OutQuad);

void showShop(bool from_repairs)
{
    FSM::instance->go(GameStates::ShowShop);

    UI::hideHealthbar();
    UI::setVisibility(UI::Element::UIDollarCount, false);
    UI::setVisibility(UI::Element::UIKeyACount, false);
    UI::setVisibility(UI::Element::UIKeyBCount, false);
    UI::setVisibility(UI::Element::UIKeyCCount, false);

    UI::setVisibility(UI::Element::UIHackingKitCount, false);

    UI::setVisibility(UI::Element::GameSavedPrompt, false);

    title.setVisibility(true, uint32_t(50));
    if (!from_repairs) title_opts.reset();
    title_opts.setAvailableCount(GameVariables::hasUnusedBlueprints() ? 4 : 3);
//    active_options = GameVariables::hasUnusedBlueprints() ? &title_opts2 : &title_opts1;
}

void quitShopState() {
    goGame();
    title.setVisibility(false);
    blueprintunlock.setVisibility(false, true);
}

void goRepairState() {
    title.setVisibility(false);
    showRepairs();
    blueprintunlock.setVisibility(false, true);
}

void goBlueprintState() {
    title.setVisibility(false);
    showBlueprintsShop();
    blueprintunlock.setVisibility(false, true);
}

void updateShopState(FSM&)
{
    ControlStatus status = Controls::getStatus();
    UI::update(physicsTimestep);
    blueprintunlock.update(physicsTimestep);

    title_opts.update(status.down.pressed(), status.up.pressed());

    if (status.a.pressed()) {
        AudioSystem::play(sfxConfirm);
        switch(title_opts.activeIndex()) {
        case 0:
            Controls::blockControls(8);
            quitShopState();
            break;
        case 1:
            Player::storeData();
            GameVariables::saveGame();
            MapManager::dumpMutables(GameVariables::savefile);
#ifdef __EMSCRIPTEN__
            EM_ASM({ FS.syncfs(false, function(Error) {}); });
#endif
            quitShopState();
            UI::showForDuration(UI::Element::GameSavedPrompt, 2.0f);
            AudioSystem::play(sfxGetItem);
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
            title_opts.foreach([&](uint8_t idx, bool active) {
                Helpers::drawNotchedRect(x + 9, y + 10 + idx * 8, w - 9, 7, 0);
                RenderSystem::sprite(x, y + 10 + idx * 8, poi[!active ? 0 : 1]);

                RenderSystem::print(x + 12, y + 10 + idx * 8, title_opt_lines[idx], !active ? 6 : 10);
            });

        }
    });
    blueprintunlock.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h){
       if (h > 7) {
           Helpers::printHorizontallyCentered(x + w/2, y + 1, "BLUEPRINT UNLOCKED", 10);
       }
    });
}

void showBPUnlock()
{
    blueprintunlock.showForDuration(2.0f);
}
