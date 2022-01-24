#include "mapviewer.h"

#include "core/controls.h"

#include "game/states/game.h"
#include "game/states/pause.h"
#include "game/ui/ui.h"
#include "game/utilities/helpers.h"

#include "game/utilities/mapmanager.h"
#include "game/maps/mapsprite.h"
#include "game/player.h"

static UIElement title_prompt = UIElement::getExpander(55,7,60,11, Tween::Easing::OutQuad);
static UIElement map_area = UIElement::getExpander(55,48,84,66, Tween::Easing::OutQuad);

bool hiding = false;

class Blinker {
    float counter = 0.0f;
    const float total, on_length;
public:
    Blinker(float tot, float on) : total(tot), on_length(on) {}
    void update() {
        counter += physicsTimestep;
        while (counter >= total) counter -= total;
    }
    bool active() const { return counter < on_length; }
};

Blinker blinky(1.2f, 1.0f);

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
    blinky.update();
}

void MapViewer::draw()
{
    drawShadedGame();
    map_area.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h){
        if (h > 8 && w > 8) RenderSystem::drawRect2(x + 2, y + 2, w - 4, h - 4, 1);
        if (h > 56) {
            Vec2i tl = Vec2i(x + w/2 - sprite_map[0]/2, y + h/2-sprite_map[1]/2);
            RenderSystem::sprite(tl.x(), tl.y(), sprite_map);
            if (blinky.active()) {
                Vec2f pos = Player::position();
                pos *= 54.0f/216.0f/6.0f;
                if (pos.x() < (x - tl.x())) {
                    pos.setX(x - tl.x());
                } else if (pos.x() > (x - tl.x() + w - 2)) {
                    pos.setX(x - tl.x() + w - 2);
                }
                if (pos.y() < (y - tl.y())) {
                    pos.setY(y - tl.y());
                } else if (pos.y() > (y - tl.y() + h - 2)) {
                    pos.setY(y - tl.y() + h - 2);
                }
                RenderSystem::sprite(tl.x() + pos.x() - 3.5f, tl.y() + pos.y() - 3.5f, poi[1], 0);
            }
        }
    });
    title_prompt.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h){
        if (h > 10) {
            Helpers::printHorizontallyCentered(x + w/2, y + 2, "Map", 10);
        }
    });
}
