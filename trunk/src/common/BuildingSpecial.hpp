#ifndef _BUILDINGSPECIAL_HPP_
#define _BUILDINGSPECIAL_HPP_

#include "Building.hpp"
#include "BuildingInformation.hpp"

class BuildingSpecial: public Building {

    private:
    
    BuildingInformation *bi;

    public:

    BuildingSpecial(BuildingInformation *bi, Point position, 
            unsigned char owner, Date built);
    virtual ~BuildingSpecial();

    virtual unsigned char getType();

    BuildingInformation* getBuildingInformation();

};

#endif

