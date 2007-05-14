#include "building_manager.h"
#include "client.h"


int building_manager::thrive_value_connection_get(map *m, char slot, int x, int y) {

    for(int xv = x - 3; xv <= x + 3; xv++)
        for(int yv = y - 3; yv <= y + 3; yv++)
            if(m->get(xv, yv).owner == slot 
                    && m->get(xv, yv).road)return 20;

    return 0;

}

int building_manager::thrive_value_electricity_get(map *m, char slot, int x, int y) {

    return 0;

}

int building_manager::thrive_value_taxes_get(char slot) {

    return 0;

}

int building_manager::thrive_value_get(map *m, char slot, int x, int y) {

    return thrive_value_connection_get(m, slot, x, y)
            + thrive_value_electricity_get(m, slot, x, y)
            + thrive_value_taxes_get(slot);

}

