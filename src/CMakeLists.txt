
SUBDIRS(NL error)

IF(WIN32)
  SET(CMAKE_CXX_FLAGS "-D__GTHREAD_HIDE_WIN32API -g -Wall")
  SET(CMAKE_CXX_FLAGS_DEBUG "-D__GTHREAD_HIDE_WIN32API -g -Wall")
ELSE(WIN32)
  SET(CMAKE_CXX_FLAGS "-g -Wall -pedantic")
  SET(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -pedantic")
ENDIF(WIN32)


# TODO: Fix allegro unshareable problem

FIND_LIBRARY(ALLEGRO NAMES alld-4.2.1 alld-4.2.0 alld)
FIND_LIBRARY(GUICHAN NAMES guichan)
FIND_LIBRARY(GUICHANALLEGRO NAMES guichan_allegro)
FIND_LIBRARY(ALLEGROUNSHARABLE NAMES alld_unsharable)
FIND_LIBRARY(GLYPHKEEPER glyph-alleg)

INCLUDE_DIRECTORIES(${Simulty_SOURCE_DIR}/src/NL ${Simulty_SOURCE_DIR}/src/error /usr/local/include)

LINK_DIRECTORIES(${Simulty_BINARY_DIR}/src/NL ${Simulty_BINARY_DIR}/src/error /usr/local/lib L/usr/X11R6/lib)

# Server:
###############################################################################
ADD_EXECUTABLE(../simulty-server Server.cpp Point.cpp Map.cpp PlayerManager.cpp
    Tile.cpp Player.cpp player_server_network.cpp player_server_ai.cpp
    Building.cpp BuildingZone.cpp BuildingPolice.cpp BuildingFire.cpp
    BuildingHospital.cpp BuildingManager.cpp BuildingFactory.cpp
    BuildingResidential.cpp BuildingCommersial.cpp BuildingIndustrial.cpp
    PlayerManagerServer.cpp BuildingManagerServer.cpp ThriveMap.cpp Date.cpp
    Budget.cpp)

IF(WIN32)
  TARGET_LINK_LIBRARIES(../simulty-server NL elog ${ALLEGRO} wsock32)
ELSE(WIN32)
  TARGET_LINK_LIBRARIES(../simulty-server NL elog ${ALLEGRO})
ENDIF(WIN32)

IF(ALLEGROUNSHARABLE-NOTFOUND)
ELSE(ALLEGROUNSHARABLE-NOTFOUND)
  TARGET_LINK_LIBRARIES(../simulty-server ${ALLEGROUNSHARABLE})
ENDIF(ALLEGROUNSHARABLE-NOTFOUND)

# Client:
###############################################################################
ADD_EXECUTABLE(../simulty-client Client.cpp Point.cpp Map.cpp PlayerManager.cpp
    Tile.cpp Player.cpp player_client_local.cpp player_client_remote.cpp 
    Building.cpp BuildingZone.cpp BuildingFactory.cpp BuildingFire.cpp
    BuildingHospital.cpp BuildingManager.cpp BuildingPolice.cpp
    BuildingResidential.cpp BuildingCommersial.cpp BuildingIndustrial.cpp 
    Date.cpp Budget.cpp

    guis/allegro/AllegroGUI.cpp
    guis/allegro/MapRender.cpp
    guis/allegro/BuildingRender.cpp
    guis/allegro/Mouse.cpp
    guis/allegro/Camera.cpp
    guis/allegro/widgets/MiniMap.cpp 
    guis/allegro/widgets/allegrotruetypefont.cpp
    guis/allegro/widgets/EconomyWindow.cpp
    guis/allegro/widgets/Toolbar.cpp)

IF(WIN32)
  TARGET_LINK_LIBRARIES(../simulty-client NL elog ${GLYPHKEEPER} ${ALLEGRO} ${GUICHAN} ${GUICHANALLEGRO} wsock32 )
ELSE(WIN32)
  TARGET_LINK_LIBRARIES(../simulty-client NL elog ${GLYPHKEEPER} ${ALLEGRO} ${GUICHAN} ${GUICHANALLEGRO})
ENDIF(WIN32)

IF(ALLEGROUNSHARABLE-NOTFOUND)
ELSE(ALLEGROUNSHARABLE-NOTFOUND)
  TARGET_LINK_LIBRARIES(../simulty-client ${ALLEGROUNSHARABLE})
ENDIF(ALLEGROUNSHARABLE-NOTFOUND)

