#include "cloudmanager.h"
#include "game/tilesets.h"
#include "core/rendering/camera.h"

Cloud CloudManager::s_clouds[4];

void CloudManager::update(float dt)
{
    static int cam_x = Camera::tl_x();
    static int cam_y = Camera::tl_y();
    int new_x = Camera::tl_x();
    int new_y = Camera::tl_y();

    for(Cloud &c : s_clouds) {
        c.x -= 0.5 * dt;
        c.x -= float(new_x - cam_x) * 1;
        c.y -= float(new_y - cam_y) * 1;
        if (c.x < -48) {
            c.x = 110;
            c.reset(false,true);
        } else if (c.x > 110) {
            c.x = -46;
            c.reset(false,true);
        }
        if (c.y < -36) {
            c.y = 88;
            c.reset(true, false);
        } else if (c.y > 88) {
            c.y = -34;
            c.reset(true, false);
        }
    }
    cam_x = new_x;
    cam_y = new_y;
}

void CloudManager::draw()
{
    for(const Cloud &c : s_clouds) {
        RenderSystem::drawShadow(c.x, c.y, clouds[c.index], clouds[c.index][2]);
    }
}
