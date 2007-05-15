
SUBDIRS(netlib error)

IF(WIN32)
  SET(CMAKE_CXX_FLAGS "-D__GTHREAD_HIDE_WIN32API -g -Wall")
  SET(CMAKE_CXX_FLAGS_DEBUG "-D__GTHREAD_HIDE_WIN32API -g -Wall")  
ELSE(WIN32)
  SET(CMAKE_CXX_FLAGS "-g -Wall")
  SET(CMAKE_CXX_FLAGS_DEBUG "-g -Wall")
ENDIF(WIN32)

INCLUDE_DIRECTORIES(${Simulty_SOURCE_DIR}/src/netlib ${Simulty_SOURCE_DIR}/src/error /usr/local/include)

LINK_DIRECTORIES(${Simulty_BINARY_DIR}/src/netlib ${Simulty_BINARY_DIR}/src/error /usr/local/lib L/usr/X11R6/lib)

# Server:
ADD_EXECUTABLE(../simulty-server server.cpp game_server.cpp game_server_player.cpp game_server_packet.cpp Point.cpp map_base.cpp player_manager.cpp player_manager_server.cpp player.cpp player_server_network.cpp player_server_ai.cpp building.cpp building_manager.cpp tile.cpp)

IF(WIN32)
TARGET_LINK_LIBRARIES(../simulty-server netlib elog alleg wsock32)
ELSE(WIN32)
TARGET_LINK_LIBRARIES(../simulty-server netlib elog alleg-4.2.0)
ENDIF(WIN32)

# Client:
ADD_EXECUTABLE(../simulty-client client.cpp game_client.cpp game_client_packet.cpp Point.cpp map_base.cpp map.cpp tile.cpp player_manager.cpp player.cpp player_client_local.cpp player_client_remote.cpp building.cpp building_manager.cpp gui.cpp Mouse.cpp Camera.cpp)

IF(WIN32)
TARGET_LINK_LIBRARIES(../simulty-client netlib elog alleg wsock32)
ELSE(WIN32)
TARGET_LINK_LIBRARIES(../simulty-client netlib elog alleg-4.2.0)
ENDIF(WIN32)




