#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include "core/rendersystem.h"
#include "core/rendering/tilemap.h"
#include "core/rendering/camera.h"
#include "game/rendering/skytilemap.h"

typedef Tilemap<6,6> BackgroundMap;

class MapManager
{
    static BackgroundMap s_background;

    static SkyTilemap s_foreground;

public:

    enum SpecialTiles : uint8_t {
        UnexplodedOrdinance = 63,
        ExplodedOrdinance = 64,
        DestroyedTurret = 185
    };

    static void draw(bool bg);

    static void draw(ScreenBuffer * buffer);

    static uint16_t getMapIndex(float x, float y);

    static uint8_t getTileAt(float x, float y);

    static void setTileAt(float x, float y, uint8_t override);
};

#endif // MAPMANAGER_H
