#ifndef _BUILDINGPOWERPLANT_HPP_
#define _BUILDINGPOWERPLANT_HPP_

#include "BuildingSpecial.hpp"

class BuildingPowerplant: public BuildingSpecial {

    public:

    BuildingPowerplant(BuildingInformation *bi, Point position, int owner, Date built);

};

#endif
