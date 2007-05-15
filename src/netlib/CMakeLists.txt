ADD_LIBRARY(NL NLNetwork.cpp NLSocket.cpp NLPacket.cpp NLException.cpp)

IF(WIN32)
  TARGET_LINK_LIBRARIES(NL wsock32)
ENDIF(WIN32)

