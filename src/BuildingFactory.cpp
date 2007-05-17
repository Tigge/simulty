
#include "BuildingFactory.hpp"

#include "Building.hpp"
#include "BuildingPolice.hpp"


Building *BuildingFactory::getBuilding(int type, int x, int y) {


    switch(type) {
    
        case SIMULTY_BUILDING_POLICE: {
            return new BuildingPolice(x, y);
            break;
        }
        
        case SIMULTY_BUILDING_FIRE: {
        
            break;
        } 
        
        case SIMULTY_BUILDING_HOSPITAL: {
        
            break;
        }
    
    }

    return NULL; // Throw!
}

