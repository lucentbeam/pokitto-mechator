TEMPLATE = app
CONFIG += console c++17
CONFIG += app_bundle
CONFIG -= qt

SOURCES += \
    core/rendering/spritewrapper.cpp \
    core/utilities/babyfsm.cpp \
    core/utilities/fpshelper.cpp \
    game/entities/barracks.cpp \
    game/entities/enemies/enemyboat.cpp \
    game/entities/enemies/enemybomber.cpp \
    game/entities/enemies/enemyhelicopter.cpp \
    game/entities/enemies/enemylasers.cpp \
    game/entities/enemies/enemymech.cpp \
    game/entities/enemies/enemytank.cpp \
    game/entities/enemies/enemyturret.cpp \
    game/entities/enemy.cpp \
    game/entities/pickups.cpp \
    game/entities/pois.cpp \
    game/maps/alertregion.cpp \
    game/maps/regiontransitionhandler.cpp \
    game/maps/spawnpoint.cpp \
    game/maps/spawnpoints.cpp \
    game/physics/pathfinding.cpp \
    game/player.cpp \
    game/rendering/cloudmanager.cpp \
    core/rendering/screenbuffer.cpp \
    game/entities/effects.cpp \
    game/entities/projectile.cpp \
    game/physics/body.cpp \
    game/physics/collisionmanager.cpp \
    game/physics/steering.cpp \
    core/rendering/camera.cpp \
    game/rendering/skytilemap.cpp \
    game/sequences.cpp \
    game/states/blueprints.cpp \
    game/states/collectblueprintprompt.cpp \
    game/states/eventscene.cpp \
    game/states/game.cpp \
    game/states/gameover.cpp \
    game/states/gamewon.cpp \
    game/states/mapviewer.cpp \
    game/states/opendoorprompt.cpp \
    game/states/openshopprompt.cpp \
    game/states/pause.cpp \
    game/states/repairs.cpp \
    game/states/shop.cpp \
    game/states/title.cpp \
    game/ui/ui.cpp \
    game/utilities/blinker.cpp \
    game/utilities/helpers.cpp \
    game/utilities/mapmanager.cpp \
    game/utilities/playerstats.cpp \
    core/utilities/tween.cpp \
    core/utilities/vec.cpp \
    game/maps/sequencetrigger.cpp \
    game/utilities/screentilestore.cpp \
    game/utilities/ticker.cpp \
    game/variables.cpp \
    game/weapons.cpp \
    main.cpp \
    core/audiosystem.cpp \
    core/utilities/rect.cpp \
    core/controls.cpp \
    core/pokittolibextensions.cpp \
    core/rendersystem.cpp

HEADERS += \
    core/audiosystem.h \
    core/utilities/objectpool.h \
    game/constants.h \
    game/entities/barracks.h \
    game/entities/enemies/enemyboat.h \
    game/entities/enemies/enemybomber.h \
    game/entities/enemies/enemyhelicopter.h \
    game/entities/enemies/enemylasers.h \
    game/entities/enemies/enemymech.h \
    game/entities/enemies/enemytank.h \
    game/entities/enemies/enemyturret.h \
    game/entities/enemy.h \
    game/entities/pickups.h \
    game/entities/pois.h \
    game/enums.h \
    game/funcs.h \
    game/maps/alertregion.h \
    game/maps/regiontransitionhandler.h \
    game/maps/spawnpoint.h \
    game/maps/spawnpoints.h \
    #game/maps/worldmutables.h \
    game/physics/pathfinding.h \
    game/rendering/cloudmanager.h \
    core/rendering/screenbuffer.h \
    core/rendering/tilemap.h \
    core/utilities/babyfsm.h \
    core/pokittolibextensions.h \
    core/rendersystem.h \
    core/serialization.h \
    core/utilities/fpshelper.h \
    core/utilities/rect.h \
    core/controls.h \
    core/settings_pokitto.h \
    game/entities/effects.h \
    game/entities/projectile.h \
    game/maps/worldtiles.h \
    #game/maps/mechator.h \
    #game/maps/mechator_sky.h \
    game/physics/body.h \
    game/physics/collisionmanager.h \
    game/physics/steering.h \
    game/player.h \
    core/rendering/camera.h \
    game/rendering/skytilemap.h \
    core/rendering/spritewrapper.h \
    game/sequences.h \
    game/states/blueprints.h \
    game/states/collectblueprintprompt.h \
    game/states/eventscene.h \
    game/states/game.h \
    game/states/gameover.h \
    game/states/gamewon.h \
    game/states/mapviewer.h \
    game/states/opendoorprompt.h \
    game/states/openshopprompt.h \
    game/states/pause.h \
    game/states/repairs.h \
    game/states/shop.h \
    game/states/title.h \
    game/ui/ui.h \
    game/utilities/blinker.h \
    game/utilities/helpers.h \
    game/utilities/mapmanager.h \
    game/utilities/playerstats.h \
    game/utilities/rumbler.h \
    core/utilities/tween.h \
    core/utilities/vec.h \
    game/utilities/sceneobjects.h \
    game/maps/sequencetrigger.h \
    game/utilities/screentilestore.h \
    game/utilities/ticker.h \
    game/variables.h \
    game/sprites.h \
    game/weapons.h \
    core/palettes.h \
    game/alertpalettes.h

#win32:INCLUDEPATH += C:/SFML\SFML-2.5.1_msvc2017_64/include
#win32:DEPENDPATH += C:/SFML\SFML-2.5.1_msvc2017_64/include
#win32:DEFINES += SFML_STATIC

win32:CONFIG(release, debug|release): LIBS += -LC:/SDL/lib/x64/ -lSDL2 -lSDL2main
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/SDL/lib/x64/ -lSDL2 -lSDL2main

win32:CONFIG(release, debug|release): CONFIG -= console

win32:INCLUDEPATH += C:/SDL/include/
win32:DEPENDPATH += C:/SDL/lib/x64/

#win32:CONFIG(release, debug|release): LIBS += -LC:/SFML/SFML-2.5.1_msvc2017_64/lib/ -lsfml-main -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lfreetype #-ljpeg
#else:win32:CONFIG(debug, debug|release): LIBS += -LC:/SFML/SFML-2.5.1_msvc2017_64/lib/ -lsfml-main-d -lsfml-graphics-s-d -lsfml-window-s-d -lsfml-system-s-d -lfreetype #-ljpeg
#win32: LIBS += -L"$$PWD/../../../../../Program Files (x86)/Windows Kits/10/Lib/10.0.16299.0/um/x64/" -lwinmm -lgdi32 -lopengl32 -luser32 -ladvapi32

mac: QMAKE_LFLAGS += -F$$PWD/../../SFML-2.5.1/Frameworks/
mac: LIBS += -framework SFML -framework sfml-system -framework sfml-window -framework sfml-graphics

mac: INCLUDEPATH += $$PWD/../../SFML-2.5.1/include
mac: DEPENDPATH += $$PWD/../../SFML-2.5.1/include

unix: QMAKE_CXXFLAGS += -Wno-narrowing

CONFIG(debug, debug|release): DEFINES += DEBUGS

DEFINES += DESKTOP_BUILD
DEFINES += SDL_CORE

unix: LIBS += -lsfml-system -lsfml-window -lsfml-graphics -lsfml-audio
unix: LIBS += -lSDL2 -lSDL2main
