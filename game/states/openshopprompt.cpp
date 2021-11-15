#include "openshopprompt.h"

#include "core//controls.h"
#include "game/entities/pois.h"
#include "game/states/game.h"
#include "core//rendersystem.h"

#include "game/states/shop.h"

#include "core/utilities/tween.h"
#include "game/constants.h"

#include "game/ui/ui.h"

#include "game/variables.h"

static bool can_open = false;

static UIElement title_box = UIElement::getExpander(55, 35, 76, 9, Tween::Easing::OutQuad);
static UIElement alert_box = UIElement::getExpander(55, 45, 76, 9, Tween::Easing::OutQuad);
static UIOptions yes_no(true, {"NO", "YES"});

void showOpenShopPrompt()
{
    FSM::instance->go(GameStates::ShowUnlockShop);

    UI::setVisibility(UI::Element::UIHackingKitCount, true, uint32_t(100));

    UI::hideHealthbar();
    UI::setVisibility(UI::Element::UIKeyACount, false);
    UI::setVisibility(UI::Element::UIKeyBCount, false);
    UI::setVisibility(UI::Element::UIKeyCCount, false);
    UI::setVisibility(UI::Element::UIDollarCount, false);

    can_open = GameVariables::hackingKits() > 0;
    title_box.setVisibility(true, uint32_t(50));
    alert_box.setVisibility(true, uint32_t(150));
    yes_no.reset();
}

void quitOpenShop() {
    title_box.setVisibility(false);
    alert_box.setVisibility(false);
    goGame();
}

void updateOpenShopState(FSM&)
{
    ControlStatus status = Controls::getStatus();

    if (can_open) {
        if (status.b.pressed()) {
            quitOpenShop();
        } else if (status.a.pressed()) {
            if (yes_no.activeIndex() == 0) {
                quitOpenShop();
            } else {
                title_box.setVisibility(false);
                alert_box.setVisibility(false);
                GameVariables::changeHackingKits(-1);
                POIs::unlockCurrent();
                showShop();
            }
        }
        yes_no.update(status);
    } else {
        if (status.a.pressed() || status.b.pressed()) {
            quitOpenShop();
        }
    }
}

void drawOpenShopState()
{
    drawShadedGame();
    title_box.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h){
        if (h > 8) Helpers::printHorizontallyCentered(x + w/2, y + 1, "ENEMY BASE", 10);
    });
    if (!can_open) {
        alert_box.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h){
            if (h > 8) Helpers::printHorizontallyCentered(x + w/2, y + 1, "- NO HACKING KITS -", 6);
        });
    } else {
        alert_box.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h){
            if (h > 8) {
                Helpers::printHorizontallyCentered(x + w/2, y + 1, "USE A HACKING KIT?", 10);
                yes_no.foreach([](uint8_t idx, bool active, const char * name) {
                    Helpers::drawNotchedRect(46, 51 + idx * 8, 20, 7, 0);
                    RenderSystem::sprite(38, 51 + idx * 8, poi[active ? 1 : 0]);
                    Helpers::printHorizontallyCentered(56, 51 + idx * 8, name, active ? 10 : 6);
                });
            }
        });
    }
}
