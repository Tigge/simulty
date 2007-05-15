#include "BuildingManager.hpp"
#include "Client.hpp"


int BuildingManager::thrive_value_connection_get(Map *map, char slot, int x, int y) {

    for(int xv = x - 3; xv <= x + 3; xv++)
        for(int yv = y - 3; yv <= y + 3; yv++)
            if(map->getTile(xv, yv)->getOwner() == slot 
                    && map->getTile(xv, yv)->isRoad())return 20;

    return 0;

}

int BuildingManager::thrive_value_electricity_get(Map *map, char slot, int x, int y) {

    return 0;

}

int BuildingManager::thrive_value_taxes_get(char slot) {

    return 0;

}

int BuildingManager::thrive_value_get(Map *map, char slot, int x, int y) {

    return thrive_value_connection_get(map, slot, x, y)
            + thrive_value_electricity_get(map, slot, x, y)
            + thrive_value_taxes_get(slot);

}

