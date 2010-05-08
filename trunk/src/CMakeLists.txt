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


INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib/NL
                    ${PROJECT_SOURCE_DIR}/lib/error
                    ${SDL_INCLUDE_DIR}
                    common)

LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/lib/NL
                 ${PROJECT_BINARY_DIR}/lib/error)

# Server:
###############################################################################
ADD_EXECUTABLE(../simulty-server server/Server.cpp common/Point.cpp 
    common/Map.cpp common/PlayerManager.cpp common/Tile.cpp common/Player.cpp 
    server/player_server_network.cpp server/player_server_ai.cpp
    common/Building.cpp common/BuildingZone.cpp common/BuildingPolice.cpp 
    common/BuildingFire.cpp common/BuildingHospital.cpp 
    common/BuildingManager.cpp common/BuildingFactory.cpp
    common/BuildingResidential.cpp common/BuildingCommersial.cpp 
    common/BuildingIndustrial.cpp server/PlayerManagerServer.cpp 
    server/BuildingManagerServer.cpp common/ThriveMap.cpp common/Date.cpp
    common/Budget.cpp)

target_link_libraries(../simulty-server NL elog)

# Client:
###############################################################################
ADD_EXECUTABLE(../simulty-client client/Client.cpp common/Point.cpp 
    common/Map.cpp common/PlayerManager.cpp common/Tile.cpp common/Player.cpp 
    client/player_client_local.cpp client/player_client_remote.cpp 
    common/Building.cpp common/BuildingZone.cpp common/BuildingFactory.cpp 
    common/BuildingFire.cpp common/BuildingHospital.cpp 
    common/BuildingManager.cpp common/BuildingPolice.cpp 
    common/BuildingResidential.cpp common/BuildingCommersial.cpp 
    common/BuildingIndustrial.cpp 
    common/Date.cpp common/Budget.cpp

    client/sdl/SDLGUI.cpp
    client/sdl/Camera.cpp
    client/sdl/MapRender.cpp
    client/sdl/BuildingRender.cpp
    client/sdl/ToolRender.cpp
    client/sdl/widgets/MainMenu.cpp
    client/sdl/widgets/Toolbar.cpp
    client/sdl/widgets/MiniMap.cpp
    client/sdl/widgets/Console.cpp
    client/sdl/widgets/StatusBar.cpp
    client/sdl/widgets/sdltruetypefont.cpp)
    
target_link_libraries(../simulty-client NL elog ${GUICHAN} ${GUICHANSDL})
