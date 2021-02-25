#include <math.h>

#include "core/rendering/rendersystem.h"
#include "core/controls/controls.h"
#include "core/utilities/babyfsm.h"
#include "core/utilities/fpshelper.h"

#include "game/rendering/camera.h"
#include "game/player.h"
#include "game/utilities/mapmanager.h"
#include "game/entities/projectile.h"

#include "core/rendering/screenbuffer.h"
#include "game/entities/effects.h"

RenderSystem renderSystem;
Player player;
ScreenBuffer screenbuffer;

void updateState(FSM &fsm) {
    player.update(0.014f);
    ProjectileManager::update(0.014f);
    EffectManager::update(0.014f);
}

void drawState() {
    static FPSHelper fps(10);

    Camera::update(player.pos().x(), player.pos().y());

//    MapManager::draw(&renderSystem);
    MapManager::draw(&screenbuffer);
    renderSystem.drawBuffer(screenbuffer.getData());
    player.draw(&renderSystem);
    ProjectileManager::draw(&renderSystem);

    MapManager::draw(&renderSystem, false);
    EffectManager::draw(&renderSystem);

    fps.update(&renderSystem);
    fps.draw(&renderSystem, 2, 82, 9);
    fps.draw(&renderSystem, 2, 81, 37);
}

int main ()
{
    MapManager::setBackground(BGMap::Approach);
    renderSystem.initialize();

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
