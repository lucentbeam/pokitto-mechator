#ifndef SEQUENCES_H
#define SEQUENCES_H

#include "core/utilities/tween.h"
#include "core/audiosystem.h"

#include "game/player.h"
#include "game/utilities/sceneobjects.h"
#include "game/entities/enemy.h"
#include "game/states/eventscene.h"
#include "game/states/game.h"
#include "game/entities/barracks.h"
#include "game/constants.h"
#include "game/entities/pois.h"
#include "game/variables.h"
#include "game/maps/regiontransitionhandler.h"

const SceneWait wait60 = SceneWait(asCounts(1.0f));
const SceneWait wait30 = SceneWait(asCounts(0.5f));

extern Tween toLand;

const SceneSetQuest qintro = SceneSetQuest(QuestStatus::QuestIntro);
const SceneSetQuest qgettank = SceneSetQuest(QuestStatus::QuestTank);
const SceneSetQuest qgetboat = SceneSetQuest(QuestStatus::QuestBoat);
const SceneSetQuest qbuildboat = SceneSetQuest(QuestStatus::QuestBuildBoat);
const SceneSetQuest qgetheli = SceneSetQuest(QuestStatus::QuestHeli);
const SceneSetQuest qgoend = SceneSetQuest(QuestStatus::QuestFinal);

// after leaving tutorial island; set get tank
const SceneDialogue exit_tut[] = {
    { "Good work.", nullptr, SceneDialogue::Base, false },
    { "Now that you're", "off that island,", SceneDialogue::Base, false },
    { "head to the", "tank factory.", SceneDialogue::Base, false },
    { "We've marked it", "on your map.", SceneDialogue::Base, false },
    { "But you might", "need to explore", SceneDialogue::Base, false },
    { "and find some", "keycards first.", SceneDialogue::Base, false },
    { "Be sure to open", "your map from", SceneDialogue::Base, false },
    { "the menu and", "get your bearings.", SceneDialogue::Base, false },
    { "Central out.", nullptr, SceneDialogue::Base, true },
};

// get jeep
const SceneDialogue got_jeep[] = {
    { "You've acquired", "the jeep!", SceneDialogue::Base, false },
    { "It has two weapons:", "grenades or guns.", SceneDialogue::Base, false },
    { "Open your menu", "and select grenades.", SceneDialogue::Base, false },
    { "You'll need them", "to blast crates.", SceneDialogue::Base, false },
    { "But they won't", "hit air targets.", SceneDialogue::Base, false },
    { "Decide what you need", "in every situation.", SceneDialogue::Base, false },
    { "Now get moving!", "Central out.", SceneDialogue::Base, true },
};

// get jeep blueprint
const SceneDialogue got_jeep_bp[] = {
    { "You've picked up", "a blueprint.", SceneDialogue::Base, false },
    { "Now go upload it", "to a shop.", SceneDialogue::Base, false },
    { "Vehicles then need", "to be built.", SceneDialogue::Base, false },
    { "Other blueprints", "will apply effects", SceneDialogue::Base, false },
    { "automatically", "after uploading.", SceneDialogue::Base, true },
};

// first time entering a tank; set get boat
const SceneDialogue got_tank[] = {
    { "Excellent! You have", "the tank.", SceneDialogue::Base, false },
    { "Next, you'll need", "to get a boat.", SceneDialogue::Base, false },
    { "We have records", "of a boat factory", SceneDialogue::Base, false },
    { "at Stormy Cape.", "Your map is", SceneDialogue::Base, false },
    { "now updated.", "Good luck!", SceneDialogue::Base, true },
};

// first time entering a boat; set get heli
const SceneDialogue got_boat[] = {
    { "Nice going, soldier.", nullptr, SceneDialogue::Base, false },
    { "The final blueprint", "is the Helicopter.", SceneDialogue::Base, false },
    { "We've marked it", "on your map.", SceneDialogue::Base, false },
    { "But you'll need", "keys from other", SceneDialogue::Base, false },
    { "island bases around.", "Good luck.", SceneDialogue::Base, true },
};

const SceneFunc func0 = SceneFunc([](){ toLand.reset(); return true; });
const SceneFunc func1 = SceneFunc([](){
    int x = toLand.getInterpolationInt(-20, playerStartTileX * 6 + 3);
    int z = toLand.getInterpolationInt(60, 0);
    Player::setPosition(SoldierMode, {float(x), Player::position().y()});
    Soldier::setZ(z);
    return toLand.done();
});
const SceneDialogue dlog0 = SceneDialogue("Soldier!       ","Can you hear me?", SceneDialogue::Base, false);
const SceneDialogue dlog1 = SceneDialogue("This is central.", nullptr, SceneDialogue::Base, false);
const SceneDialogue dlog2 = SceneDialogue("A mainframe in this","area is glitching.", SceneDialogue::Base, false);
const SceneDialogue dlog2a = SceneDialogue("It is building and","mobilizing mechs.", SceneDialogue::Base, false);
const SceneDialogue dlog2b = SceneDialogue("You need to get to","it and \"reboot\" it.", SceneDialogue::Base, false);
const SceneDialogue dlog3 = SceneDialogue("Find its bases!       ","Hack and use them.", SceneDialogue::Base, false);
const SceneDialogue dlog4 = SceneDialogue("You can build your","own weapons...", SceneDialogue::Base, false);
const SceneDialogue dlog5 = SceneDialogue("...and infiltrate!",nullptr, SceneDialogue::Base, false);
const SceneDialogue dlog6 = SceneDialogue("Good luck.        ", "Central out.", SceneDialogue::Base, true);

const SceneSequence intro_scene[] = {
    {SceneSequence::DoFunction, &func0},
    {SceneSequence::DoFunction, &func1},
    {SceneSequence::Wait, &wait60 },
    {SceneSequence::ShowDialogue, &dlog0 },
    {SceneSequence::ShowDialogue, &dlog1 },
    {SceneSequence::ShowDialogue, &dlog2 },
    {SceneSequence::ShowDialogue, &dlog2a },
    {SceneSequence::ShowDialogue, &dlog2b },
    {SceneSequence::ShowDialogue, &dlog3 },
    {SceneSequence::ShowDialogue, &dlog4 },
    {SceneSequence::ShowDialogue, &dlog5 },
    {SceneSequence::ShowDialogue, &dlog6 },
    {SceneSequence::SetQuestStatus, &qintro},
    {SceneSequence::End, nullptr}
};

const SceneFunc tb_f0 = SceneFunc([](){
    Enemy::setMaxMechs(4, true);
    EnemyHelicopter * h = Enemy::createHelicopter({80 * 6, 14 * 6});
    EventScene::setTextSpeed(baseTextSpeedLPS / 2.0f);
    h->setMaxLife(18);
    RegionTransitionHandler::goBoss();
    UI::showBoss(h->getLifePtr());
    // set door behind player (218) on (51,19) through (51, 22)
    MapManager::setTileAt(51 * 6 + 3, 19 * 6 + 3, 218);
    MapManager::setTileAt(51 * 6 + 3, 20 * 6 + 3, 218);
    MapManager::setTileAt(51 * 6 + 3, 21 * 6 + 3, 218);
    MapManager::setTileAt(51 * 6 + 3, 22 * 6 + 3, 218);
    Barracks * b1 = Barracks::getBarracksAt({55, 11});
    Barracks * b2 = Barracks::getBarracksAt({69, 20});
    b1->disablePathfindingChecks();
    b2->disablePathfindingChecks();

    b1->disableDestroyOutOfRange();
    b2->disableDestroyOutOfRange();

    h->disableDestroyOutOfRange();

    registerCallback({h->getLifePtr(), b1->getLifePtr(), b2->getLifePtr()}, [](){
        // release door as grass (86)
        Enemy::setMaxMechs();
        RegionTransitionHandler::leaveBoss();
        MapManager::setTileAt(51 * 6 + 3, 19 * 6 + 3, 86);
        MapManager::setTileAt(51 * 6 + 3, 20 * 6 + 3, 86);
        MapManager::setTileAt(51 * 6 + 3, 21 * 6 + 3, 86);
        MapManager::setTileAt(51 * 6 + 3, 22 * 6 + 3, 86);
    });
    EventScene::registerUpdate([h](){
        h->tick();
        Vec2f d = Camera::center() - h->getSteering().pos();
        float l = d.length();
        if (l > 0) {
            d /= l;
        }
        if (l > 5) h->getSteering().update(physicsTimestep, d.x(), d.y());
        return false;
    });
    return true;
});
const SceneFunc cam_release = SceneFunc([](){ Camera::stopMovement(); return true; });
const SceneMoveCam cam_return = SceneMoveCam(cameraCutsceneSpeed);

const SceneFunc exit_tut_start = SceneFunc([]{
    RegionTransitionHandler::clear();
    return true;
});

const SceneSequence exit_tut_island[] = {
    { SceneSequence::DoFunction, &exit_tut_start },
    { SceneSequence::ShowDialogue, exit_tut },
    { SceneSequence::ShowDialogue, exit_tut + 1 },
    { SceneSequence::ShowDialogue, exit_tut + 2},
    { SceneSequence::ShowDialogue, exit_tut + 3},
    { SceneSequence::ShowDialogue, exit_tut + 4},
    { SceneSequence::ShowDialogue, exit_tut + 5},
    { SceneSequence::ShowDialogue, exit_tut + 6},
    { SceneSequence::ShowDialogue, exit_tut + 7},
    { SceneSequence::ShowDialogue, exit_tut + 8},
    { SceneSequence::SetQuestStatus, &qgettank },
    {SceneSequence::End, nullptr}
};

const SceneSequence acquiredjeep_scene[] = {
    { SceneSequence::ShowDialogue, got_jeep },
    { SceneSequence::ShowDialogue, got_jeep + 1 },
    { SceneSequence::ShowDialogue, got_jeep + 2},
    { SceneSequence::ShowDialogue, got_jeep + 3},
    { SceneSequence::ShowDialogue, got_jeep + 4},
    { SceneSequence::ShowDialogue, got_jeep + 5},
    { SceneSequence::ShowDialogue, got_jeep + 6},
    {SceneSequence::End, nullptr}
};

const SceneSequence acquiredtank_scene[] = {
    { SceneSequence::ShowDialogue, got_tank },
    { SceneSequence::ShowDialogue, got_tank + 1 },
    { SceneSequence::ShowDialogue, got_tank + 2},
    { SceneSequence::ShowDialogue, got_tank + 3},
    { SceneSequence::ShowDialogue, got_tank + 4},
    { SceneSequence::SetQuestStatus, &qgetboat },
    {SceneSequence::End, nullptr}
};

const SceneSequence acquiredboat_scene[] = {
    { SceneSequence::ShowDialogue, got_boat },
    { SceneSequence::ShowDialogue, got_boat + 1 },
    { SceneSequence::ShowDialogue, got_boat + 2},
    { SceneSequence::ShowDialogue, got_boat + 3},
    { SceneSequence::ShowDialogue, got_boat + 4},
    { SceneSequence::SetQuestStatus, &qgetheli },
    {SceneSequence::End, nullptr}
};


const SceneSequence acquiredblueprint_scene[] = {
    { SceneSequence::ShowDialogue, got_jeep_bp },
    { SceneSequence::ShowDialogue, got_jeep_bp + 1 },
    { SceneSequence::ShowDialogue, got_jeep_bp + 2},
    { SceneSequence::ShowDialogue, got_jeep_bp + 3},
    { SceneSequence::ShowDialogue, got_jeep_bp + 4},
    {SceneSequence::End, nullptr}
};

const SceneMoveCam tb_m0 = SceneMoveCam(cameraCutsceneSpeed, {65, 15});

const SceneDialogue tb_dlog0 = SceneDialogue("INFILTRATOR!",nullptr, SceneDialogue::EnemyPilot, false);
const SceneDialogue tb_dlog1 = SceneDialogue("YOU WILL GET", "NO FURTHER.", SceneDialogue::EnemyPilot, false);
const SceneDialogue tb_dlog2 = SceneDialogue("NOR FARTHER,   ", "FOR THAT MATTER.", SceneDialogue::EnemyPilot, true);

const SceneSequence tutorial_boss_scene[] = {
    {SceneSequence::DoFunction, &tb_f0},
    {SceneSequence::MoveCamera, &tb_m0 },
    {SceneSequence::Wait, &wait60 },
    {SceneSequence::ShowDialogue, &tb_dlog0 },
    {SceneSequence::ShowDialogue, &tb_dlog1 },
    {SceneSequence::ShowDialogue, &tb_dlog2 },
    {SceneSequence::MoveCamera, &cam_return },
    {SceneSequence::End, &cam_release}
};

const SceneMoveCam tt_m0 = SceneMoveCam(cameraCutsceneSpeed, {27, 99});
const SceneMoveCam tt_m1 = SceneMoveCam(cameraCutsceneSpeed * 2.3f, {27, 99});
const SceneMoveCam tt_m2 = SceneMoveCam(cameraCutsceneSpeed * 2.3f, {19, 94});

const SceneDialogue tt_dlog0 = SceneDialogue("IT IS THE","INFILTRATOR!", SceneDialogue::EnemyCPU, false);
const SceneDialogue tt_dlog1 = SceneDialogue("HERE FOR THE", "TANK BLUEPRINTS?", SceneDialogue::EnemyCPU, false);
const SceneDialogue tt_dlog2 = SceneDialogue("TOO BAD.", nullptr, SceneDialogue::EnemyCPU, false);
const SceneDialogue tt_dlog3 = SceneDialogue("THEY HAVE ALREADY", "BEEN RELOCATED.", SceneDialogue::EnemyCPU, false);
const SceneDialogue tt_dlog4 = SceneDialogue("NOW TO RELOCATE YOU.", nullptr, SceneDialogue::EnemyCPU, true);

const SceneDialogue tt_dlog5 = SceneDialogue("      ??????", nullptr, SceneDialogue::EnemyPilot, true);

const SceneDialogue tt_dlog6 = SceneDialogue("TO...    ", "    ...umm...", SceneDialogue::EnemyCPU, true);
const SceneDialogue tt_dlog7 = SceneDialogue("THE AFTERLIFE!", nullptr, SceneDialogue::EnemyCPU, true);

const SceneDialogue tt_dlog8 = SceneDialogue("NICE SAVE.", nullptr, SceneDialogue::EnemyPilot, true);

const SceneDialogue tt_dlog9 = SceneDialogue("THANKS.", nullptr, SceneDialogue::EnemyCPU, true);

const SceneFunc tt_f0 = SceneFunc([](){
    Enemy::setMaxMechs(5, true);
    Enemy::setMaxTanks(1, true);

    Barracks::getBarracksAt({27, 99})->setSpawnsTanks();


    Barracks::getBarracksAt({27, 99})->setLife(27);
    Barracks::getBarracksAt({25, 99})->setLife(27);
    Barracks::getBarracksAt({30, 99})->setLife(27);

    Barracks::getBarracksAt({27, 99})->disablePathfindingChecks();
    Barracks::getBarracksAt({25, 99})->disablePathfindingChecks();
    Barracks::getBarracksAt({30, 99})->disablePathfindingChecks();

    RegionTransitionHandler::goBoss(false);
    UI::showBoss(Barracks::getBarracksAt({27, 99})->getLifePtr());
    EnemyTurret::setAllEnabled(true);
    MapManager::setTileAt(26 * 6 + 3, 110 * 6 + 3, 236);
    MapManager::setTileAt(27 * 6 + 3, 110 * 6 + 3, 236);
    MapManager::setTileAt(28 * 6 + 3, 110 * 6 + 3, 236);
    POIs::setShopsDisabled(true);

    MapManager::setTileAt(16 * 6 + 3, 100 * 6 + 3, 218);
    MapManager::setTileAt(16 * 6 + 3, 101 * 6 + 3, 218);
    MapManager::setTileAt(16 * 6 + 3, 102 * 6 + 3, 218);
    MapManager::setTileAt(16 * 6 + 3, 103 * 6 + 3, 218);
    MapManager::setTileAt(16 * 6 + 3, 104 * 6 + 3, 218);

    registerCallback({Barracks::getBarracksAt({27, 99})->getLifePtr(), Barracks::getBarracksAt({25, 99})->getLifePtr(), Barracks::getBarracksAt({30, 99})->getLifePtr(), Enemy::getTurretAt({19, 94})->getLifePtr(), Enemy::getTurretAt({35, 94})->getLifePtr(), Enemy::getTurretAt({19, 105})->getLifePtr(), Enemy::getTurretAt({35, 105})->getLifePtr()}, [](){
        POIs::setShopsDisabled(false);
        RegionTransitionHandler::leaveBoss();
        EnemyTurret::setAllEnabled(false);
        Enemy::setMaxMechs();
        Enemy::setMaxTanks();
        MapManager::setTileAt(16 * 6 + 3, 100 * 6 + 3, 184);
        MapManager::setTileAt(16 * 6 + 3, 101 * 6 + 3, 184);
        MapManager::setTileAt(16 * 6 + 3, 102 * 6 + 3, 184);
        MapManager::setTileAt(16 * 6 + 3, 103 * 6 + 3, 184);
        MapManager::setTileAt(16 * 6 + 3, 104 * 6 + 3, 184);
    });
    return true;
});

const SceneFunc stop_music = SceneFunc([](){
   AudioSystem::playSong(musNone);
   return true;
});

const SceneSequence tank_boss_scene[] = {
    {SceneSequence::DoFunction, &stop_music},
    {SceneSequence::MoveCamera, &tt_m0 },
    {SceneSequence::Wait, &wait30 },
    {SceneSequence::ShowDialogue, &tt_dlog0 },
    {SceneSequence::ShowDialogue, &tt_dlog1 },
    {SceneSequence::ShowDialogue, &tt_dlog2 },
    {SceneSequence::ShowDialogue, &tt_dlog3 },
    {SceneSequence::ShowDialogue, &tt_dlog4 },
    {SceneSequence::MoveCamera, &tt_m2 },
    {SceneSequence::ShowDialogue, &tt_dlog5 },
    {SceneSequence::MoveCamera, &tt_m1 },
    {SceneSequence::ShowDialogue, &tt_dlog6 },
    {SceneSequence::Wait, &wait60 },
    {SceneSequence::ShowDialogue, &tt_dlog7 },
    {SceneSequence::MoveCamera, &tt_m2 },
    {SceneSequence::ShowDialogue, &tt_dlog8 },
    {SceneSequence::MoveCamera, &tt_m1 },
    {SceneSequence::ShowDialogue, &tt_dlog9 },
    {SceneSequence::MoveCamera, &cam_return },
    {SceneSequence::DoFunction, &tt_f0 },
    {SceneSequence::End, &cam_release}
};

const SceneMoveCam by_m0 = SceneMoveCam(cameraCutsceneSpeed, {165, 206});

const SceneDialogue by_dlog0 = SceneDialogue("Soldier, come in.", nullptr, SceneDialogue::Base, false);
//const SceneDialogue dlog1 = SceneDialogue("This is central.", nullptr, false, false);
const SceneDialogue by_dlog2 = SceneDialogue("Great job making it", "to the boat yard.", SceneDialogue::Base, true);

const SceneDialogue by_dlog3 = SceneDialogue("But it looks like","it's out of order.", SceneDialogue::Base, true);
const SceneDialogue by_dlog4 = SceneDialogue("Get the blueprint","and hit that switch.", SceneDialogue::Base, false);
const SceneDialogue by_dlog5 = SceneDialogue("That will lower a","bridge in the north.", SceneDialogue::Base, false);
const SceneDialogue by_dlog6 = SceneDialogue("We've detected a","boat yard up there.", SceneDialogue::Base, false);

const SceneSequence boatyard_scene[] = {
    {SceneSequence::ShowDialogue, &by_dlog0 },
    {SceneSequence::ShowDialogue, &dlog1 },
    {SceneSequence::ShowDialogue, &by_dlog2 },

    {SceneSequence::MoveCamera, &by_m0 },
    {SceneSequence::Wait, &wait60 },
    {SceneSequence::ShowDialogue, &by_dlog3 },
    {SceneSequence::ShowDialogue, &by_dlog4 },
    {SceneSequence::ShowDialogue, &by_dlog5 },
    {SceneSequence::ShowDialogue, &by_dlog6 },
    {SceneSequence::ShowDialogue, &dlog6 },

    {SceneSequence::MoveCamera, &cam_return },
    { SceneSequence::SetQuestStatus, &qbuildboat },
    {SceneSequence::End, &cam_release}
};

const SceneDialogue ah_dlog0 = SceneDialogue("Soldier, come in.", nullptr, SceneDialogue::Base, false);
const SceneDialogue ah_dlog1 = SceneDialogue("Good job getting", "the helicopter.", SceneDialogue::Base, false);
const SceneDialogue ah_dlog2 = SceneDialogue("I tagged the final", "target on your map.", SceneDialogue::Base, false);
const SceneDialogue ah_dlog3 = SceneDialogue("Head over and", "finish them off.", SceneDialogue::Base, true);


const SceneSequence acquiredheli_scene[] = {
    {SceneSequence::ShowDialogue, &ah_dlog0 },
    {SceneSequence::ShowDialogue, &ah_dlog1 },
    {SceneSequence::ShowDialogue, &ah_dlog2 },
    {SceneSequence::ShowDialogue, &ah_dlog3 },
    { SceneSequence::SetQuestStatus, &qgoend },
    {SceneSequence::End, nullptr}
};

const SceneMoveCam fb_m0 = SceneMoveCam(cameraCutsceneSpeed, {100, 140});
const SceneMoveCam fb_m1 = SceneMoveCam(cameraCutsceneSpeed, {116, 138});
const SceneMoveCam fb_m2 = SceneMoveCam(cameraCutsceneSpeed, {116, 159});
const SceneMoveCam fb_m3 = SceneMoveCam(cameraCutsceneSpeed, {132, 170});

const SceneDialogue fb_dlog0 = SceneDialogue("Soldier, come in.", nullptr, SceneDialogue::Base, false);
const SceneDialogue fb_dlog1 = SceneDialogue("You've finally made it.", nullptr, SceneDialogue::Base, true);
const SceneDialogue fb_dlog2 = SceneDialogue("This is the enemy", "mainframe.", SceneDialogue::Base, true);

const SceneDialogue fb_dlog3 = SceneDialogue("Get a good look.",nullptr, SceneDialogue::Base, true);
const SceneDialogue fb_dlog4 = SceneDialogue("Destroy these", "bases...", SceneDialogue::Base, true);
const SceneDialogue fb_dlog5 = SceneDialogue("And your mission","is complete.", SceneDialogue::Base, true);
const SceneDialogue fb_dlog6 = SceneDialogue("They've detected you.","Fight hard!", SceneDialogue::Base, false);

constexpr int boss_barracks_life = 40;

inline int updateBossBarracks(int lx, int ly, bool * configured) {
    if (Barracks::isDestroyed(lx, ly)) {
        return 0;
    }
    Barracks * b1 = Barracks::getBarracksAt({float(lx), float(ly)});
    if (b1 != nullptr) {
        if (!*configured) {
            b1->disablePathfindingChecks();
            b1->disableDestroyOutOfRange();
            b1->setLife(boss_barracks_life);
            *configured = true;
        }
        return b1->getLife();
    }
    return boss_barracks_life;
}

inline void randomEnemyShot() {
    int x = (rand() % 10) < 5 ? 70 : -70;
    int y = (rand() % 10) < 5 ? 60 : -60;
    if (rand() % 10 < 6) {
        x = rand() % 110;
        x-= 55;
    } else {
        y = rand() % 88;
        y-= 44;
    }

    AudioSystem::play(sfxEnemyShoot);
    Vec2f loc = Camera::center() + Vec2f(x, y);
    Vec2f dir = Vec2f(-x, -y);
    dir = dir / dir.length();
    ProjectileManager::create(loc, dir * 50.0f, 3, 6.0)
            ->setSprite(BulletMedium)
            ->setIgnoreWalls()
            ->setTargetMask({PlayerTarget, GroundTarget, AirTarget});
}

const SceneFunc fb_triggers = SceneFunc([](){
    RegionTransitionHandler::goBoss(true);
    POIs::setShopsDisabled(true);
    static int8_t life = 24;
    life = 24;
    UI::showBoss(&life);

    static bool configured[4] = {false, false, false, false};
    for(int i = 0; i < 4; ++i) configured[i] = false;

    registerUpdateCallback([&](){
        static int shot_timer = 20;
        static float finished_timer = 3.0f;

        int ltot = 0;
        ltot += updateBossBarracks(100, 140, configured);
        ltot += updateBossBarracks(116, 138, configured+1);
        ltot += updateBossBarracks(116, 159, configured+2);
        ltot += updateBossBarracks(132, 170, configured+3);
        life = float(ltot) / 4.0f / float(boss_barracks_life) * 24.0f;

        shot_timer--;
        if (shot_timer < 0 && ltot > 0) {
            shot_timer = 28;
            randomEnemyShot();
        }

        if (ltot <= 0) {
            if (finished_timer >= 3.0f) {
                Enemy::clearAll();
                ProjectileManager::clear();
                Camera::shake(0.8f, 1.5f);
            }
            finished_timer -= physicsTimestep;
            if (finished_timer <= 0) {
                RegionTransitionHandler::leaveBoss();
                GameVariables::setGameWon();
            }
        }
        return finished_timer <= 0;
    });
    return true;
});

const SceneSequence finalboss_scene[] = {
    {SceneSequence::ShowDialogue, &fb_dlog0 },
    {SceneSequence::ShowDialogue, &fb_dlog1 },

    {SceneSequence::MoveCamera, &fb_m0 },
    {SceneSequence::ShowDialogue, &fb_dlog2 },
    {SceneSequence::MoveCamera, &fb_m1 },
    {SceneSequence::ShowDialogue, &fb_dlog3 },
    {SceneSequence::MoveCamera, &fb_m2 },
    {SceneSequence::ShowDialogue, &fb_dlog4 },
    {SceneSequence::MoveCamera, &fb_m3 },
    {SceneSequence::ShowDialogue, &fb_dlog5 },
    {SceneSequence::MoveCamera, &cam_return },

    {SceneSequence::DoFunction, &fb_triggers },
    {SceneSequence::ShowDialogue, &fb_dlog6 },

    {SceneSequence::End, &cam_release}
};

const SceneDialogue win_dlog0 = SceneDialogue("The enemy is", "defeated!", SceneDialogue::Base, false);
const SceneDialogue win_dlog1 = SceneDialogue("You're the hero", "today, soldier.", SceneDialogue::Base, false);
const SceneDialogue win_dlog2 = SceneDialogue("Now shutting down", "your thread.", SceneDialogue::Base, true);

const SceneSequence win_scene[] = {
    {SceneSequence::ShowDialogue, &win_dlog0 },
    {SceneSequence::ShowDialogue, &win_dlog1 },
    {SceneSequence::ShowDialogue, &win_dlog2 },
    {SceneSequence::End, nullptr}
};
#endif // SEQUENCES_H
