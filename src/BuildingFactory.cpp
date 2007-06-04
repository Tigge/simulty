
#include "BuildingFactory.hpp"

#include "BuildingPolice.hpp"
#include "BuildingFire.hpp"
#include "BuildingHospital.hpp"

#include "BuildingResidential.hpp"

Building *BuildingFactory::getBuilding(int type, Point position, int owner) {


    switch(type) {
    
        case Building::TYPE_POLICE: {
            return new BuildingPolice(position, owner);
        }

        case Building::TYPE_FIRE: {
            return new BuildingFire(position, owner);
        }

        case Building::TYPE_HOSPITAL: {
            return new BuildingHospital(position, owner);
        }

        case Building::TYPE_RESIDENTIAL: {
            return new BuildingResidential(position, owner);
        }
    }

    throw -1;
}

