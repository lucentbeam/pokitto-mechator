#include "mapviewer.h"

#include "core/controls.h"
#include "core/audiosystem.h"

#include "game/states/game.h"
#include "game/states/pause.h"
#include "game/ui/ui.h"
#include "game/utilities/helpers.h"

#include "game/maps/worldtiles.h"
#include "game/utilities/mapmanager.h"
#include "game/maps/mapsprite.h"
#include "game/player.h"

#include "game/variables.h"
#include "game/utilities/blinker.h"

#include "game/maps/spawnpoint.h"
#include "game/maps/spawnpoints.h"

static UIElement title_prompt = UIElement::getExpander(48,8,48,11, Tween::Easing::OutQuad);
static UIElement map_area = UIElement::getExpander(48,50,84,66, Tween::Easing::OutQuad);

bool hiding = false;

Blinker playerloc_blink(0.4f, 0.3f);
Blinker goal_blink(1.2f, 1.0f);
Blinker door_blink(3.6f, 1.2f);

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
        AudioSystem::play(sfxCancel);
    }
    playerloc_blink.update();
    door_blink.update();
    goal_blink.update();
}

void MapViewer::draw()
{
    drawShadedGame();
    map_area.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h){
        constexpr float world_to_loc = 54.0f / 216.0f / 6.0f;
        if (h > 8 && w > 8) RenderSystem::drawRect2(x + 2, y + 2, w - 4, h - 4, 1);
        if (h > 56) {
            Vec2i tl = Vec2i(x + w/2 - mechator_reduced[0]/2 + 11.0f, y + h/2-mechator_reduced[1]/2);
            Helpers::drawRLE(tl.x() - 11.0f, tl.y(), mechator_reduced, mechator_reduced[2]);

            int color;
            for (int i = 0; i < spawnpoint_data_count; ++i) {
                if (GameVariables::doorStates()[i] == Discovered) {
                    SpawnPoint pt = points[i];
                    color = pt.type() == SpawnPoint::SpawnPointType::DoorA ? 48 : pt.type() == SpawnPoint::SpawnPointType::DoorB ? 32 : pt.type() == SpawnPoint::SpawnPointType::DoorC ? 16 : 10;
                    Vec2f pos = pt.pos() * world_to_loc;
                    RenderSystem::drawRect2(tl.x() + pos.x() - 1, tl.y() + pos.y() - 1, 2, 2, color);
                }
            }

            Vec2i goal = GameVariables::getGoal();
            if (goal.x() > -200) {
                Vec2f pos = Vec2f(goal.x() * 54.0f/216.0f, goal.y() * 54.0f/216.0f);
                RenderSystem::sprite(tl.x() + pos.x() - 3.5f, tl.y() + pos.y() - 3.5f, poi[goal_blink.active() ? 1 : 4], 0);
            }
            Helpers::drawNotchedRect(80, y + h/2 - 17, 30, 37, 0);
            if (door_blink.active()) {
                RenderSystem::drawRect2(83, y + h/2 + 5, 3, 3, 48);
            } else if (door_blink.active(1.2f)) {
                RenderSystem::drawRect2(83, y + h/2 + 5, 3, 3, 16);
            } else {
                RenderSystem::drawRect2(83, y + h/2 + 5, 3, 3, 32);
            }
            if (playerloc_blink.active()) RenderSystem::pixel(84, y + h/2 - 12, 10);
            RenderSystem::print(91, y + h/2 - 15, "loc", 10);

            RenderSystem::sprite(81, y + h/2 - 6, poi[goal_blink.active() ? 1 : 4], 0);
            RenderSystem::print(90, y + h/2 - 7, "goal", 10);

            RenderSystem::print(90, y + h/2 + 2, "door", 10);

            RenderSystem::drawRect2(83, y + h/2 + 13, 3, 3, 10);
            RenderSystem::print(90, y + h/2 + 10, "base", 10);

            if (playerloc_blink.active()) {
                Vec2f pos = Player::position();
                pos *= world_to_loc;
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
                RenderSystem::pixel(tl.x() + pos.x(), tl.y() + pos.y(), 10);
            }
        }
    });
    title_prompt.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h){
        if (h > 10) {
            Helpers::printHorizontallyCentered(x + w/2, y + 2, "Map", 10);
        }
    });
}
