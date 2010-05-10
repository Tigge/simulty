ADD_LIBRARY(NL Network.cpp AbstractSocket.cpp ServerSocket.cpp 
               Socket.cpp  Packet.cpp Exception.cpp)

IF(WIN32)
  TARGET_LINK_LIBRARIES(NL wsock32)
ENDIF(WIN32)

