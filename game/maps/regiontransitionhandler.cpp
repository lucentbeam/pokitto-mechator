#include "regiontransitionhandler.h"

#include "core/audiosystem.h"
#include "core/rendersystem.h"
#include "core/palettes.h"
#include "game/constants.h"

#include "game/utilities/blinker.h"
#include "game/alertpalettes.h"

const int transitionTime = 25;

RegionTransitionHandler::State RegionTransitionHandler::s_state;

void RegionTransitionHandler::updateBoss()
{
    static bool active = false;
    static bool was_active = false;
    active = s_state.transition_trackers[int(Boss)] > 0;
    if (active && !was_active) {
        AudioSystem::playSong(musBoss);
    }
    was_active = active;
}

void RegionTransitionHandler::updateOverworld()
{
    static bool active = false;
    static bool was_active = false;
    active = s_state.transition_trackers[int(Overworld)] > 0;
    if (active && !was_active) {
        AudioSystem::playSong(musOverworld);
    }
    was_active = active;
}

void RegionTransitionHandler::updatePeninsula()
{
    static bool active = false;
    static bool was_active = false;
    active = s_state.transition_trackers[int(Peninsula)] > 0;
    if (active && !was_active) {
        AudioSystem::playSong(musCape);
    }
    static Blinker capeLightning(6.0f, 0.3);
    static float paletteIndex = 2;
    if (s_state.transition_trackers[int(Peninsula)] < transitionTime) {
        paletteIndex = 2.0f + 4.0f * float(s_state.transition_trackers[int(Peninsula)]) / float(transitionTime);
    } else {
        bool flashing = capeLightning.active();
        capeLightning.update();
        if (capeLightning.active()) {
            if (!flashing) {
                AudioSystem::play(sfxExplosionBig);
            }
            paletteIndex = 0;
        } else {
            paletteIndex += physicsTimestep * 12.0f;
            if (paletteIndex > 5) paletteIndex = 5;
            if (flashing) {
                capeLightning = Blinker(0.35f + float(rand() % 100)/100.0f * 12.0f, 0.1f + float(rand() % 100)/100.0f * 0.2f);
            }
        }
    }
    RenderSystem::setPalette(palette_list[int(paletteIndex)]);
    was_active = active;
}

void RegionTransitionHandler::updateFinalBoss() {
    static bool active = false;
    static bool was_active = false;
    active = s_state.transition_trackers[int(FinalBoss)] > 0;
    if (active && !was_active) {
        AudioSystem::playSong(musBoss);
    }
    was_active = active;

    static float position = 0.0f;
    position += physicsTimestep * 40.0f;
    int idx = int(position) % 31;
    if (idx > 15) {
        idx = 30 - idx;
    }
    const uint16_t * palettes[] = {default_palette, palette_0, palette_1, palette_2, palette_3, palette_4, palette_5, palette_6, palette_7, palette_8, palette_9, palette_10, palette_11, palette_12, palette_13, palette_14};
    RenderSystem::setPalette(palettes[idx]);
}

void RegionTransitionHandler::goRegion(RegionNames name)
{
    if (s_state.in_boss) return;
    if (name == RegionStormyCape) {
        if (s_state.status == Peninsula) return;
        s_state.previous = s_state.status;
        s_state.status = Peninsula;
    } else {
        if (s_state.status == Overworld) return;
        s_state.previous = s_state.status;
        s_state.status = Overworld;
    }
}

void RegionTransitionHandler::goBoss(bool final)
{
    s_state.in_boss = true;
    s_state.previous = s_state.status;
    s_state.status = final ? FinalBoss : Boss;
}

void RegionTransitionHandler::leaveBoss()
{
    s_state.in_boss = false;
    s_state.status = s_state.previous;
}

void RegionTransitionHandler::update()
{
    if (s_state.transition_trackers[s_state.previous] > 0) {
        s_state.transition_trackers[s_state.previous]--;
        AudioSystem::setMusicFraction(float(s_state.transition_trackers[s_state.previous])/float(transitionTime));
        switch (s_state.previous) {
        case Overworld:
            updateOverworld();
            return;
        case Peninsula:
            updatePeninsula();
            return;
        case Boss:
            updateBoss();
            return;
        case FinalBoss:
            updateFinalBoss();
            return;
        default:
            return;
        }
    }
    if (s_state.transition_trackers[s_state.previous] <= 0 && s_state.transition_trackers[s_state.status] < transitionTime) {
        s_state.transition_trackers[s_state.status]++;
        AudioSystem::setMusicFraction(float(s_state.transition_trackers[s_state.status])/float(transitionTime));
    }
    if (s_state.transition_trackers[s_state.status] > 0) {
        switch (s_state.status) {
        case Overworld:
            updateOverworld();
            return;
        case Peninsula:
            updatePeninsula();
            return;
        case Boss:
            updateBoss();
            return;
        case FinalBoss:
            updateFinalBoss();
            return;
        default:
            return;
        }
    }
}

void RegionTransitionHandler::clear()
{
    s_state = State();
    updateOverworld();
    updateBoss();
    updateFinalBoss();
    updatePeninsula();
}
