# Creating the network library NL
echo "Network Library NL"
echo "=============================================="
cd src/NL
echo " * Compiling..."
g++ -D__GTHREAD_HIDE_WIN32API -c NLNetwork.cpp NLSocket.cpp NLPacket.cpp NLException.cpp
echo " * Linking..."
ar rcs libNL.a *.o
echo ""
cd ../..
sleep 1

# Creating the error log library elog
echo "Error log library"
echo "=============================================="
cd src/error
echo " * Compiling..."
g++ -c elog.cpp
echo " * Linking..."
ar rcs libelog.a elog.o
echo ""
cd ../..
sleep 1

# Compiling the Client
echo "Simulty Client"
echo "=============================================="
echo " * Compiling and linking..."
g++ src/Client.cpp src/Budget.cpp src/Building.cpp src/BuildingManager.cpp src/Date.cpp src/Map.cpp src/Player.cpp src/PlayerManager.cpp src/Point.cpp src/ThriveMap.cpp src/Tile.cpp src/BuildingFactory.cpp src/BuildingFire.cpp src/BuildingHospital.cpp src/BuildingPolice.cpp src/BuildingResidential.cpp src/BuildingCommersial.cpp src/BuildingIndustrial.cpp src/BuildingZone.cpp src/player_client_local.cpp src/guis/allegro/*.cpp src/guis/allegro/widgets/*.cpp -Isrc/NL -I./src/error -L./src/NL -L./src/error -lNL -lelog -lwsock32 -lguichan -lguichan_allegro -lglyph-alleg -lalleg -o simulty-client.exe
echo ""
sleep 1

# Compiling the Server
echo "Simulty Server"
echo "=============================================="
echo " * Compiling and linking..."
g++ src/Server.cpp src/Budget.cpp src/Building.cpp src/BuildingManager.cpp src/BuildingManagerServer.cpp src/Date.cpp src/Map.cpp src/Player.cpp src/PlayerManager.cpp src/PlayerManagerServer.cpp src/Point.cpp src/ThriveMap.cpp src/Tile.cpp src/BuildingFactory.cpp src/BuildingFire.cpp src/BuildingHospital.cpp src/BuildingPolice.cpp src/BuildingResidential.cpp src/BuildingCommersial.cpp src/BuildingIndustrial.cpp src/BuildingZone.cpp src/player_client_local.cpp src/player_server_ai.cpp src/player_server_network.cpp -Isrc/NL -I./src/error -L./src/NL -L./src/error -lNL -lelog -lwsock32 -lalleg -o simulty-server.exe
echo ""
sleep 1
