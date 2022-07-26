#include "eventscene.h"

#include "core/rendering/camera.h"

#include "game/enums.h"
#include "game/constants.h"

#include "game/states/game.h"
#include "game/player.h"
#include "game/maps/regiontransitionhandler.h"

const SceneSequence * EventScene::s_active_sequence = nullptr;

int EventScene::s_counter;

EventScene::Data EventScene::s_data;

void EventScene::updateMove()
{
    if (Camera::atMovementDestination()) {
        goNext();
    }
}

void EventScene::updateShowDialogue()
{
    ControlStatus ctrl = Controls::getStatus();
    if (s_data.text_tween.done(100) && ctrl.a.pressed()) {
        if (getDialogue()->close) {
            s_data.dialogue_box.setVisibility(false);
        } else {
            goNext();
        }
    } else if (s_data.dialogue_box.hidden()) {
        goNext();
    } else if (ctrl.a.held() && !s_data.text_tween.done()) {
        s_data.text_tween.advance(physicsTimestepMs * 2);
    } else if (ctrl.b.pressed() && !s_data.text_tween.done()) {
        s_data.text_tween.advance(20000);
    }
}

void EventScene::updateWait()
{
    s_data.counter--;
    if (s_data.counter <= 0) {
        goNext();
    }
}

void EventScene::updateDoFunction()
{
    if (getFunc()->callback()) {
        goNext();
    }
}

void EventScene::goNext()
{
    s_counter++;
    switch(active().type) {
    case SceneSequence::MoveCamera:
        if (getMove()->to_player) {
            Camera::moveTo(Player::position(), getMove()->velocity);
        } else {
            Camera::moveTo(getMove()->target, getMove()->velocity);
        }
        break;
    case SceneSequence::Wait:
        s_data.counter = ((SceneWait*)(active().data))->ticks;
        break;
    case SceneSequence::DoFunction:

        break;
    case SceneSequence::ShowDialogue:
        s_data.dialogue_box.setVisibility(true);
        s_data.text_tween = Tween(float(getDialogue()->length())/s_data.text_lps);
        if (!s_data.dialogue_box.showing()) s_data.text_tween.reset(int32_t(uiEasingTimeMs + 50));
        else s_data.text_tween.reset(int32_t(150));
        break;
    case SceneSequence::End:
        FSM::instance->go(GameStates::Game);
        UI::showHealthbar();
        if (getFunc() != nullptr) {
            getFunc()->callback();
        }
        break;
    }
}

const SceneFunc *EventScene::getFunc() { return ((SceneFunc*)(active().data)); }

const SceneMoveCam *EventScene::getMove() { return ((SceneMoveCam*)(active().data)); }

const SceneDialogue *EventScene::getDialogue() { return ((SceneDialogue*)(active().data)); }

void EventScene::startScene(const SceneSequence *sequence)
{
    s_active_sequence = sequence;
    s_counter = -1;
    FSM::instance->go(GameStates::EventState);
    UI::hideHealthbar();
    goNext();
    s_data.text_lps = baseTextSpeedLPS;
}

void EventScene::update(FSM &fsm)
{
    void(*fncs[])() = {updateMove, updateShowDialogue, updateWait, updateDoFunction };
    fncs[int(s_active_sequence[s_counter].type)]();

    if (s_data.has_update_func) {
        s_data.has_update_func = !s_data.update_callback();
    }
    RegionTransitionHandler::update();
    updateRegionIndicator();
    Camera::update(Player::position().x(), Player::position().y());
    MapManager::update();
}

void EventScene::draw()
{
    drawGameState();
    if (active().type == SceneSequence::ShowDialogue) {
        s_data.dialogue_box.draw(true, [](int16_t x, int16_t y, int16_t, int16_t h) {
            if (h > 20) {
                Helpers::drawNotchedRect(x + 2, y + 2, 17, 17, 1);
                static int counter = 0;
                counter++;

                uint8_t spr[2 + 15 * 15];
                switch(getDialogue()->portrait) {
                case SceneDialogue::Base:
                    spr[0] = 15;
                    spr[1] = 15;
                    std::memset(spr + 2, 1, 15*15);
                    {
                        int ct = rand() % 3;
                        while (ct < 15*15-1) {
                            static const int c[4] = { 2, 3, 6, 50};
                            spr[2 + ct] = c[rand() % 4];
                            ct += 2 + (rand() % 4);
                        }
                    }
                    break;
                case SceneDialogue::EnemyPilot:
                    std::memcpy(spr, portrait_enemy[(counter % asCounts(1.66f)) > asCounts(1.0f) ? 1 : 0], 2 + 15*15);
                    break;
                case SceneDialogue::EnemyCPU:
                    {
                        static int beep_frame = 1;
                        int ctr = counter % asCounts(2.5f);
                        if (ctr == asCounts(1.6f)) {
                            beep_frame = 1 + (rand() % 4);
                        }
                        std::memcpy(spr, portrait_enemy_cpu[(ctr > asCounts(1.6f) && (ctr < asCounts(1.93f))) || (ctr > asCounts(2.17f)) ? beep_frame : 0], 2 + 15*15);

                    }
                    break;
                }

                static const int ct = 3;
                static int gy[ct] = {1, 7, 12};
                static int gh[ct] = {1, 3, 2};
                static int of[ct] = {1, -1, 1};

                static int freq = 10 + (rand() % 40);
                if (counter % freq == 0) {
                    for(int i = 0; i < ct; ++i) {
                        gy[i] = 1 + (rand() % 12);
                        gh[i] = 1 + (rand() % 3);
                        of[i] = (rand() % 2) == 0 ? 1 : - 1;
                        if (rand() % 8 == 0) of[i] *= 2;
                    }
                }
                if (counter % freq < 8) {
                    for(int i = 0; i < ct; ++i) {
                        std::memmove(spr + 2 + gy[i] * 15, spr + 2 + gy[i] * 15 + of[i], gh[i] * 15);
                    }
                }
                if (counter % asCounts(3.0f) == 0) {
                    freq = asCounts(0.66f) + (rand() % asCounts(2.33f));
                }
                RenderSystem::sprite(x + 3, y + 3, spr);


                int letter_ct = s_data.text_tween.getInterpolationInt(0, getDialogue()->length());
                char helper[30] = "";
                if (getDialogue()->twoLines()) {
                    if (letter_ct > std::strlen(getDialogue()->text1)) {
                        RenderSystem::print(x + 22, y + 2, getDialogue()->text1, 9);
                        letter_ct -= std::strlen(getDialogue()->text1);
                        std::strncpy(helper, getDialogue()->text2, letter_ct);
                        RenderSystem::print(x + 22, y + 11, helper, 9);
                    } else {
                        std::strncpy(helper, getDialogue()->text1, letter_ct);
                        RenderSystem::print(x + 22, y + 2, helper, 9);
                    }
                } else {
                    std::strncpy(helper, getDialogue()->text1, letter_ct);
                    RenderSystem::print(x + 22, y + 6, helper, 9);
                }

                if (s_data.text_tween.done(100)) {
                    Helpers::drawNotchedRect(105, 80, 6, 7, 10);
                    RenderSystem::print(107, 80, ">", 1);
                }
            }
        });
    }
}

void EventScene::registerUpdate(std::function<bool ()> fnc)
{
    s_data.update_callback = fnc;
    s_data.has_update_func = true;
}

void EventScene::setTextSpeed(float lps)
{
    s_data.text_lps = lps;
}
