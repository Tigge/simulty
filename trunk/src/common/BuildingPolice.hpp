#ifndef _BUILDINGPOLICE_HPP_
#define _BUILDINGPOLICE_HPP_

#include "BuildingSpecial.hpp"

class BuildingPolice: public BuildingSpecial {

    public:

    BuildingPolice(BuildingInformation *bi, Point position, int owner, Date built);

};

#endif
