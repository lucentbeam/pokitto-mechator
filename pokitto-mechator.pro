TEMPLATE = app
CONFIG += console c++17
CONFIG += app_bundle
CONFIG -= qt

SOURCES += \
    game/physics/collisionmanager.cpp \
    game/physics/steering.cpp \
    game/rendering/camera.cpp \
    game/utilities/mapmanager.cpp \
    game/utilities/tilecollider.cpp \
    game/utilities/vec.cpp \
    main.cpp \
    core/audio/audiosystem.cpp \
    core/utilities/rect.cpp \
    core/controls/controls.cpp \
    core/pokittolibextensions.cpp \
    core/rendering/rendersystem.cpp

HEADERS += \
    core/audio/audiosystem.h \
    core/rendering/tilemap.h \
    core/utilities/babyfsm.h \
    core/pokittolibextensions.h \
    core/rendering/rendersystem.h \
    core/serialization/serialization.h \
    core/utilities/fpshelper.h \
    core/utilities/rect.h \
    core/controls/controls.h \
    core/settings_pokitto.h \
    game/maps/mechator.h \
    game/physics/collisionmanager.h \
    game/physics/steering.h \
    game/player.h \
    game/rendering/camera.h \
    game/tilesets.h \
    game/utilities/mapmanager.h \
    game/utilities/tilecollider.h \
    game/utilities/vec.h

win32:INCLUDEPATH += C:/SFML\SFML-2.5.1_msvc2017_64/include
win32:DEPENDPATH += C:/SFML\SFML-2.5.1_msvc2017_64/include

win32:DEFINES += SFML_STATIC

win32:CONFIG(release, debug|release): LIBS += -LC:/SFML/SFML-2.5.1_msvc2017_64/lib/ -lsfml-main -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lfreetype #-ljpeg
else:win32:CONFIG(debug, debug|release): LIBS += -LC:/SFML/SFML-2.5.1_msvc2017_64/lib/ -lsfml-main-d -lsfml-graphics-s-d -lsfml-window-s-d -lsfml-system-s-d -lfreetype #-ljpeg

win32: LIBS += -L"$$PWD/../../../../../Program Files (x86)/Windows Kits/10/Lib/10.0.16299.0/um/x64/" -lwinmm -lgdi32 -lopengl32 -luser32 -ladvapi32

mac: QMAKE_LFLAGS += -F$$PWD/../../../SFML-2.5.1/Frameworks/
mac: LIBS += -framework SFML -framework sfml-system -framework sfml-window -framework sfml-graphics

mac: INCLUDEPATH += $$PWD/../../../SFML-2.5.1/include
mac: DEPENDPATH += $$PWD/../../../SFML-2.5.1/include

DEFINES += POKITTO_SFML
