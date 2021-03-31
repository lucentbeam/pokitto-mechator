#ifndef CLOUDMANAGER_H
#define CLOUDMANAGER_H

#include "core/rendering/rendersystem.h"
#include "core/utilities/vec.h"

struct Cloud {
    float x, y;
    float vx;
    uint8_t index;

    Cloud() {
        reset(true,true);
    }

    void reset(bool do_x, bool do_y) {
        if (do_x) x = (rand() % 110) - 12;
        if (do_y) y = (rand() % 88) - 9;
        index = rand() % 3;
        vx = 4.0 + float(rand() % 300) / 100.0;
    }
};

class CloudManager
{
    static Cloud s_clouds[4];
public:

    static void update(float dt);

    static void draw();
};

#endif // CLOUDMANAGER_H
