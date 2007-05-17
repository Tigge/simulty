
#include "BuildingFactory.hpp"


Building *BuildingFactory::getBuilding(int type, Point position) {


    switch(type) {
    
        case Building::TYPE_POLICE: {
            return new BuildingPolice(position);
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

