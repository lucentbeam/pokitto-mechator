#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include "core/rendering/rendersystem.h"
#include "core/rendering/tilemap.h"
#include "game/rendering/camera.h"
#include "game/maps/mechator.h"
#include "game/tilesets.h"

typedef Tilemap<6,6> BackgroundMap;

enum BGMap {
    Approach = 0
};

class MapManager
{
    static BackgroundMap s_backgrounds[];

    static BackgroundMap * s_activeBackground;

public:

    static void draw(RenderSystem * renderer);

    static void draw(ScreenBuffer * buffer);

    static void setBackground(BGMap idx);

    static uint8_t getTileAt(float x, float y);

    static void setTileAt(float x, float y, uint8_t override);
};

#endif // MAPMANAGER_H
