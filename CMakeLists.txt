cmake_minimum_required(VERSION 3.15)
project(project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-narrowing -O3")

if( ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
    set(USE_FLAGS "-s USE_SDL=2 --embed-file ../content/sfx_desktop@sfx --embed-file favicon.ico -fsanitize=undefined")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${USE_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS} -lidbfs.js -s ASYNCIFY -s FORCE_FILESYSTEM=1 --shell-file ../shell.html")
    set(CMAKE_EXECUTABLE_SUFFIX .html)
else()
    find_package(SDL2 REQUIRED)
endif()

include_directories(${CMAKE_SOURCE_DIR} ${SDL2_INCLUDE_DIRS})

set(SOURCES
    core/rendering/spritewrapper.cpp 
    core/utilities/babyfsm.cpp 
    core/utilities/fpshelper.cpp 
    game/entities/barracks.cpp 
    game/entities/enemies/enemyaihelper.cpp 
    game/entities/enemies/enemyboat.cpp 
    game/entities/enemies/enemybomber.cpp 
    game/entities/enemies/enemyhelicopter.cpp 
    game/entities/enemies/enemylasers.cpp 
    game/entities/enemies/enemymech.cpp 
    game/entities/enemies/enemytank.cpp 
    game/entities/enemies/enemyturret.cpp 
    game/entities/enemy.cpp 
    game/entities/pickups.cpp 
    game/entities/pois.cpp 
    game/maps/alertregion.cpp 
    game/maps/regiontransitionhandler.cpp 
    game/maps/spawnpoint.cpp 
    game/maps/spawnpoints.cpp 
    game/physics/pathfinding.cpp 
    game/player.cpp 
    core/rendering/screenbuffer.cpp 
    game/entities/effects.cpp 
    game/entities/projectile.cpp 
    game/physics/body.cpp 
    game/physics/collisionmanager.cpp 
    game/physics/steering.cpp 
    core/rendering/camera.cpp 
    game/rendering/skytilemap.cpp 
    game/sequences.cpp 
    game/states/blueprints.cpp 
    game/states/collectblueprintprompt.cpp 
    game/states/eventscene.cpp 
    game/states/game.cpp 
    game/states/gameover.cpp 
    game/states/gamewon.cpp 
    game/states/mapviewer.cpp 
    game/states/opendoorprompt.cpp 
    game/states/openshopprompt.cpp 
    game/states/optionsviewer.cpp 
    game/states/pause.cpp 
    game/states/repairs.cpp 
    game/states/shop.cpp 
    game/states/title.cpp 
    game/ui/ui.cpp 
    game/utilities/blinker.cpp 
    game/utilities/debuglog.cpp 
    game/utilities/helpers.cpp 
    game/utilities/mapmanager.cpp 
    game/utilities/playerstats.cpp 
    core/utilities/tween.cpp 
    core/utilities/vec.cpp 
    game/maps/sequencetrigger.cpp 
    game/utilities/screentilestore.cpp 
    game/utilities/ticker.cpp 
    game/variables.cpp 
    game/weapons.cpp 
    main.cpp 
    core/audiosystem.cpp 
    core/utilities/rect.cpp 
    core/controls.cpp 
    core/pokittolibextensions.cpp 
    core/rendersystem.cpp
    )

#set(SOURCES_THIRDPARTY
#    thirdparty/glad/glad.c)

set(HEADERS
    core/audiosystem.h 
    core/utilities/objectpool.h 
    game/constants.h 
    game/entities/barracks.h 
    game/entities/enemies/enemyaihelper.h 
    game/entities/enemies/enemyboat.h 
    game/entities/enemies/enemybomber.h 
    game/entities/enemies/enemyhelicopter.h 
    game/entities/enemies/enemylasers.h 
    game/entities/enemies/enemymech.h 
    game/entities/enemies/enemytank.h 
    game/entities/enemies/enemyturret.h 
    game/entities/enemy.h 
    game/entities/pickups.h 
    game/entities/pois.h 
    game/enums.h 
    game/funcs.h 
    game/maps/alertregion.h 
    game/maps/regiontransitionhandler.h 
    game/maps/spawnpoint.h 
    game/maps/spawnpoints.h 
    game/physics/pathfinding.h 
    core/rendering/screenbuffer.h 
    core/rendering/tilemap.h 
    core/utilities/babyfsm.h 
    core/pokittolibextensions.h 
    core/rendersystem.h 
    core/serialization.h 
    core/utilities/fpshelper.h 
    core/utilities/rect.h 
    core/controls.h 
    game/entities/effects.h 
    game/entities/projectile.h 
    game/maps/worldtiles.h 
    game/physics/body.h 
    game/physics/collisionmanager.h 
    game/physics/steering.h 
    game/player.h 
    core/rendering/camera.h 
    game/rendering/skytilemap.h 
    core/rendering/spritewrapper.h 
    game/sequences.h 
    game/states/blueprints.h 
    game/states/collectblueprintprompt.h 
    game/states/eventscene.h 
    game/states/game.h 
    game/states/gameover.h 
    game/states/gamewon.h 
    game/states/mapviewer.h 
    game/states/opendoorprompt.h 
    game/states/openshopprompt.h 
    game/states/optionsviewer.h 
    game/states/pause.h 
    game/states/repairs.h 
    game/states/shop.h 
    game/states/title.h 
    game/ui/ui.h 
    game/utilities/blinker.h 
    game/utilities/debuglog.h 
    game/utilities/helpers.h 
    game/utilities/mapmanager.h 
    game/utilities/playerstats.h 
    game/utilities/rumbler.h 
    core/utilities/tween.h 
    core/utilities/vec.h 
    game/utilities/sceneobjects.h 
    game/maps/sequencetrigger.h 
    game/utilities/screentilestore.h 
    game/utilities/ticker.h 
    game/variables.h 
    game/sprites.h 
    game/weapons.h 
    core/palettes.h 
    game/alertpalettes.h
    )

add_compile_definitions(SDL_CORE DESKTOP_BUILD)

add_executable(project ${SOURCES} ${HEADERS})
target_link_libraries(project ${SDL2_LIBRARIES})

if( ${CMAKE_SYSTEM_NAME} MATCHES "Emscripten")
	em_link_js_library(index ${libraryJsFiles})
endif()