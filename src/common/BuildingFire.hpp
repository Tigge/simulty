#ifndef _BUILDINGFIRE_HPP_
#define _BUILDINGFIRE_HPP_

#include "BuildingSpecial.hpp"

class BuildingFire: public BuildingSpecial {

    public:

    BuildingFire(BuildingInformation *bi, Point position, int owner, Date built);
    
};

#endif
