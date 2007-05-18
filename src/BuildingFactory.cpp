
#include "BuildingFactory.hpp"


Building *BuildingFactory::getBuilding(int type, Point position, int owner) {


    switch(type) {
    
        case Building::TYPE_POLICE: {
            return new BuildingPolice(position, owner);
        }
        
        case Building::TYPE_FIRE: {
        
            break;
        } 
        
        case Building::TYPE_HOSPITAL: {
        
            break;
        }
    
    }

    return NULL; // Throw!
}

