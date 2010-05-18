#ifndef _BUILDINGLOADER_HPP_
#define _BUILDINGLOADER_HPP_

#include <string>
#include <iostream>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "SimultyException.hpp"
#include "Building.hpp"

class BuildingInformation {

    public:

    unsigned char type;
    int id;

    std::string name;
    std::string description;

    int costBuy, costDemolish, costUpkeep;
    unsigned char sizeWidth, sizeHeight;
    
    std::string graphicLocation;

};


class BuildingLoader {
    
    private:
    
    static unsigned char typeStringToTypeNumber(std::string);
    
    
    public:
    
    static BuildingInformation *loadBuildingInformation(std::string);
    
};

#endif
