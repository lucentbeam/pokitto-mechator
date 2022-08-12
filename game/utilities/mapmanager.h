#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include "core/rendersystem.h"
#include "core/rendering/tilemap.h"
#include "core/rendering/camera.h"
#include "game/rendering/skytilemap.h"
#include "game/utilities/screentilestore.h"

typedef Tilemap<6,6> BackgroundMap;

class MapManager
{
    static BackgroundMap s_background, s_island1, s_island2, s_island3;

    static SkyTilemap s_foreground;

    static ScreenTileStore s_camera_tiles;

    static uint8_t getTileAtPvt(float x, float y);
public:
    static void update();

    static void draw(bool bg);

    static void draw(ScreenBuffer * buffer);

    static int getMapIndex(float x, float y);

    static uint8_t getTileAt(float x, float y);

    static void setTileAt(float x, float y, uint8_t override);

    static void loadMutables(char * file);

    static void dumpMutables(char * file);

    static void resetMutables();
};

#endif // MAPMANAGER_H
