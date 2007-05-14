ADD_LIBRARY(netlib network.cpp socket.cpp functions.cpp packet.cpp)

IF(WIN32)
TARGET_LINK_LIBRARIES(netlib wsock32)
ENDIF(WIN32)

