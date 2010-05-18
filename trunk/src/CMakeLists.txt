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

Find_Package ( LibXml2   REQUIRED )

if ( NOT SDL_FOUND )
   message ( FATAL_ERROR "SDL not found!" )
endif ( NOT SDL_FOUND )

link_libraries (
   ${SDL_LIBRARY}
   ${SDLIMAGE_LIBRARY}
   ${SDLTTF_LIBRARY}
   SDLmain
   ${LIBXML2_LIBRARIES}
)

# GUICHAN
################################################################################

Find_Library(GUICHAN NAMES guichan)
Find_Library(GUICHANSDL NAMES guichan_sdl)


INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib/NL
                    ${PROJECT_SOURCE_DIR}/lib/error
                    ${SDL_INCLUDE_DIR}
                    ${LIBXML2_INCLUDE_DIR}
                    common)

LINK_DIRECTORIES(${PROJECT_BINARY_DIR}/lib/NL
                 ${PROJECT_BINARY_DIR}/lib/error)

# Server:
###############################################################################
ADD_EXECUTABLE(../simulty-server server/Server.cpp 
    server/ServerListener.cpp common/PacketReceiver.cpp common/PacketSender.cpp
    
    common/Runnable.cpp common/Point.cpp 
    common/Map.cpp common/PlayerManager.cpp common/Tile.cpp common/Player.cpp 
    server/PlayerServerNetwork.cpp server/PlayerServerAI.cpp
    common/Building.cpp common/BuildingZone.cpp common/BuildingPolice.cpp 
    common/BuildingFire.cpp common/BuildingHospital.cpp 
    common/BuildingPowerplant.cpp
    common/BuildingManager.cpp common/BuildingFactory.cpp
    common/BuildingLoader.cpp
    common/BuildingResidential.cpp common/BuildingCommersial.cpp 
    common/BuildingIndustrial.cpp server/PlayerManagerServer.cpp 
    server/BuildingManagerServer.cpp common/ThriveMap.cpp common/Date.cpp
    common/Budget.cpp)

target_link_libraries(../simulty-server NL elog)

# Client:
###############################################################################
ADD_EXECUTABLE(../simulty-client client/Client.cpp common/Point.cpp 
    common/Runnable.cpp common/PacketReceiver.cpp common/PacketSender.cpp
    common/Map.cpp common/PlayerManager.cpp common/Tile.cpp common/Player.cpp 
    client/PlayerClientLocal.cpp client/PlayerClientRemote.cpp 
    common/Building.cpp common/BuildingZone.cpp common/BuildingFactory.cpp 
    common/BuildingFire.cpp common/BuildingHospital.cpp 
    common/BuildingManager.cpp common/BuildingPolice.cpp 
    common/BuildingPowerplant.cpp
    common/BuildingLoader.cpp
    common/BuildingResidential.cpp common/BuildingCommersial.cpp 
    common/BuildingIndustrial.cpp 
    common/Date.cpp common/Budget.cpp

    client/SDLGUI.cpp
    client/Camera.cpp
    client/MapRender.cpp
    client/BuildingRender.cpp
    client/ToolRender.cpp
    client/widgets/MainMenu.cpp
    client/widgets/Toolbar.cpp
    client/widgets/MiniMap.cpp
    client/widgets/Console.cpp
    client/widgets/StatusBar.cpp
    client/widgets/sdltruetypefont.cpp)
    
target_link_libraries(../simulty-client NL elog ${GUICHAN} ${GUICHANSDL})
