SUBDIRS(NL error)
INCLUDE(FindSDL)

IF(WIN32)
  SET(CMAKE_CXX_FLAGS "-D__GTHREAD_HIDE_WIN32API -g -Wall")
  SET(CMAKE_CXX_FLAGS_DEBUG "-D__GTHREAD_HIDE_WIN32API -g -Wall")
ELSE(WIN32)
  SET(CMAKE_CXX_FLAGS "-g -Wall -pedantic")
  SET(CMAKE_CXX_FLAGS_DEBUG "-g -Wall -pedantic")
ENDIF(WIN32)

# SDL
##############################################################################

Find_Package ( SDL       REQUIRED )
Find_Package ( SDL_image REQUIRED )
Find_Package ( SDL_mixer REQUIRED )
Find_Package ( SDL_ttf   REQUIRED )

if ( NOT SDL_FOUND )
   message ( FATAL_ERROR "SDL not found!" )
endif ( NOT SDL_FOUND )

link_libraries (
   ${SDL_LIBRARY}
   ${SDLIMAGE_LIBRARY}
   ${SDLTTF_LIBRARY}
   SDLmain
)

# GUICHAN
################################################################################

Find_Library(GUICHAN NAMES guichan)
Find_Library(GUICHANSDL NAMES guichan_sdl)


INCLUDE_DIRECTORIES(${Simulty_SOURCE_DIR}/src/NL 
                    ${Simulty_SOURCE_DIR}/src/error
                    ${SDL_INCLUDE_DIR})

LINK_DIRECTORIES(${Simulty_BINARY_DIR}/src/NL 
                 ${Simulty_BINARY_DIR}/src/error)

# Server:
###############################################################################
ADD_EXECUTABLE(../simulty-server Server.cpp Point.cpp Map.cpp PlayerManager.cpp
    Tile.cpp Player.cpp player_server_network.cpp player_server_ai.cpp
    Building.cpp BuildingZone.cpp BuildingPolice.cpp BuildingFire.cpp
    BuildingHospital.cpp BuildingManager.cpp BuildingFactory.cpp
    BuildingResidential.cpp BuildingCommersial.cpp BuildingIndustrial.cpp
    PlayerManagerServer.cpp BuildingManagerServer.cpp ThriveMap.cpp Date.cpp
    Budget.cpp)

target_link_libraries(../simulty-server NL elog)

# Client:
###############################################################################
ADD_EXECUTABLE(../simulty-client Client.cpp Point.cpp Map.cpp PlayerManager.cpp
    Tile.cpp Player.cpp player_client_local.cpp player_client_remote.cpp 
    Building.cpp BuildingZone.cpp BuildingFactory.cpp BuildingFire.cpp
    BuildingHospital.cpp BuildingManager.cpp BuildingPolice.cpp
    BuildingResidential.cpp BuildingCommersial.cpp BuildingIndustrial.cpp 
    Date.cpp Budget.cpp

    guis/sdl/SDLGUI.cpp
    guis/sdl/Camera.cpp
    guis/sdl/MapRender.cpp
    guis/sdl/BuildingRender.cpp
    guis/sdl/widgets/MainMenu.cpp
    guis/sdl/widgets/Toolbar.cpp
    guis/sdl/widgets/MiniMap.cpp
    guis/sdl/widgets/sdltruetypefont.cpp)
    
target_link_libraries(../simulty-client NL elog ${GUICHAN} ${GUICHANSDL})
