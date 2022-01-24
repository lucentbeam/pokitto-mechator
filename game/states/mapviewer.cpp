#include "mapviewer.h"

#include "core/controls.h"

#include "game/states/game.h"
#include "game/states/pause.h"
#include "game/ui/ui.h"
#include "game/utilities/helpers.h"

#include "game/utilities/mapmanager.h"
#include "game/maps/mapsprite.h"
#include "game/player.h"

#include "game/variables.h"

static UIElement title_prompt = UIElement::getExpander(48,8,60,11, Tween::Easing::OutQuad);
static UIElement map_area = UIElement::getExpander(48,50,84,66, Tween::Easing::OutQuad);

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
Blinker blue_dot(3.6f, 1.2f);
Blinker red_dot(3.6f, 2.4f);

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
    blue_dot.update();
    red_dot.update();

#ifdef DEBUGS
    if (ctrl.right.pressed()) {
        static int q = 0;
        q = (q + 1) % 6;
        GameVariables::setQuestStatus(q);
    }
#endif
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
            Vec2i goal = GameVariables::getGoal();
            if (goal.x() > -200) {
                Vec2f pos = Vec2f(goal.x() * 54.0f/216.0f, goal.y() * 54.0f/216.0f);
                RenderSystem::sprite(tl.x() + pos.x() - 3.5f, tl.y() + pos.y() - 3.5f, poi[4], 0);
            }
            RenderSystem::sprite(109 - ui_legend[0], y + h/2 - ui_legend[1]/2, ui_legend, ui_legend[2]);
            if (blue_dot.active()) {
                RenderSystem::pixel(109 - ui_legend[0] + 5, y + h/2 - ui_legend[1]/2 + 23, 48);
            } else if (red_dot.active()) {
                RenderSystem::pixel(109 - ui_legend[0] + 5, y + h/2 - ui_legend[1]/2 + 23, 16);
            }
        }
    });
    title_prompt.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h){
        if (h > 10) {
            Helpers::printHorizontallyCentered(x + w/2, y + 2, "Map", 10);
        }
    });
}
