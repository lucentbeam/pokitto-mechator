#include "variables.h"

#include "core/serialization.h"
#include "game/maps/sequencetrigger.h"

#ifdef DEBUGS
bool DebugOptions::noclip = false;
#endif

bool GameVariables::gameWon = false;

GameStorage GameVariables::s_data;

char GameVariables::savefile[30] = "/data/mechator/save1.dat";

void GameVariables::changeDollars(int16_t delta)
{
    s_data.dollarCount += std::fmax(delta, -s_data.dollarCount);
}

void GameVariables::changeDollarsBig(int8_t delta)
{
    s_data.dollarCount += 10;
}

void GameVariables::changeHackingKits(int8_t delta)
{
    s_data.hackingKitCount += delta;
}

void GameVariables::changeKeysA(int8_t delta)
{
    s_data.keyACount += delta;
}

void GameVariables::changeKeysB(int8_t delta)
{
    s_data.keyBCount += delta;
}

void GameVariables::changeKeysC(int8_t delta)
{
    s_data.keyCCount += delta;
}

bool GameVariables::hasBlueprint(int bp)
{
    return (s_data.acquiredBlueprints & (1 << bp)) > 0;
}

void GameVariables::acquireBlueprint(int bp)
{
    s_data.acquiredBlueprints = s_data.acquiredBlueprints | (1 << bp);
}

bool GameVariables::hasBlueprintUnlocked(int bp)
{
    return (s_data.unlockedBlueprints & (1 << bp)) > 0;
}

void GameVariables::unlockBlueprint(int bp)
{
    s_data.unlockedBlueprints = s_data.unlockedBlueprints | (1 << bp);
}

bool GameVariables::hasBlueprintToUnlock(int bp)
{
    return hasBlueprint(bp) && !hasBlueprintUnlocked(bp);
}

bool GameVariables::hasUnusedBlueprints()
{
    return s_data.acquiredBlueprints != s_data.unlockedBlueprints;
}

uint8_t GameVariables::keysA() { return s_data.keyACount; }

uint8_t GameVariables::keysB() { return s_data.keyBCount; }

uint8_t GameVariables::keysC() { return s_data.keyCCount; }

uint8_t GameVariables::hackingKits() { return s_data.hackingKitCount; }

uint16_t GameVariables::dollars() { return s_data.dollarCount; }

void GameVariables::visitEvent(int index) { s_data.header.visitedEvents[index] = true; }

bool GameVariables::eventVisited(int index) { return s_data.header.visitedEvents[index]; }

void GameVariables::setQuestStatus(uint8_t q)
{
    s_data.questStatus = q;
}

Vec2i GameVariables::getGoal()
{
    switch (QuestStatus(s_data.questStatus)) {
    case QuestIntro:
        return Vec2i(26, 48);
    case QuestTank:
        return Vec2i(26, 102);
    case QuestBoat:
        return Vec2i(160, 202);
    case QuestBuildBoat:
        return Vec2i(209, 32);
    case QuestHeli:
        return Vec2i(-20, 210);
    case QuestFinal:
        return Vec2i(103, 147);
    default:
        return Vec2i(-500, -500);
    }
}

void GameVariables::setGameWon()
{
    gameWon = true;
}

bool GameVariables::getGameWon()
{
    return gameWon;
}

void GameVariables::updateTime(int ms)
{
    s_data.header.elapsedMilliseconds += ms;
}

void GameVariables::saveGame()
{
    Serialization::tryStore<GameStorage>(savefile, &s_data);
}

DoorStatus *GameVariables::doorStates()
{
    return s_data.door_states;
}

int *GameVariables::activatedDoors()
{
    return s_data.activated_doors;
}

int *GameVariables::acquiredSpecials()
{
    return s_data.acquired_specials;
}

GameStorage *GameVariables::getData()
{
    return &s_data;
}

void GameVariables::loadGame(GameStorage s)
{
    s_data = s;
}

GameOptions GameOptions::s_options;

void GameOptions::initialize()
{
    s_options = GameOptions();
#ifdef DESKTOP_BUILD
    Serialization::tryGet<GameOptions>("../data/mechator/options.cfg", &s_options);
#else
    Serialization::tryGet<GameOptions>("/data/mechator/options.cfg", &s_options);
#endif
    setMusicOn(s_options.mus_on);
    setSfxOn(s_options.sfx_on);
}

void GameOptions::save()
{
#ifdef DESKTOP_BUILD
    Serialization::tryStore<GameOptions>("../data/mechator/options.cfg", &s_options);
#else
    Serialization::tryStore<GameOptions>("/data/mechator/options.cfg", &s_options);
#endif
}

bool GameOptions::musicOn()
{
    return s_options.mus_on;
}

void GameOptions::setMusicOn(bool value)
{
    s_options.mus_on = value;
    AudioSystem::setMusicOn(value);
}

bool GameOptions::sfxOn()
{
    return s_options.sfx_on;
}

void GameOptions::setSfxOn(bool value)
{
    s_options.sfx_on = value;
    AudioSystem::setSfxOn(value);
}

float GameOptions::volumeFrac()
{
    return AudioSystem::getVolume() / 100.0f;
}

void GameOptions::setVolumeFrac(float value)
{
    AudioSystem::setVolume(value < 0 ? 0 : value > 1 ? 100 : value * 100.0f);
}
