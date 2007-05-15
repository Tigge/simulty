
SUBDIRS(NL error)

IF(WIN32)
  SET(CMAKE_CXX_FLAGS "-D__GTHREAD_HIDE_WIN32API -g -Wall")
  SET(CMAKE_CXX_FLAGS_DEBUG "-D__GTHREAD_HIDE_WIN32API -g -Wall")  
ELSE(WIN32)
  SET(CMAKE_CXX_FLAGS "-g -Wall")
  SET(CMAKE_CXX_FLAGS_DEBUG "-g -Wall")
ENDIF(WIN32)

INCLUDE_DIRECTORIES(${Simulty_SOURCE_DIR}/src/NL ${Simulty_SOURCE_DIR}/src/error /usr/local/include)

LINK_DIRECTORIES(${Simulty_BINARY_DIR}/src/NL ${Simulty_BINARY_DIR}/src/error /usr/local/lib L/usr/X11R6/lib)

# Server:
###############################################################################
ADD_EXECUTABLE(../simulty-server server.cpp game_server.cpp game_server_player.cpp game_server_packet.cpp Point.cpp Map.cpp PlayerManager.cpp player_manager_server.cpp Player.cpp player_server_network.cpp player_server_ai.cpp Building.cpp BuildingManager.cpp Tile.cpp)

IF(WIN32)
  TARGET_LINK_LIBRARIES(../simulty-server NL elog alleg wsock32)
ELSE(WIN32)
  TARGET_LINK_LIBRARIES(../simulty-server NL elog alleg-4.2.0)
ENDIF(WIN32)

# Client:
###############################################################################
ADD_EXECUTABLE(../simulty-client Client.cpp Point.cpp Map.cpp GUIMap.cpp Tile.cpp PlayerManager.cpp Player.cpp player_client_local.cpp player_client_remote.cpp Building.cpp BuildingManager.cpp GUI.cpp Mouse.cpp Camera.cpp)

IF(WIN32)
  TARGET_LINK_LIBRARIES(../simulty-client NL elog alleg wsock32)
ELSE(WIN32)
  TARGET_LINK_LIBRARIES(../simulty-client NL elog alleg-4.2.0)
ENDIF(WIN32)




