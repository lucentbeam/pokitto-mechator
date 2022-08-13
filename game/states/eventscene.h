#ifndef EVENTSCENE_H
#define EVENTSCENE_H

#include <cstring>
#include <functional>

#include "core/utilities/babyfsm.h"
#include "core/utilities/vec.h"

#include "game/ui/ui.h"

#include "game/utilities/sceneobjects.h"

class EventScene
{
    static const SceneSequence * s_active_sequence;
    static int s_counter;

    static struct Data {
        int counter;
        UIElement dialogue_box = UIElement::getExpander(55, 74, 108, 21, Tween::OutQuad);
        Tween text_tween = Tween(0.0f);
        bool has_update_func = false;
        std::function<bool()> update_callback;
        float text_lps = 25.0f;
    } s_data;

    static void updateMove();
    static void updateShowDialogue();
    static void updateWait();
    static void updateDoFunction();
    static void updateSetQuest();

    static void goNext();

    static const SceneSequence &active() { return s_active_sequence[s_counter]; }

    static const SceneFunc * getFunc();
    static const SceneMoveCam * getMove();
    static const SceneDialogue * getDialogue();
    static const SceneSetQuest * getSetQuest();
public:
    static void startScene(const SceneSequence * sequence);

    static void update(FSM &fsm);

    static void draw();

    static void registerUpdate(std::function<bool()> fnc);

    static void setTextSpeed(float lps);
};

#endif // EVENTSCENE_H
