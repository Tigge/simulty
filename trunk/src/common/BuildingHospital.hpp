#ifndef _BUILDINGHOSPITAL_HPP_
#define _BUILDINGHOSPITAL_HPP_

#include "BuildingSpecial.hpp"

class BuildingHospital: public BuildingSpecial {

    public:

    BuildingHospital(BuildingInformation *bi, Point position, int owner, Date built);

};

#endif
