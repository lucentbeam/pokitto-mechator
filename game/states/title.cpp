#include "title.h"

#include <cmath>

#include "core/rendersystem.h"
#include "core/palettes.h"
#include "core/controls.h"
#include "game/constants.h"

#include "game/sprites.h"
#include "game/states/game.h"

Title::TitleState Title::s_state = Title::Select;

int Title::select_index = 0;

int Title::data_index = 0;

bool Title::has_data[3] = {false, false, false};

float Title::timer = 0.0f;

void Title::go()
{
    select_index = 0;
    data_index = 0;
    s_state = Select;

    // deserialize game saves

    RenderSystem::setPalette(default_palette);
}

void Title::update(FSM &fsm)
{
    timer += physicsTimestep * 10.0f;
    if (select_index > 0 && Controls::getStatus().up.pressed()) {
        select_index = 0;
    } else if (select_index == 0 && Controls::getStatus().down.pressed()) {
        select_index = 1;
    }
    if (Controls::getStatus().a.pressed()) {
        goGame();
    }
}

void Title::draw()
{
    RenderSystem::clear(50);

    RenderSystem::drawRect(0, 0, 7, 88, 0);
    RenderSystem::drawRect(103, 0, 7, 88, 0);


    const float spacing = 34.0f;
    float start = std::fmod(-timer, spacing);
    while (start < 88) {
        RenderSystem::drawRect(6, start, 98, 6, 0);
        start += spacing;
    }

    RenderSystem::print(93, 80, "v0.8", 6);

    RenderSystem::sprite(6, 6, title_text, 0);

    RenderSystem::print(35, 32, "CONTINUE", select_index == 0 ? 10 : 8);
    RenderSystem::print(35, 41, "NEW GAME", select_index == 1 ? 10 : 8);
    RenderSystem::sprite(27, 32 + 1 + select_index * 9, ui_arrow_left, 0, true);

    RenderSystem::sprite(0, 58, title_jeep, 0);
    RenderSystem::sprite(58, 82, title_dirt, 0);
    RenderSystem::sprite(65, 60, title_shot_1, 0);
    RenderSystem::sprite(82, 64, title_shot_2, 0);
}
