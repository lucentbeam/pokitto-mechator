#ifndef FUNCS_H
#define FUNCS_H

#include "core/rendering/camera.h"

inline void onBarracksExplode() {
    Camera::shake(0.4f, 0.75f);
}

inline void onVehicleDestroyed() {
    Camera::shake(0.5f, 0.5f);
}

inline void onTurretDestroyed() {
    Camera::shake(0.2f, 0.4f);
}

inline void onBossDestroyed() {
    Camera::shake(0.8f, 0.75f);
}

inline void onPlayerMissileExplode() {
    Camera::shake(0.1f, 0.35f);
}

inline void onEnemyMissileExplode() {
//    Camera::shake(0.2f, 0.25f);
}

#endif // FUNCS_H
