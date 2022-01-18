#ifndef SCENEOBJECTS_H
#define SCENEOBJECTS_H

#include <cstring>

#include "core/utilities/vec.h"

struct SceneSequence {
    enum Type {
        MoveCamera,
        ShowDialogue,
        Wait,
        DoFunction,
        End
    } type;

    const void * data;
};

struct SceneWait {
    int ticks;
    constexpr SceneWait(int t) : ticks(t) {}
};

struct SceneMoveCam {
    float velocity;
    Vec2f target;
    bool to_player;
    constexpr SceneMoveCam(float v, const Vec2f t) : velocity(v * 6.0f), target(t * 6.0f), to_player(false) {}
    constexpr SceneMoveCam(float v) : velocity(v * 6.0f), target(0,0), to_player(true) {}
};

struct SceneFunc {
    bool(*callback)();
    constexpr SceneFunc(bool(*c)()) : callback(c) {}
};

struct SceneDialogue {
    const char * const text1;
    const char * const text2;
    bool enemy;
    bool close;
    bool twoLines() const { return text2 != nullptr; }
    int length() const { return std::strlen(text1) + (text2 == nullptr ? 0 : std::strlen(text2)); }
    constexpr SceneDialogue(const char * const txt, const char * const txt2 = nullptr, bool e = true, bool c = true) : text1(txt), text2(txt2), enemy(e), close(c) {}
};

#endif // SCENEOBJECTS_H
