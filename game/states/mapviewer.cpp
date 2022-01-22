#include "mapviewer.h"

#include "core/controls.h"

#include "game/states/game.h"
#include "game/states/pause.h"
#include "game/ui/ui.h"
#include "game/utilities/helpers.h"

#include "game/utilities/mapmanager.h"

static UIElement title_prompt = UIElement::getExpander(55,6,60,11, Tween::Easing::OutQuad);
static UIElement map_area = UIElement::getExpander(55,44,102,80, Tween::Easing::OutQuad);

bool hiding = false;

void MapViewer::go()
{
    title_prompt.setVisibility(true, uint32_t(0));
    map_area.setVisibility(true, uint32_t(0));
    hiding = false;
}

void MapViewer::update(FSM &fsm)
{
    if (hiding && title_prompt.hidden()) {
        goPause(true);
    }
    if (hiding) return;
    ControlStatus ctrl = Controls::getStatus();
    if (ctrl.b.pressed()) {
        hiding = true;
        title_prompt.setVisibility(false);
        map_area.setVisibility(false);
    }
}

void MapViewer::draw()
{
    drawShadedGame();
    map_area.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h){
        if (h > 8 && w > 8) RenderSystem::drawRect2(x + 2, y + 2, w - 4, h - 4, 1);
        if (h > 79) {
//            int spacing = 216/72;
//            for(int j = 0; j < 72; ++j) {
//                for(int i = 0; i < 72; ++i) {
//                    if (i * spacing >= 216 || j * spacing >= 216) continue;
//                    int t = MapManager::getTileAt((i * spacing) * 6, (j * spacing) * 6);
//                    RenderSystem::pixel(x + 16 + i, y + 4 + j, t == 19 ? 1 : t < 53 ? 2 : t >= 162 ? 4 : t == 86 ? 7 : 3);
//                }
//            }
            int spacing = 216/54;
            for(int j = 0; j < 54; ++j) {
                for(int i = 0; i < 54; ++i) {
                    if (i * spacing >= 216 || j * spacing >= 216) continue;
                    int t = MapManager::getTileAt((i * spacing) * 6, (j * spacing) * 6);
                    RenderSystem::pixel(x + 16 + i, y + 4 + j, t == 19 ? 1 : t < 53 ? 2 : t >= 162 ? 4 : 3);
                }
            }
            RenderSystem::print(x + 4, y + h - 4 - 5, "AUSTREXICO", 0);
        }
    });
//    title_prompt.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h){
//        if (h > 10) {
//            Helpers::printHorizontallyCentered(x + w/2, y + 2, "Map", 10);
//        }
//    });
}
