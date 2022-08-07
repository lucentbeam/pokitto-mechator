#include "title.h"

#include "core/rendersystem.h"
#include "core/palettes.h"
#include "core/controls.h"

#include "game/sprites.h"

Title::TitleState Title::s_state = Title::Select;

int Title::select_index = 0;

int Title::data_index = 0;

bool Title::has_data[3] = {false, false, false};

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
    if (select_index > 0 && Controls::getStatus().up.pressed()) {
        select_index = 0;
    } else if (select_index == 0 && Controls::getStatus().down.pressed()) {
        select_index = 1;
    }
}

void Title::draw()
{
    RenderSystem::clear(0);
    RenderSystem::print(18, 10, "MECHATOR", 4, 2);

    RenderSystem::print(28, 40, "CONTINUE", 3);
    RenderSystem::print(28, 52, "NEW GAME", 3);
    RenderSystem::sprite(20, 40 + select_index * 12 + 1, ui_arrow_left, 0, true);
}
