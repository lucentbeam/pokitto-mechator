#ifndef EVENTSCENE_H
#define EVENTSCENE_H

#include <cstring>

#include "core/utilities/babyfsm.h"
#include "core/utilities/vec.h"

#include "game/ui/ui.h"

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

class EventScene
{
    static const SceneSequence * s_active_sequence;
    static int s_counter;

    static struct Data {
        int counter;
        UIElement dialogue_box = UIElement::getExpander(55, 74, 108, 21, Tween::OutQuad);
        Tween text_tween = Tween(0.0f);
    } s_data;

    static void updateMove();
    static void updateShowDialogue();
    static void updateWait();
    static void updateDoFunction();

    static void goNext();

    static const SceneSequence &active() { return s_active_sequence[s_counter]; }

    static const SceneFunc * getFunc();
    static const SceneMoveCam * getMove();
    static const SceneDialogue * getDialogue();
public:
    static void startScene(const SceneSequence * sequence);

    static void update(FSM &fsm);

    static void draw();
};

#endif // EVENTSCENE_H
