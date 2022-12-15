#include "regiontransitionhandler.h"

#include "core/audiosystem.h"
#include "core/rendersystem.h"
#include "core/palettes.h"
#include "game/constants.h"

#include "game/utilities/blinker.h"
#include "game/alertpalettes.h"
#include "game/maps/sequencetrigger.h"

struct TransitionHelper {
    int waitCounter = 60;
    const int waitTime = 60;
    int transitionCounter = 20;
    const int transitionTime = 20;

    float lastTarget = 0.8f;
    float nextTarget = 1.0f;
    bool update(float &frac) {
        if (waitCounter > 0) {
            waitCounter--;
            frac = lastTarget;
            return false;
        } else {
            transitionCounter--;
            if (transitionCounter == 0) {
                lastTarget = nextTarget;
                nextTarget = 0.6f + 0.004f * float(rand() % 101);
                waitCounter = waitTime;
                transitionCounter = transitionTime;
                frac = lastTarget;
                return true;
            } else {
                float f = (float((transitionTime - transitionCounter)) / transitionTime);
                frac = f * nextTarget + (1.0 - f) * lastTarget;
                return true;
            }
        }
    }
};

const int transitionTime = 90;

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

void RegionTransitionHandler::updateBoat()
{
    static bool active = false;
    static bool was_active = false;
    active = s_state.transition_trackers[int(Boating)] > 0;
    if (active && !was_active) {
        AudioSystem::playSong(musOcean);
    }
    was_active = active;
}

void RegionTransitionHandler::updateOverworld()
{
    static bool active = false;
    static bool was_active = false;
    active = s_state.transition_trackers[int(Overworld)] > 0;
    if (active && !was_active) {
        AudioSystem::playSong(SequenceTrigger::tutorialComplete() ? musOverworldMain : musOverworld);
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

    static int counter = 30;
    static bool count_down = true;
    if (active && s_state.transition_trackers[int(FinalBoss)] < transitionTime) {
        float frac = float(s_state.transition_trackers[int(FinalBoss)]) / float(transitionTime);
        RenderSystem::setPaletteLerped(default_palette, palette_14, frac);
    } else if (!active) {
        RenderSystem::setPalette(default_palette);
    } else {
        if (count_down) {
            counter--;
            count_down = counter > 0;
        } else {
            counter++;
            count_down = counter == 30;
        }
        RenderSystem::setPaletteLerped(default_palette, palette_14, float(counter)/30.0f);
    }
}

void RegionTransitionHandler::updateSwamp()
{
    static bool active = false;
    static bool was_active = false;
    active = s_state.transition_trackers[int(Swamp)] > 0;
    if (active && !was_active) {
        AudioSystem::playSong(musSwamp);
    }
    was_active = active;

    static TransitionHelper transitions;
    if (active && s_state.transition_trackers[int(Swamp)] < transitionTime) {
        float frac = float(s_state.transition_trackers[int(Swamp)]) / float(transitionTime);
        float t;
        transitions.update(t);
        RenderSystem::setPaletteLerped(default_palette, 255, 255, 255, t * frac);
    } else if (!active) {
        RenderSystem::setPalette(default_palette);
    } else {
        float t;
        if (transitions.update(t)) RenderSystem::setPaletteLerped(default_palette, 255, 255, 255, t);
    }
}

void RegionTransitionHandler::updateNorthBase()
{
    static bool active = false;
    static bool was_active = false;
    active = s_state.transition_trackers[int(NorthBase)] > 0;
    if (active && !was_active) {
        AudioSystem::playSong(musOverworld);
    }
    was_active = active;
}

void RegionTransitionHandler::updateCanyons()
{
    static bool active = false;
    static bool was_active = false;
    active = s_state.transition_trackers[int(Canyons)] > 0;
    if (active && !was_active) {
        AudioSystem::playSong(musCanyon);
    }
    was_active = active;

    static TransitionHelper transitions;
    if (active && s_state.transition_trackers[int(Canyons)] < transitionTime) {
        float frac = float(s_state.transition_trackers[int(Canyons)]) / float(transitionTime);
        float t;
        transitions.update(t);
        RenderSystem::setPaletteLerped(default_palette, light2_palette, t * frac);
    } else if (!active) {
        RenderSystem::setPalette(default_palette);
    } else {
        float t;
        if (transitions.update(t)) RenderSystem::setPaletteLerped(default_palette, light2_palette, t);
    }
}

void RegionTransitionHandler::goRegion(RegionNames name)
{
    if (s_state.in_boss) return;
    if (Player::mode() == BoatMode) {
        if (s_state.status == Boating) return;
        s_state.previous = s_state.status;
        s_state.status = Boating;
    } else if (name == RegionStormyCape) {
        if (s_state.status == Peninsula) return;
        s_state.previous = s_state.status;
        s_state.status = Peninsula;
    } else if (name == RegionFoggySwamp) {
        if (s_state.status == Swamp) return;
        s_state.previous = s_state.status;
        s_state.status = Swamp;
    } else if (name == RegionNorthBase) {
        if (s_state.status == NorthBase) return;
        s_state.previous = s_state.status;
        s_state.status = NorthBase;
    } else if (name == RegionScorchedCanyons) {
        if (s_state.status == Canyons) return;
        s_state.previous = s_state.status;
        s_state.status = Canyons;
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
    clear();
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
        case Swamp:
            updateSwamp();
            return;
        case NorthBase:
            updateNorthBase();
            return;
        case Canyons:
            updateCanyons();
            return;
        case Boating:
            updateBoat();
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
        case Swamp:
            updateSwamp();
            return;
        case NorthBase:
            updateNorthBase();
            return;
        case Canyons:
            updateCanyons();
            return;
        case Boating:
            updateBoat();
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
    updateSwamp();
    updateNorthBase();
    updateCanyons();
    updateBoat();
}
