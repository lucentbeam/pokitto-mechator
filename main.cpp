#include <math.h>

#include "core/rendering/rendersystem.h"
#include "core/controls/controls.h"

#include "core/utilities/babyfsm.h"

// testing stuff
#include "game/tilesets.h"
#include "core/rendering/tilemap.h"
#include "game/maps/mechator.h"

#include "core/utilities/fpshelper.h"

#include "game/rendering/camera.h"
#include "game/player.h"

RenderSystem renderSystem;
Tilemap<6,6> tileset(jungletiles, approach);

Camera camera;
Player player(6*20,6*48);

void updateState(FSM &fsm) {
    player.update(0.014f);
}

void drawState() {
    static FPSHelper fps(10);

    camera.update(player.x(), player.y(), tileset.mapWidth(), tileset.mapHeight(), tileset.tileWidth(), tileset.tileHeight());

    tileset.draw(&renderSystem, camera.tl_x() / 6, camera.tl_y() / 6, camera.render_width, camera.render_height, camera.offset_x(), camera.offset_y());
    player.draw(&renderSystem, camera);

    fps.update(&renderSystem);
    fps.draw(&renderSystem, 2, 82, 9);
    fps.draw(&renderSystem, 2, 81, 37);
}

int main ()
{
    renderSystem.initialize();

    Controls controls;

    FSM fsm;
    fsm.add(0, updateState, drawState);

    int32_t gameTime = 0;
    uint32_t lastGameTime = renderSystem.getTimeMs();

    const uint8_t physicsTimestepMs = 14;
    const uint8_t maxPhysicsStepsPerFrame = 5; // prevent death spiral
    
    while (renderSystem.running())
    {
        bool draws = renderSystem.update();

        gameTime += renderSystem.getTimeMs() - lastGameTime;
        lastGameTime = renderSystem.getTimeMs();
        uint8_t step_counter = 0;
        while (gameTime > 0 && step_counter < maxPhysicsStepsPerFrame) {
            fsm.update();
            gameTime -= physicsTimestepMs;
            step_counter++;
        }

        if (draws) {
            fsm.draw(); // TODO: add frame interpolation?
        }
    }

    return 0;
}
