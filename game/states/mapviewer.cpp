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
#include "game/maps/doors.h"
#include "game/utilities/blinker.h"

static UIElement title_prompt = UIElement::getExpander(48,8,48,11, Tween::Easing::OutQuad);
static UIElement map_area = UIElement::getExpander(48,50,84,66, Tween::Easing::OutQuad);

bool hiding = false;

Blinker playerloc_blink(1.2f, 1.0f);
Blinker goal_blink(3.6f, 3.25f);
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
        constexpr float world_to_loc = 54.0f / 216.0f / 6.0f;
        if (h > 8 && w > 8) RenderSystem::drawRect2(x + 2, y + 2, w - 4, h - 4, 1);
        if (h > 56) {
            Vec2i tl = Vec2i(x + w/2 - mechator_reduced[0]/2, y + h/2-mechator_reduced[1]/2);
            RenderSystem::sprite(tl.x(), tl.y(), mechator_reduced, mechator_reduced[2]);

            RenderSystem::sprite(tl.x() + delta_x_island_1 / 4, tl.y() + delta_y_island_1 / 4, island_1_reduced, island_1_reduced[2]);
            RenderSystem::sprite(tl.x() + delta_x_island_2 / 4, tl.y() + delta_y_island_2 / 4, island_2_reduced, island_2_reduced[2]);
            RenderSystem::sprite(tl.x() + delta_x_island_3 / 4, tl.y() + delta_y_island_3 / 4, island_3_reduced, island_3_reduced[2]);
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

            int color;
            for (int i = 0; i < door_count; ++i) {
                if (GameVariables::doorStates()[i] == Discovered) {
                    color = SpawnPoint::door_labels[i] == POIType::DoorA ? 48 : SpawnPoint::door_labels[i] == POIType::DoorB ? 32 : 16;
                    Vec2f pos = doors[i].pos();
                    pos *= world_to_loc;
                    RenderSystem::drawRect(tl.x() + pos.x() - 1, tl.y() + pos.y() - 1, 3, 2, color);
                }
            }

            Vec2i goal = GameVariables::getGoal();
            if (goal.x() > -200) {
                Vec2f pos = Vec2f(goal.x() * 54.0f/216.0f, goal.y() * 54.0f/216.0f);
                RenderSystem::sprite(tl.x() + pos.x() - 3.5f, tl.y() + pos.y() - 3.5f, poi[!goal_blink.active() ? 1 : 4], 0);
            }
            Helpers::drawNotchedRect(81, y + h/2 - 14, 28, 29, 0);
            if (door_blink.active()) {
                RenderSystem::drawRect(85, y + h/2 + 8, 4, 3, 48);
            } else if (door_blink.active(1.2f)) {
                RenderSystem::drawRect(85, y + h/2 + 8, 4, 3, 16);
            } else {
                RenderSystem::drawRect(85, y + h/2 + 8, 4, 3, 32);
            }
            if (playerloc_blink.active()) RenderSystem::pixel(86, y + h/2 - 9, 10);
            RenderSystem::print(93, y + h/2 - 12, "loc", 10);

            RenderSystem::sprite(83, y + h/2 - 3, poi[!goal_blink.active() ? 1 : 4], 0);
            RenderSystem::print(92, y + h/2 - 4, "goal", 10);

            RenderSystem::print(92, y + h/2 + 5, "door", 10);
        }
    });
    title_prompt.draw(true, [](int16_t x, int16_t y, int16_t w, int16_t h){
        if (h > 10) {
            Helpers::printHorizontallyCentered(x + w/2, y + 2, "Map", 10);
        }
    });
}
